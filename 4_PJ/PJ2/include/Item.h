#pragma once
#include <string>

enum class ItemType { Helmet, Armor, Boots, Weapon, MagicStone };

struct Item {
    std::string name;
    ItemType type;
    int value = 0;        // 价值，用于路径收益统计
    int power_bonus = 0;  // 武力加成，影响玩家战斗能力
};

// Helpers for (de)serialization
inline int item_type_to_int(ItemType t) {
    switch (t) {
        case ItemType::Helmet: return 0;
        case ItemType::Armor: return 1;
        case ItemType::Boots: return 2;
        case ItemType::Weapon: return 3;
        case ItemType::MagicStone: return 4;
    }
    return 0;
}

inline ItemType item_type_from_int(int x) {
    switch (x) {
        case 0: return ItemType::Helmet;
        case 1: return ItemType::Armor;
        case 2: return ItemType::Boots;
        case 3: return ItemType::Weapon;
        default: return ItemType::MagicStone;
    }
}
