import random
from typing import List, Optional, cast

from app.core.loot_manager import LootManager
from app.core.map_gen import MapGenerator
from app.models import EquipmentSlots, GameResponse, Item, MapStructure, Monster, PlayerState, Room, RoomType
from app.utils.persistence import (
    clear_dungeon_state,
    load_dungeon_state,
    load_user_save,
    save_dungeon_state,
    save_user_save,
)


class GameEngine:
    """Core game loop: manage state, moves, combat, loot, and boss movement."""

    def __init__(self, map_generator: Optional[MapGenerator] = None, seed: int | None = None):
        self.map_generator = map_generator or MapGenerator(seed=seed)
        self.rng = random.Random(seed)
        self.state: Optional[GameResponse] = None
        self.boss_move_interval = 2
        self.turn_count = 0
        self.difficulty_factor = 1.0
        self.loot_manager = LootManager()
        self._load_user_collection()
        # ensure user save file exists with current tree
        self._persist_user_collection()

    # --- Public API ---
    def new_game(self, difficulty_factor: float, loadout_ids: Optional[List[int]] = None) -> GameResponse:
        map_view: MapStructure = self.map_generator.generate(difficulty_factor)
        player = PlayerState(
            current_room_id=0,
            hp=100,
            max_hp=100,
            base_power=10,
            total_power=10,
            equipment=EquipmentSlots(),
            inventory=[],
        )
        # Phase6: bring-in loadout deducted from collection
        loadout_ids = loadout_ids or []
        if loadout_ids:
            items = self.loot_manager.extract_items(loadout_ids)
            player.inventory.extend(items)
            # persist removal immediately to prevent duping
            self._persist_user_collection()
        logs = [f"Game initialized (factor={difficulty_factor})."]
        self.turn_count = 0
        self.difficulty_factor = difficulty_factor
        self.state = GameResponse(player=player, map_view=map_view, logs=logs, status="playing")
        # ensure user save exists even if cleaned between runs
        self._persist_user_collection()
        self._persist_state()
        return self._require_state()

    def start_game(self, difficulty_factor: float, resume: bool = True, loadout_ids: Optional[List[int]] = None) -> GameResponse:
        """Resume existing dungeon if allowed and valid; otherwise create new."""
        if resume:
            restored = self.resume_game()
            if restored:
                return restored
        # no valid save
        clear_dungeon_state()
        return self.new_game(difficulty_factor, loadout_ids=loadout_ids)

    def is_valid_move(self, target_room_id: int) -> bool:
        if not self.state or self.state.status != "playing":
            return False
        current = self._current_room()
        return target_room_id in current.neighbors

    def process_turn(self, target_room_id: int) -> GameResponse:
        state = self._require_state()
        if state.status != "playing":
            return state
        if not self.is_valid_move(target_room_id):
            raise ValueError("Invalid move: not a neighbor")

        self.turn_count += 1
        self._enter_room(target_room_id)

        if self.state and self.state.status == "playing" and self.turn_count % self.boss_move_interval == 0:
            self._move_boss()

        self._persist_state()
        return self._require_state()

    def player_equip(self, item_id: int) -> GameResponse:
        state = self._require_state()
        target: Optional[Item] = next((i for i in state.player.inventory if i.id == item_id), None)
        if not target:
            raise ValueError("Item not found in inventory")
        eq = state.player.equipment
        from app.models import ItemType

        if target.type == ItemType.WEAPON:
            eq.weapon = target
        elif target.type == ItemType.HELMET:
            eq.helmet = target
        elif target.type == ItemType.ARMOR:
            eq.armor = target
        elif target.type == ItemType.BOOTS:
            eq.boots = target
        elif target.type == ItemType.MAGIC_STONE:
            eq.magic_stone = target
        self._recalc_power()
        state.logs.append(f"Equipped {target.name}, TotalPower={state.player.total_power}")
        self._persist_state()
        return state

    def use_consumable(self, item_id: int) -> GameResponse:
        state = self._require_state()
        target: Optional[Item] = next((i for i in state.player.inventory if i.id == item_id), None)
        if not target:
            raise ValueError("Item not found in inventory")
        from app.models import ItemType

        if target.type != ItemType.POTION:
            raise ValueError("Item is not consumable")

        # apply effect: restore 20 HP capped by max_hp
        heal = 20
        before = state.player.hp
        state.player.hp = min(state.player.max_hp, state.player.hp + heal)
        # remove the item from inventory (first occurrence)
        state.player.inventory = [i for i in state.player.inventory if i.id != item_id] + []
        state.logs.append(
            f"Used {target.name}, restored {state.player.hp - before} HP (HP {before}->{state.player.hp})."
        )
        self._persist_state()
        return state

    def resume_game(self) -> Optional[GameResponse]:
        data = load_dungeon_state()
        if not data:
            return None
        saved_state = data.get("state")
        if not saved_state:
            return None
        restored = GameResponse.parse_obj(saved_state)
        # skip finished runs
        if restored.status in {"win", "dead"}:
            return None
        self.state = restored
        self.turn_count = data.get("turn_count", 0)
        self.difficulty_factor = data.get("difficulty_factor", 1.0)
        return self._require_state()

    # --- Internals ---
    def _current_room(self) -> Room:
        state = self._require_state()
        return state.map_view.rooms[state.player.current_room_id]

    def _enter_room(self, target_room_id: int) -> None:
        state = self._require_state()
        rooms = state.map_view.rooms
        player = state.player
        room = rooms[target_room_id]

        # combat if monster alive
        if room.monster and room.monster.alive:
            self._resolve_combat(room)
            if state.status == "dead":
                return

        # move player
        player.current_room_id = target_room_id
        room.visited = True
        room.visible = True

        # pickup loot
        if room.loot:
            player.inventory.append(room.loot)
            state.logs.append(f"Picked up {room.loot.name} (+{room.loot.power_bonus} power)")
            # update global collection and persist user save
            self.loot_manager.add_item_to_collection(room.loot)
            self._persist_user_collection()
            room.loot = None
            self._recalc_power()

        # reveal fog
        self._reveal_visibility(target_room_id)

        # win check
        if room.type == RoomType.BOSS and (not room.monster or not room.monster.alive):
            state.status = "win"
            state.logs.append("Boss defeated! You win.")
            # Phase6: return carried gear to collection
            self._return_inventory_to_collection()

    def _resolve_combat(self, room: Room) -> None:
        state = self._require_state()
        player = state.player
        monster: Monster = room.monster  # type: ignore
        if player.total_power >= monster.power:
            monster.alive = False
            state.logs.append(f"Dominance! Defeated monster (power {monster.power}) without damage.")
            return

        diff = monster.power - player.total_power
        cost = int(diff * self.rng.uniform(0.8, 1.2))
        if cost >= player.hp:
            player.hp = 0
            state.status = "dead"
            state.logs.append(
                f"You attacked a too-strong foe (power {monster.power}). Took fatal damage ({cost}), you died."
            )
            return

        player.hp -= cost
        monster.alive = False
        state.logs.append(f"Hard fight! Lost {cost} HP vs monster (power {monster.power}). HP={player.hp}")

    def _reveal_visibility(self, room_id: int) -> None:
        rooms = self._require_state().map_view.rooms
        rooms[room_id].visible = True
        rooms[room_id].visited = True
        for n in rooms[room_id].neighbors:
            rooms[n].visible = True

    def _recalc_power(self) -> None:
        state = self._require_state()
        eq = state.player.equipment
        total_bonus = sum(
            item.power_bonus
            for item in [eq.weapon, eq.helmet, eq.armor, eq.boots, eq.magic_stone]
            if item is not None
        )
        state.player.total_power = state.player.base_power + total_bonus

    def _move_boss(self) -> None:
        state = self._require_state()
        rooms = state.map_view.rooms
        boss_id = self._find_boss_room_id()
        boss_room = rooms[boss_id]
        if not boss_room.monster or not boss_room.monster.alive:
            return
        candidates = [n for n in boss_room.neighbors if rooms[n].type != RoomType.START]
        if not candidates:
            return
        target = self.rng.choice(candidates)
        target_room = rooms[target]
        # swap boss into target
        target_room.monster = boss_room.monster
        boss_room.monster = None
        # move boss room identity as well to avoid stale win conditions
        if boss_room.type == RoomType.BOSS:
            boss_room.type = RoomType.NORMAL
            target_room.type = RoomType.BOSS
        state.logs.append(f"Boss moved to room {target} (hidden)")

        # if boss moved onto the player, resolve immediate combat to avoid forced detour
        if target == state.player.current_room_id and target_room.monster and target_room.monster.alive:
            self._resolve_combat(target_room)
            # win check if boss died on player tile
            if target_room.type == RoomType.BOSS and (not target_room.monster or not target_room.monster.alive):
                state.status = "win"
                state.logs.append("Boss defeated! You win.")

    def _find_boss_room_id(self) -> int:
        state = self._require_state()
        for rid, room in state.map_view.rooms.items():
            if room.monster and room.monster.is_boss:
                return rid
        # fallback: first boss type room
        for rid, room in state.map_view.rooms.items():
            if room.type == RoomType.BOSS:
                return rid
        return 0

    def _require_state(self) -> GameResponse:
        if self.state is None:
            raise ValueError("Game not initialized")
        return cast(GameResponse, self.state)

    def _return_inventory_to_collection(self) -> None:
        """Ingest current inventory and equipped items back into collection (on win)."""
        if not self.state:
            return
        items = list(self.state.player.inventory)
        eq = self.state.player.equipment
        for maybe in [eq.weapon, eq.helmet, eq.armor, eq.boots, eq.magic_stone]:
            if maybe:
                items.append(maybe)
        if items:
            self.loot_manager.ingest_items(items)
            self._persist_user_collection()

    # --- Persistence helpers ---
    def _persist_state(self) -> None:
        if not self.state:
            return
        payload = {
            "state": self.state.dict(),
            "turn_count": self.turn_count,
            "difficulty_factor": self.difficulty_factor,
        }
        save_dungeon_state(payload)

    def _load_user_collection(self) -> None:
        data = load_user_save()
        if not data:
            return
        items_raw = data.get("collection_items", [])
        items: List[Item] = []
        for raw in items_raw:
            try:
                items.append(Item.parse_obj(raw))
            except Exception:
                continue
        self.loot_manager.ingest_items(items)

    def _persist_user_collection(self) -> None:
        payload = {
            "collection_items": self.loot_manager.to_flat_list(),
            "collection_tree": self.loot_manager.to_dict(),
        }
        save_user_save(payload)
