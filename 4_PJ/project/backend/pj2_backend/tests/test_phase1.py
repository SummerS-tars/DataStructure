import sys
from pathlib import Path

from fastapi.testclient import TestClient

# ensure app package is importable when running from repo root
BASE_DIR = Path(__file__).resolve().parents[1]
if str(BASE_DIR) not in sys.path:
    sys.path.insert(0, str(BASE_DIR))

from app.core.loot_manager import LootManager  # noqa: E402
from app.core.map_gen import MapGenerator, is_connected, path_exists  # noqa: E402
from app.main import app  # noqa: E402
from app.models import RoomType


def test_map_generation_connectivity_and_boss_power():
    gen = MapGenerator(seed=123)
    map_view = gen.generate(difficulty_factor=1.5)
    rooms = map_view.rooms

    assert len(rooms) >= 10, "should generate at least 10 rooms"
    assert is_connected(rooms), "graph should be connected"

    boss_id = next(rid for rid, r in rooms.items() if r.type == RoomType.BOSS)
    assert path_exists(rooms, 0, boss_id), "start to boss should have a path"

    assert rooms[boss_id].monster is not None
    boss_monster = rooms[boss_id].monster
    assert boss_monster is not None
    boss_power = boss_monster.power
    normal_powers = [r.monster.power for r in rooms.values() if r.type == RoomType.NORMAL and r.monster]
    assert boss_power > max(normal_powers), "boss must be the strongest"


def test_monster_power_increases_with_distance():
    gen = MapGenerator(seed=999)
    map_view = gen.generate(difficulty_factor=2.0)
    rooms = map_view.rooms

    # compute average power by distance from start
    dist_to_power = {}
    for rid, room in rooms.items():
        if room.type != RoomType.NORMAL:
            continue
        if room.monster is None:
            continue
        dist = gen._bfs_dist(rooms, 0, rid)  # using internal helper is acceptable for test
        dist_to_power.setdefault(dist, []).append(room.monster.power)

    if len(dist_to_power) >= 2:
        near = min(dist_to_power.keys())
        far = max(dist_to_power.keys())
        avg_near = sum(dist_to_power[near]) / len(dist_to_power[near])
        avg_far = sum(dist_to_power[far]) / len(dist_to_power[far])
        assert avg_far >= avg_near, "farther rooms should not be weaker than nearest"


def test_loot_tree_to_dict_contains_categories_and_items():
    lm = LootManager()
    data = lm.to_dict()
    assert data["name"] == "Collection"
    # ensure key categories exist
    names = [child["name"] for child in data["children"]]
    assert "Equipment" in names
    # ensure at least one seeded item exists
    def collect_items(node):
        items = list(node.get("items", []))
        for c in node.get("children", []):
            items.extend(collect_items(c))
        return items

    assert len(collect_items(data)) > 0, "seed items should exist in loot tree"


def test_init_endpoint_returns_valid_game_response():
    client = TestClient(app)
    res = client.post("/init", json={"difficulty": "normal", "loadout_ids": []}, params={"resume": False})
    assert res.status_code == 200
    payload = res.json()

    assert payload["player"]["current_room_id"] == 0
    rooms = payload["map_view"]["rooms"]
    # rooms is dict keyed by stringified int in JSON
    assert len(rooms) >= 10
    # verify bidirectional neighbor for a sample edge
    edges = payload["map_view"]["edges"]
    u, v = edges[0]
    neighbors_u = set(map(str, rooms[str(u)]["neighbors"]))
    neighbors_v = set(map(str, rooms[str(v)]["neighbors"]))
    assert str(v) in neighbors_u
    assert str(u) in neighbors_v

    # boss exists and has highest power
    boss_entries = [r for r in rooms.values() if r["type"] == "boss"]
    assert boss_entries, "boss room must exist"
    boss_power = boss_entries[0]["monster"]["power"]
    normal_powers = [r["monster"]["power"] for r in rooms.values() if r["type"] == "normal"]
    assert boss_power > max(normal_powers)

    assert payload["status"] == "playing"
    assert payload["logs"], "logs should contain init message"
