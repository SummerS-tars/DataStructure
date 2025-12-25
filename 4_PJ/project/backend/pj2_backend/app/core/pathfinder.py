import heapq
from typing import Dict, List, Set

from app.models import Room


class PathFinder:
    """Provide shortest path and top-K value paths (simple heuristic)."""

    def get_shortest_path(self, rooms: Dict[int, Room], start_id: int, target_id: int) -> List[int]:
        queue = [(0, start_id, [start_id])]  # cost, node, path
        visited: Set[int] = set()
        while queue:
            cost, u, path = heapq.heappop(queue)
            if u == target_id:
                return path
            if u in visited:
                continue
            visited.add(u)
            for v in rooms[u].neighbors:
                if v not in visited:
                    heapq.heappush(queue, (cost + 1, v, path + [v]))
        return []

    def get_top_k_value_paths(self, rooms: Dict[int, Room], start_id: int, target_id: int, k: int = 3) -> List[dict]:
        all_paths: List[dict] = []

        def dfs(curr: int, path: List[int], seen: Set[int]):
            if curr == target_id:
                val = self._calculate_path_value(rooms, path)
                all_paths.append({"path": path[:], "value": val})
                return
            if len(path) > 15:
                return
            for n in rooms[curr].neighbors:
                if n not in seen:
                    seen.add(n)
                    path.append(n)
                    dfs(n, path, seen)
                    path.pop()
                    seen.remove(n)

        dfs(start_id, [start_id], {start_id})
        return sorted(all_paths, key=lambda x: x["value"], reverse=True)[:k]

    def _calculate_path_value(self, rooms: Dict[int, Room], path: List[int]) -> float:
        value = 0.0
        for rid in path:
            r = rooms[rid]
            if r.loot:
                value += r.loot.value
            if r.monster and r.monster.alive:
                value -= r.monster.power * 0.5
        return value
