from typing import List, Optional, Dict, Any

from app.models import Item, ItemType


class LootNode:
    """N-ary tree node for loot/collection management."""

    def __init__(self, name: str, is_category: bool = True):
        self.name = name
        self.is_category = is_category
        self.children: List["LootNode"] = []
        self.items: List[Dict[str, Any]] = []  # store plain dicts for easy JSON serialization

    def add_child(self, node: "LootNode") -> None:
        self.children.append(node)

    def add_item(self, item: Item) -> None:
        # store minimal fields for tree display
        self.items.append(item.dict())

    def to_dict(self) -> Dict[str, Any]:
        return {
            "name": self.name,
            "is_category": self.is_category,
            "children": [c.to_dict() for c in self.children],
            "items": self.items,
        }


class LootManager:
    """Manage predefined loot tree and insertion by item type."""

    def __init__(self):
        self.root = LootNode("Collection")
        self._seen_ids: set[int] = set()
        self._type_to_node: Dict[ItemType, LootNode] = {}
        self._init_tree_structure()

    def _init_tree_structure(self) -> None:
        equipment = LootNode("Equipment")
        weapons = LootNode("Weapons")
        defense = LootNode("Defense")
        helmets = LootNode("Helmets")
        armors = LootNode("Armors")
        boots = LootNode("Boots")
        magic = LootNode("Magic Stones")
        consumable = LootNode("Consumable")
        collectible = LootNode("Collectibles")

        # assemble tree
        self.root.add_child(equipment)
        equipment.add_child(weapons)
        equipment.add_child(defense)
        defense.add_child(helmets)
        defense.add_child(armors)
        defense.add_child(boots)
        self.root.add_child(magic)
        self.root.add_child(consumable)
        self.root.add_child(collectible)

        # type mapping for quick insertion
        self._type_to_node = {
            ItemType.WEAPON: weapons,
            ItemType.HELMET: helmets,
            ItemType.ARMOR: armors,
            ItemType.BOOTS: boots,
            ItemType.MAGIC_STONE: magic,
            ItemType.POTION: consumable,
            ItemType.COLLECTIBLE: collectible,
        }

        # seed with one sample item per category for Phase 1 demo
        sample_items = [
            Item(id=1, name="Iron Sword", type=ItemType.WEAPON, power_bonus=5, value=40),
            Item(id=2, name="Leather Cap", type=ItemType.HELMET, power_bonus=2, value=20),
            Item(id=3, name="Chainmail", type=ItemType.ARMOR, power_bonus=6, value=60),
            Item(id=4, name="Swift Boots", type=ItemType.BOOTS, power_bonus=3, value=35),
            Item(id=5, name="Fire Stone", type=ItemType.MAGIC_STONE, power_bonus=8, value=80),
            Item(id=6, name="Health Potion", type=ItemType.POTION, value=25, description="Restore 20 HP"),
            Item(id=7, name="Golden Statue", type=ItemType.COLLECTIBLE, value=200),
        ]
        for item in sample_items:
            self.add_item_to_collection(item)

    def add_item_to_collection(self, item: Item) -> None:
        if item.id in self._seen_ids:
            return
        target = self._type_to_node.get(item.type)
        if target is None:
            target = self.root
        target.add_item(item)
        self._seen_ids.add(item.id)

    def to_dict(self) -> Dict[str, Any]:
        return self.root.to_dict()

    def ingest_items(self, items: List[Item]) -> None:
        for item in items:
            self.add_item_to_collection(item)

    def to_flat_list(self) -> List[Dict[str, Any]]:
        """Flatten all stored items into a list of dicts for persistence."""
        collected: List[Dict[str, Any]] = []

        def collect(node: LootNode) -> None:
            collected.extend(node.items)
            for child in node.children:
                collect(child)

        collect(self.root)
        return collected
