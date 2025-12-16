#pragma once
#include <random>
#include "DungeonMap.h"
#include "MapGenerator.h"
#include "Player.h"
#include "PathFinder.h"
#include "BossManager.h"

class GameSystem {
public:
    GameSystem() : rng_(std::random_device{}()) {}

    void run();

private:
    DungeonMap map_;
    Player player_{10};
    std::mt19937 rng_;

    void menu_print() const;
    void action_generate_map();
    void action_view_map() const;
    void action_shortest_path() const;
    void action_top_value_paths() const;
    void action_explore_with_battle();
    void action_view_loot(bool only_fav) const;
    void action_toggle_favorite();
    void action_view_logs() const;
    void action_boss_move_and_replan();
    void action_save_load();

    // battle
    bool battle_room(const Room& r, int& cur_power);
};
