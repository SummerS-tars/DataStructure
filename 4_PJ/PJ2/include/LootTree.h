#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Item.h"

class LootTree {
public:
    enum class NodeType { Root, Category, ItemNode };

    struct Node {
        NodeType type;
        std::string label; // Root/Category 名称或 Item 名称
        Item item;         // 仅当 ItemNode 有效
        bool favorite = false; // 收藏标记，仅 ItemNode 使用
        int item_id = -1;      // 自增 ID，仅 ItemNode 使用
        std::vector<std::unique_ptr<Node>> children;

        Node(NodeType t, std::string lbl) : type(t), label(std::move(lbl)) {}
    };

    LootTree();

    // 插入新物品，返回分配的 item_id
    int add_item(const Item& item, bool favorite = false);

    // 将某个物品标记为收藏
    bool mark_favorite(int item_id, bool fav = true);

    // 打印：全部或仅收藏
    void print(std::ostream& os, bool only_favorites = false) const;

    // 统计当前持有物品总价值与总武力加成
    std::pair<int,int> totals() const; // {total_value, total_power_bonus}

private:
    std::unique_ptr<Node> root_;
    Node* cat_weapons_ = nullptr;
    Node* cat_armors_ = nullptr;
    Node* cat_boots_ = nullptr;
    Node* cat_helmets_ = nullptr;
    Node* cat_stones_ = nullptr;
    int next_item_id_ = 1;

    Node* category_for(ItemType type);
    void print_rec(const Node* node, std::ostream& os, int depth, bool only_favorites) const;
};
