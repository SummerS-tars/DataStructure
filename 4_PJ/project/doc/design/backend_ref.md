这份文档基于 **Project 2 (Web Remake v4.0)** 的最新需求，为您提供一份详细的 **Python (FastAPI)** 后端实现参考。这份文档将帮助您构建项目的基础架构、数据模型和核心算法。

---

# Data Structure Project 2: Backend Implementation Guide

## 1. 项目目录结构建议

保持结构清晰，将数据模型、业务逻辑和API路由分离。

```text
pj2_backend/
├── app/
│   ├── __init__.py
│   ├── main.py              # FastAPI 入口
│   ├── models.py            # Pydantic 数据模型 (交互协议)
│   ├── core/
│   │   ├── __init__.py
│   │   ├── game_engine.py   # 核心游戏状态机 (单例或状态管理)
│   │   ├── map_gen.py       # 地图生成算法 (Graph)
│   │   ├── loot_manager.py  # 战利品树 (Tree)
│   │   └── pathfinder.py    # 寻路与排序算法 (Dijkstra/DFS)
│   └── utils/
│       ├── __init__.py
│       └── persistence.py   # JSON 读写工具
├── data/
│   ├── user_save.json       # 全局存档
│   └── dungeon_state.json   # 副本存档
├── requirements.txt
└── run.py                   # 启动脚本

```

---

## 2. 核心数据模型 (Data Models)

使用 `Pydantic` 定义数据结构，这些类既用于内部逻辑，也自动生成 API 文档。

### `app/models.py`

```python
from enum import Enum
from typing import List, Optional, Dict, Union
from pydantic import BaseModel

# --- 枚举定义 ---
class ItemType(str, Enum):
    WEAPON = "weapon"
    HELMET = "helmet"
    ARMOR = "armor"
    BOOTS = "boots"
    MAGIC_STONE = "magic_stone"
    POTION = "potion"
    COLLECTIBLE = "collectible"

class RoomType(str, Enum):
    START = "start"
    NORMAL = "normal"
    BOSS = "boss"
    SAFE = "safe" # 泉水/营地

# --- 实体对象 ---
class Item(BaseModel):
    id: int
    name: str
    type: ItemType
    power_bonus: int = 0
    value: int = 0
    description: str = ""

class Monster(BaseModel):
    power: int
    is_boss: bool = False
    alive: bool = True

class Room(BaseModel):
    id: int
    type: RoomType
    neighbors: List[int] = []  # 邻接表
    monster: Optional[Monster] = None
    loot: Optional[Item] = None
    visited: bool = False
    visible: bool = False      # 迷雾状态

class MapStructure(BaseModel):
    rooms: Dict[int, Room]     # ID -> Room 映射
    edges: List[List[int]]     # 可视化连线

# --- 玩家状态 ---
class EquipmentSlots(BaseModel):
    weapon: Optional[Item] = None
    helmet: Optional[Item] = None
    armor: Optional[Item] = None
    boots: Optional[Item] = None
    magic_stone: Optional[Item] = None

class PlayerState(BaseModel):
    current_room_id: int
    hp: int
    max_hp: int = 100
    base_power: int = 10
    total_power: int       # 计算属性
    equipment: EquipmentSlots
    inventory: List[Item]  # 背包 (扁平化，具体树结构在 LootManager 处理)

# --- API 响应 ---
class GameResponse(BaseModel):
    player: PlayerState
    map_view: MapStructure # 前端根据 visible 渲染
    logs: List[str]
    status: str            # "playing", "win", "dead"

```

---

## 3. 核心算法实现 (Core Logic)

### 3.1. 战利品树 (Loot Tree)

**文件**: `app/core/loot_manager.py`
**要求**: 实现 N-ary Tree 管理所有物品分类。

```python
class LootNode:
    def __init__(self, name: str, is_category: bool = True):
        self.name = name
        self.is_category = is_category
        self.children: List['LootNode'] = []
        self.items: List[dict] = [] # 如果是叶子节点，存储具体Item数据

    def add_child(self, node: 'LootNode'):
        self.children.append(node)

    def to_dict(self):
        # 序列化为嵌套字典供前端 TreeView 使用
        return {
            "name": self.name,
            "children": [c.to_dict() for c in self.children],
            "items": self.items
        }

class LootManager:
    def __init__(self):
        self.root = LootNode("Collection")
        self._init_tree_structure()
    
    def _init_tree_structure(self):
        # 构建分类树
        equip = LootNode("Equipment")
        self.root.add_child(equip)
        
        # 细分装备
        equip.add_child(LootNode("Weapons"))
        equip.add_child(LootNode("Armor")) # 包含头盔/护甲/靴子
        # ... 其他分类
        
    def add_item_to_collection(self, item: dict):
        # 逻辑：根据 item.type 遍历树找到对应节点并插入
        pass

```

### 3.2. 地图生成 (Backbone + Branch)

**文件**: `app/core/map_gen.py`
**要求**: 图结构，主干+支线，难度控制。

```python
import random
from app.models import Room, RoomType, Monster, Item, ItemType

class MapGenerator:
    def generate(self, difficulty_factor: float) -> (dict, list):
        rooms = {}
        edges = []
        
        # 1. 生成主干 (Backbone)
        length = random.randint(6, 10)
        backbone_ids = []
        for i in range(length):
            rid = i
            r_type = RoomType.START if i == 0 else (RoomType.BOSS if i == length-1 else RoomType.NORMAL)
            rooms[rid] = Room(id=rid, type=r_type, neighbors=[])
            backbone_ids.append(rid)
            
            if i > 0:
                self._connect(rooms, i-1, i, edges)

        # 2. 生成支线 (Branches)
        next_id = length
        for b_id in backbone_ids[1:-1]: # 起点终点不挂支线
            if random.random() < 0.6:   # 60% 概率有支线
                branch_len = random.randint(1, 3)
                curr = b_id
                for _ in range(branch_len):
                    new_room = Room(id=next_id, type=RoomType.NORMAL, neighbors=[])
                    rooms[next_id] = new_room
                    self._connect(rooms, curr, next_id, edges)
                    curr = next_id
                    next_id += 1

        # 3. 添加回环 (Loops) - 增加连通度
        # 随机选取两个距离较远的节点连接
        
        # 4. 填充内容 (Populate)
        for r in rooms.values():
            if r.type == RoomType.NORMAL:
                dist = self._bfs_dist(rooms, 0, r.id)
                self._populate_room(r, dist, difficulty_factor)
            elif r.type == RoomType.BOSS:
                # 生成超级强力的 Boss
                pass

        return rooms, edges

    def _connect(self, rooms, u, v, edges):
        rooms[u].neighbors.append(v)
        rooms[v].neighbors.append(u)
        edges.append([u, v])
        
    def _populate_room(self, room, dist, factor):
        # 根据数值文档的公式生成怪物和掉落
        # Monster Power = 10 * factor + dist * 2 + random...
        pass

```

### 3.3. 寻路与排序 (Pathfinding)

**文件**: `app/core/pathfinder.py`
**要求**: Dijkstra 和 Top-K 价值排序。

```python
import heapq

class PathFinder:
    def get_shortest_path(self, rooms: dict, start_id: int, target_id: int):
        # Dijkstra 算法
        queue = [(0, start_id, [start_id])] # cost, current, path
        visited = set()
        
        while queue:
            cost, u, path = heapq.heappop(queue)
            if u == target_id:
                return path
            if u in visited:
                continue
            visited.add(u)
            
            for v in rooms[u].neighbors:
                if v not in visited:
                    # 可以在此添加权重逻辑，例如经过有怪物的房间权重更高
                    heapq.heappush(queue, (cost + 1, v, path + [v]))
        return []

    def get_top_k_value_paths(self, rooms: dict, start_id: int, target_id: int, k=3):
        # DFS 搜索所有路径 (带深度限制防止无限循环)
        all_paths = []
        
        def dfs(curr, path, visited):
            if curr == target_id:
                val = self._calculate_path_value(rooms, path)
                all_paths.append({"path": path[:], "value": val})
                return
            
            if len(path) > 15: # 深度限制
                return

            for n in rooms[curr].neighbors:
                if n not in visited:
                    visited.add(n)
                    path.append(n)
                    dfs(n, path, visited)
                    path.pop()
                    visited.remove(n)

        dfs(start_id, [start_id], {start_id})
        
        # 排序取 Top K
        return sorted(all_paths, key=lambda x: x['value'], reverse=True)[:k]

    def _calculate_path_value(self, rooms, path):
        # 价值 = 路径上(宝箱价值 - 怪物预计损耗)
        value = 0
        for rid in path:
            r = rooms[rid]
            if r.loot: value += r.loot.value
            if r.monster and r.monster.alive: value -= (r.monster.power * 0.5) # 估算
        return value

```

---

## 4. API 路由实现 (Routes)

**文件**: `app/main.py`

```python
from fastapi import FastAPI, HTTPException
from app.models import *
from app.core.game_engine import GameEngine
from app.core.pathfinder import PathFinder

app = FastAPI()
engine = GameEngine() # 单例模式，管理全局状态

@app.post("/init", response_model=GameResponse)
def init_game(difficulty: str):
    """根据难度初始化新地图"""
    factor = 1.0 if difficulty == "easy" else (1.5 if difficulty == "normal" else 2.0)
    state = engine.new_game(factor)
    return state

@app.post("/move", response_model=GameResponse)
def move_player(target_room_id: int):
    """
    核心交互：
    1. 检查是否邻接
    2. 处理怪物战斗 (扣HP)
    3. 拾取掉落 (更新背包)
    4. 更新 Boss 位置
    5. 更新迷雾
    """
    if not engine.is_valid_move(target_room_id):
        raise HTTPException(status_code=400, detail="Invalid move")
    
    result_state = engine.process_turn(target_room_id)
    return result_state

@app.post("/equip")
def equip_item(item_id: int):
    """穿戴装备，重新计算 Total Power"""
    engine.player_equip(item_id)
    return engine.get_current_state()

@app.get("/guide/shortest")
def get_shortest_guide():
    pf = PathFinder()
    rooms = engine.state.map_structure.rooms
    boss_id = engine.find_boss_room_id()
    path = pf.get_shortest_path(rooms, engine.state.player.current_room_id, boss_id)
    return {"path": path}

@app.get("/guide/best_loot")
def get_loot_guide():
    pf = PathFinder()
    rooms = engine.state.map_structure.rooms
    boss_id = engine.find_boss_room_id()
    paths = pf.get_top_k_value_paths(rooms, engine.state.player.current_room_id, boss_id)
    return {"paths": paths}

```

---

## 5. 持久化工具 (Persistence)

**文件**: `app/utils/persistence.py`

```python
import json
import os

SAVE_FILE = "data/dungeon_state.json"

def save_game_state(state_dict: dict):
    with open(SAVE_FILE, 'w', encoding='utf-8') as f:
        json.dump(state_dict, f, indent=2, default=str)

def load_game_state():
    if not os.path.exists(SAVE_FILE):
        return None
    with open(SAVE_FILE, 'r', encoding='utf-8') as f:
        return json.load(f)

```

---

## 6. 开发与调试建议

1. **优先实现 `MapGenerator**`: 只有能生成合法的 Graph，后续的移动和寻路才有意义。写一个简单的 `print` 脚本在控制台打印邻接表来验证连通性。
2. **测试数值平衡**: 在 `GameEngine` 中添加日志，打印 `Player Power` vs `Monster Power`。确保 Easy 模式下不会第一步就遇到打不过的怪。
3. **API 调试**: 使用 FastAPI 自带的 `/docs` (Swagger UI) 界面进行测试，无需写前端代码即可模拟移动和战斗。
4. **树的序列化**: 这是一个难点。确保 `LootManager` 能把 Python 对象正确转成 JSON 格式发给前端。

这份参考文件涵盖了 Project 2 所有的核心技术点，以此为基础开发可以确保高分通过。