import random

from app.core.game_engine import GameEngine
from app.models import EquipmentSlots, GameResponse, Item, ItemType, MapStructure, Monster, PlayerState, Room, RoomType


def build_state_for_engine(engine: GameEngine, rooms: dict, edges: list) -> None:
    player = PlayerState(
        current_room_id=0,
        hp=100,
        max_hp=100,
        base_power=10,
        total_power=10,
        equipment=EquipmentSlots(),
        inventory=[],
    )
    map_view = MapStructure(rooms=rooms, edges=edges)
    engine.state = GameResponse(player=player, map_view=map_view, logs=[], status="playing")


def test_move_dominance_no_damage():
    engine = GameEngine(seed=1)
    rooms = {
        0: Room(id=0, type=RoomType.START, neighbors=[1], visible=True, visited=True),
        1: Room(id=1, type=RoomType.NORMAL, neighbors=[0], monster=Monster(power=5, alive=True)),
    }
    edges = [[0, 1]]
    build_state_for_engine(engine, rooms, edges)

    res = engine.process_turn(1)

    assert res.player.hp == 100
    assert res.map_view.rooms[1].monster is not None and not res.map_view.rooms[1].monster.alive
    assert res.status == "playing"


def test_move_hard_fight_costs_hp_and_fog_reveals_neighbors(monkeypatch):
    engine = GameEngine(seed=2)
    rooms = {
        0: Room(id=0, type=RoomType.START, neighbors=[1], visible=True, visited=True),
        1: Room(id=1, type=RoomType.NORMAL, neighbors=[0, 2], monster=Monster(power=25, alive=True)),
        2: Room(id=2, type=RoomType.NORMAL, neighbors=[1], monster=Monster(power=5, alive=True)),
    }
    edges = [[0, 1], [1, 2]]
    build_state_for_engine(engine, rooms, edges)

    # make uniform deterministic (1.0)
    monkeypatch.setattr(engine.rng, "uniform", lambda a, b: 1.0)

    res = engine.process_turn(1)

    assert res.player.hp == 100 - (25 - 10)  # diff 15
    assert res.map_view.rooms[1].visited and res.map_view.rooms[1].visible
    # neighbor of room1 (room2) should be visible now
    assert res.map_view.rooms[2].visible


def test_lethal_fight_sets_dead(monkeypatch):
    engine = GameEngine(seed=3)
    rooms = {
        0: Room(id=0, type=RoomType.START, neighbors=[1], visible=True, visited=True),
        1: Room(id=1, type=RoomType.NORMAL, neighbors=[0], monster=Monster(power=200, alive=True)),
    }
    edges = [[0, 1]]
    build_state_for_engine(engine, rooms, edges)
    monkeypatch.setattr(engine.rng, "uniform", lambda a, b: 1.2)

    res = engine.process_turn(1)

    assert res.status == "dead"
    assert res.player.hp == 0


def test_equip_updates_total_power():
    engine = GameEngine(seed=4)
    rooms = {0: Room(id=0, type=RoomType.START, neighbors=[], visible=True, visited=True)}
    edges = []
    build_state_for_engine(engine, rooms, edges)
    sword = Item(id=42, name="Sword", type=ItemType.WEAPON, power_bonus=7, value=50)
    if engine.state is not None:
        engine.state.player.inventory.append(sword)

    res = engine.player_equip(42)

    assert res.player.total_power == 10 + 7
    assert res.player.equipment.weapon is not None


def test_boss_moves_every_two_turns(monkeypatch):
    engine = GameEngine(seed=5)
    rooms = {
        0: Room(id=0, type=RoomType.START, neighbors=[1], visible=True, visited=True),
        1: Room(id=1, type=RoomType.NORMAL, neighbors=[0, 2]),
        2: Room(id=2, type=RoomType.BOSS, neighbors=[1], monster=Monster(power=50, is_boss=True, alive=True)),
    }
    edges = [[0, 1], [1, 2]]
    build_state_for_engine(engine, rooms, edges)
    # set turn_count so that next move triggers boss move (interval=2)
    engine.turn_count = 1
    # ensure boss chooses neighbor deterministically
    monkeypatch.setattr(engine.rng, "choice", lambda seq: seq[0])

    res = engine.process_turn(1)

    # boss should have moved from 2 to 1 after player's move
    assert res.map_view.rooms[2].monster is None
    assert res.map_view.rooms[1].monster is not None and res.map_view.rooms[1].monster.is_boss
