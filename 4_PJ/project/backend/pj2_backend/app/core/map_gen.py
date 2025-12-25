import random
from collections import deque
from typing import Dict, List, Tuple

from app.models import Item, ItemType, MapStructure, Monster, Room, RoomType


class MapGenerator:
    """Generate dungeon map with backbone, branches, loops, and populated monsters/loot."""

    def __init__(self, seed: int | None = None):
        self.rng = random.Random(seed)
        self._item_id = 1000

    def generate(self, difficulty_factor: float) -> MapStructure:
        rooms: Dict[int, Room] = {}
        edges: List[List[int]] = []

        # 1) Backbone Start->...->Boss
        length = self.rng.randint(6, 10)
        backbone_ids = []
        for i in range(length):
            r_type = RoomType.START if i == 0 else (RoomType.BOSS if i == length - 1 else RoomType.NORMAL)
            rooms[i] = Room(id=i, type=r_type)
            backbone_ids.append(i)
            if i > 0:
                self._connect(rooms, i - 1, i, edges)

        next_id = length
        # 2) Branches on backbone (excluding start/boss)
        for b_id in backbone_ids[1:-1]:
            if self.rng.random() < 0.7:  # 70% chance to spawn branch
                branch_len = self.rng.randint(1, 3)
                curr = b_id
                for _ in range(branch_len):
                    rooms[next_id] = Room(id=next_id, type=RoomType.NORMAL)
                    self._connect(rooms, curr, next_id, edges)
                    curr = next_id
                    next_id += 1

        # Guarantee at least 10 rooms
        while len(rooms) < 10:
            rooms[next_id] = Room(id=next_id, type=RoomType.NORMAL)
            attach = self.rng.choice(list(rooms.keys()))
            if attach == 0:
                attach = self.rng.choice([r for r in rooms.keys() if r != 0])
            self._connect(rooms, attach, next_id, edges)
            next_id += 1

        # 3) Loops: add a few extra edges between non-neighbors
        self._add_loops(rooms, edges, attempts=3)

        # 4) Populate monsters & optional loot
        max_normal_power = 0
        for room in rooms.values():
            if room.type == RoomType.NORMAL:
                dist = self._bfs_dist(rooms, 0, room.id)
                m_power = self._calc_monster_power(dist, difficulty_factor)
                room.monster = Monster(power=m_power)
                max_normal_power = max(max_normal_power, m_power)
                room.loot = self._maybe_generate_loot(difficulty_factor)
            room.visited = room.id == 0
            room.visible = room.id == 0

        # boss monster after knowing max normal
        boss_room = rooms[self._find_boss_id(rooms)]
        boss_power = max_normal_power + int(10 * difficulty_factor) + 5
        boss_room.monster = Monster(power=boss_power, is_boss=True)

        # set initial fog: neighbors of start visible
        for n in rooms[0].neighbors:
            rooms[n].visible = True

        return MapStructure(rooms=rooms, edges=edges)

    # --- helpers ---
    def _connect(self, rooms: Dict[int, Room], u: int, v: int, edges: List[List[int]]) -> None:
        if v not in rooms[u].neighbors:
            rooms[u].neighbors.append(v)
        if u not in rooms[v].neighbors:
            rooms[v].neighbors.append(u)
        edges.append([u, v])

    def _add_loops(self, rooms: Dict[int, Room], edges: List[List[int]], attempts: int = 3) -> None:
        node_ids = list(rooms.keys())
        for _ in range(attempts):
            u, v = self.rng.sample(node_ids, 2)
            if u == v or v in rooms[u].neighbors:
                continue
            self._connect(rooms, u, v, edges)

    def _bfs_dist(self, rooms: Dict[int, Room], start: int, target: int) -> int:
        visited = {start}
        q = deque([(start, 0)])
        while q:
            node, dist = q.popleft()
            if node == target:
                return dist
            for n in rooms[node].neighbors:
                if n not in visited:
                    visited.add(n)
                    q.append((n, dist + 1))
        return 0

    def _calc_monster_power(self, dist: int, factor: float) -> int:
        base = 10 * factor
        growth = dist * 2 * factor
        jitter = self.rng.randint(-3, 3)
        return max(1, int(base + growth + jitter))

    def _maybe_generate_loot(self, factor: float) -> Item | None:
        if self.rng.random() > 0.35:
            return None
        tier = 1 if factor <= 1.0 else (2 if factor <= 1.5 else (3 if factor <= 2.5 else 4))
        bonus_ranges = {1: (1, 5), 2: (6, 15), 3: (16, 30), 4: (31, 60)}
        bonus = self.rng.randint(*bonus_ranges[tier])
        slot = self.rng.choice([
            ItemType.WEAPON,
            ItemType.HELMET,
            ItemType.ARMOR,
            ItemType.BOOTS,
            ItemType.MAGIC_STONE,
        ])
        name = f"Tier{tier} {slot.value.title()}"
        self._item_id += 1
        value = bonus * 10
        return Item(id=self._item_id, name=name, type=slot, power_bonus=bonus, value=value)

    def _find_boss_id(self, rooms: Dict[int, Room]) -> int:
        for rid, room in rooms.items():
            if room.type == RoomType.BOSS:
                return rid
        return max(rooms.keys())


def is_connected(rooms: Dict[int, Room]) -> bool:
    if not rooms:
        return True
    start = next(iter(rooms))
    visited = set([start])
    q = deque([start])
    while q:
        u = q.popleft()
        for v in rooms[u].neighbors:
            if v not in visited:
                visited.add(v)
                q.append(v)
    return len(visited) == len(rooms)


def path_exists(rooms: Dict[int, Room], start: int, target: int) -> bool:
    if start not in rooms or target not in rooms:
        return False
    visited = set([start])
    q = deque([start])
    while q:
        u = q.popleft()
        if u == target:
            return True
        for v in rooms[u].neighbors:
            if v not in visited:
                visited.add(v)
                q.append(v)
    return False
