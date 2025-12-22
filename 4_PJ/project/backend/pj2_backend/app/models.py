"""Core Pydantic models for backend API (Phase 1).

根据 backend_ref.md 设计：
* Item / ItemType / Room / MapStructure
* PlayerState / EquipmentSlots / GameResponse
* 保留 Phase0 的 HealthResponse 健康检查。
"""
from enum import Enum
from typing import Dict, List, Optional

from pydantic import BaseModel, Field


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
    SAFE = "safe"  # 泉水/营地


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
    neighbors: List[int] = Field(default_factory=list)  # 邻接表
    monster: Optional[Monster] = None
    loot: Optional[Item] = None
    visited: bool = False
    visible: bool = False  # 迷雾状态


class MapStructure(BaseModel):
    rooms: Dict[int, Room]
    edges: List[List[int]] = Field(default_factory=list)


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
    total_power: int
    equipment: EquipmentSlots
    inventory: List[Item] = Field(default_factory=list)  # 背包（扁平化）


class GameResponse(BaseModel):
    player: PlayerState
    map_view: MapStructure
    logs: List[str]
    status: str  # "playing", "win", "dead"


class HealthResponse(BaseModel):
    status: str = "ok"
    message: str = "backend scaffold ready"
