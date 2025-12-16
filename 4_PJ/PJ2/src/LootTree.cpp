#include "../include/LootTree.h"

LootTree::LootTree() {
    root_ = std::make_unique<Node>(NodeType::Root, "Backpack");
    auto add_cat = [&](const std::string& name)->Node*{
        root_->children.emplace_back(std::make_unique<Node>(NodeType::Category, name));
        return root_->children.back().get();
    };
    cat_weapons_ = add_cat("Weapons");
    cat_armors_  = add_cat("Armors");
    cat_boots_   = add_cat("Boots");
    cat_helmets_ = add_cat("Helmets");
    cat_stones_  = add_cat("MagicStones");
}

LootTree::Node* LootTree::category_for(ItemType type) {
    switch (type) {
        case ItemType::Weapon: return cat_weapons_;
        case ItemType::Armor: return cat_armors_;
        case ItemType::Boots: return cat_boots_;
        case ItemType::Helmet: return cat_helmets_;
        case ItemType::MagicStone: return cat_stones_;
    }
    return cat_weapons_;
}

int LootTree::add_item(const Item& item, bool favorite) {
    Node* cat = category_for(item.type);
    auto node = std::make_unique<Node>(NodeType::ItemNode, item.name);
    node->item = item;
    node->favorite = favorite;
    node->item_id = next_item_id_++;
    int id = node->item_id;
    cat->children.emplace_back(std::move(node));
    return id;
}

bool LootTree::mark_favorite(int item_id, bool fav) {
    // 简单 DFS 搜索
    std::vector<const Node*> stack;
    std::vector<Node*> stack_mut;
    stack.reserve(64); stack_mut.reserve(64);
    stack.push_back(root_.get()); stack_mut.push_back(root_.get());
    while (!stack.empty()) {
        const Node* c = stack.back(); stack.pop_back();
        Node* m = stack_mut.back(); stack_mut.pop_back();
        if (m->type == NodeType::ItemNode && m->item_id == item_id) { m->favorite = fav; return true; }
        for (size_t i=0;i<m->children.size();++i) {
            stack.push_back(m->children[i].get());
            stack_mut.push_back(m->children[i].get());
        }
    }
    return false;
}

void LootTree::print_rec(const Node* node, std::ostream& os, int depth, bool only_favorites) const {
    auto indent = [&](int d){ for (int i=0;i<d;++i) os << "  "; };
    if (node->type == NodeType::ItemNode) {
        if (only_favorites && !node->favorite) return;
        indent(depth);
        os << "- " << node->label << " (id=" << node->item_id << ", val=" << node->item.value
           << ", pow+=" << node->item.power_bonus << (node->favorite?", *fav":"") << ")\n";
        return;
    }
    indent(depth);
    os << (node->type==NodeType::Root?"[Backpack]":"["+node->label+"]") << "\n";
    for (const auto& ch : node->children) print_rec(ch.get(), os, depth+1, only_favorites);
}

void LootTree::print(std::ostream& os, bool only_favorites) const {
    print_rec(root_.get(), os, 0, only_favorites);
}

std::pair<int,int> LootTree::totals() const {
    int tv = 0, tp = 0;
    std::vector<const Node*> st; st.push_back(root_.get());
    while (!st.empty()) {
        const Node* n = st.back(); st.pop_back();
        if (n->type == NodeType::ItemNode) { tv += n->item.value; tp += n->item.power_bonus; }
        for (const auto& ch : n->children) st.push_back(ch.get());
    }
    return {tv, tp};
}
