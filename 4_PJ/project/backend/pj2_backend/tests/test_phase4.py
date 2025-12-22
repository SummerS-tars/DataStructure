import sys
from pathlib import Path

import pytest

# ensure app package importable
BASE_DIR = Path(__file__).resolve().parents[1]
if str(BASE_DIR) not in sys.path:
    sys.path.insert(0, str(BASE_DIR))

from app.core.game_engine import GameEngine  # noqa: E402
from app.models import EquipmentSlots, GameResponse, Item, ItemType, MapStructure, PlayerState, Room, RoomType  # noqa: E402
from app.utils import persistence  # noqa: E402


@pytest.fixture(autouse=True)
def clean_saves():
    persistence.clear_dungeon_state()
    persistence.clear_user_save()
    yield
    persistence.clear_dungeon_state()
    persistence.clear_user_save()


def test_resume_uses_saved_state_when_not_finished():
    engine = GameEngine(seed=10)
    state = engine.new_game(1.0)
    # mutate HP to verify persistence
    state.player.hp = 77
    engine._persist_state()  # type: ignore

    resumed_engine = GameEngine(seed=11)
    resumed = resumed_engine.start_game(2.0, resume=True)

    assert resumed.player.hp == 77
    assert resumed.status == "playing"
    # map structure should be loaded from save (same number of rooms)
    assert len(resumed.map_view.rooms) == len(state.map_view.rooms)


def test_loot_pickup_updates_user_save():
    engine = GameEngine(seed=20)
    rooms = {
        0: Room(id=0, type=RoomType.START, neighbors=[1], visible=True, visited=True),
        1: Room(
            id=1,
            type=RoomType.NORMAL,
            neighbors=[0],
            loot=Item(id=999, name="Test Relic", type=ItemType.WEAPON, power_bonus=5, value=123),
        ),
    }
    edges = [[0, 1]]
    player = PlayerState(
        current_room_id=0,
        hp=100,
        max_hp=100,
        base_power=10,
        total_power=10,
        equipment=EquipmentSlots(),
        inventory=[],
    )
    engine.state = GameResponse(player=player, map_view=MapStructure(rooms=rooms, edges=edges), logs=[], status="playing")

    engine.process_turn(1)

    saved = persistence.load_user_save()
    assert saved is not None
    items = saved.get("collection_items", [])
    assert any(item.get("id") == 999 for item in items)


def test_collection_endpoint_returns_tree():
    from fastapi.testclient import TestClient
    from app.main import app

    client = TestClient(app)
    # force create save via /init
    client.post("/init", params={"difficulty": "easy", "resume": False})
    res = client.get("/collection")
    assert res.status_code == 200
    payload = res.json()
    assert "collection_tree" in payload
    assert payload["collection_tree"]["name"] == "Collection"


def test_save_endpoints_after_init():
    from fastapi.testclient import TestClient
    from app.main import app

    client = TestClient(app)
    client.post("/init", params={"difficulty": "easy", "resume": False})

    res_d = client.get("/save/dungeon")
    assert res_d.status_code == 200
    dungeon = res_d.json()
    assert "state" in dungeon and "turn_count" in dungeon

    res_u = client.get("/save/user")
    assert res_u.status_code == 200
    user = res_u.json()
    assert "collection_tree" in user or "collection_items" in user
