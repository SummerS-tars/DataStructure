#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

#include "../include/DungeonMap.h"
#include "../include/MapGenerator.h"
#include "../include/PathFinder.h"
#include "../include/Player.h"
#include "../include/BossManager.h"
#include "../include/GridMapGenerator.h"

static void print_usage() {
    std::cerr << "Usage:\n"
              << "  engine generate <seed> <room_num> <extra_edges> <out_map_path>\n"
              << "  engine generate_grid <width> <height> <seed> <out_map_path>\n"
              << "  engine shortest <map_path> <start> <goal>\n"
              << "  engine top <map_path> <start> <goal> <k> [initial_power]\n"
              << "  engine explore_shortest <map_path> <start> <goal> <initial_power>\n"
              << "  engine neighbors <map_path> <room_id>\n"
              << "  engine step_move <map_path> <out_map_path> <player_pos> <next_room> <power>\n"
              << "  engine bossmove <map_path> <out_map_path>\n";
}

static bool save_stdout_error(const std::string& msg) {
    std::cerr << msg << std::endl;
    return false;
}

static bool cmd_generate(int argc, char** argv) {
    if (argc < 6) return save_stdout_error("ERR missing args for generate");
    unsigned int seed = (unsigned int)std::stoul(argv[2]);
    int room_num = std::stoi(argv[3]);
    int extra_edges = std::stoi(argv[4]);
    std::string out_path = argv[5];

    GenConfig cfg; cfg.seed = seed; cfg.room_num = room_num; cfg.extra_edges = extra_edges;
    auto map = MapGenerator::generate(cfg);
    if (!map.save(out_path)) return save_stdout_error("ERR save failed");
    std::cout << map.to_json() << std::endl;
    return true;
}

static bool cmd_generate_grid(int argc, char** argv) {
    if (argc < 6) return save_stdout_error("ERR missing args for generate_grid");
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);
    unsigned int seed = (unsigned int)std::stoul(argv[4]);
    std::string out_path = argv[5];
    GridConfig cfg; cfg.width = width; cfg.height = height; cfg.seed = seed;
    auto map = GridMapGenerator::generate(cfg);
    if (!map.save(out_path)) return save_stdout_error("ERR save failed");
    std::cout << "OK " << out_path << std::endl;
    return true;
}

static bool cmd_shortest(int argc, char** argv) {
    if (argc < 5) return save_stdout_error("ERR missing args for shortest");
    std::string map_path = argv[2];
    int start = std::stoi(argv[3]);
    int goal = std::stoi(argv[4]);
    DungeonMap map = DungeonMap::load(map_path);
    auto path = PathFinder::find_shortest_path(map, start, goal);
    std::cout << "PATH";
    for (int id : path) std::cout << ' ' << id;
    std::cout << std::endl;
    return true;
}

static bool cmd_top(int argc, char** argv) {
    if (argc < 6) return save_stdout_error("ERR missing args for top");
    std::string map_path = argv[2];
    int start = std::stoi(argv[3]);
    int goal = std::stoi(argv[4]);
    int k = std::stoi(argv[5]);
    int initial_power = -1;
    if (argc >= 7) initial_power = std::stoi(argv[6]);
    DungeonMap map = DungeonMap::load(map_path);
    auto paths = PathFinder::find_all_paths_sorted(map, start, goal, 1000, initial_power);
    int rank = 1;
    for (const auto& p : paths) {
        if (rank > k) break;
        std::cout << "PATH " << rank << " val=" << p.total_value << " pow=" << p.total_power_bonus << " :";
        for (int id : p.path) std::cout << ' ' << id;
        std::cout << '\n';
        ++rank;
    }
    return true;
}

static bool cmd_explore_shortest(int argc, char** argv) {
    if (argc < 6) return save_stdout_error("ERR missing args for explore_shortest");
    std::string map_path = argv[2];
    int start = std::stoi(argv[3]);
    int goal = std::stoi(argv[4]);
    int initial_power = std::stoi(argv[5]);
    DungeonMap map = DungeonMap::load(map_path);
    auto path = PathFinder::find_shortest_path(map, start, goal);
    Player player(initial_power);
    bool success = !path.empty();
    std::vector<int> progressed;
    if (success) {
        for (int id : path) {
            player.collect_room_loot(map, id);
            progressed.push_back(id);
        }
        player.explore_path(map, progressed);
    }
    std::cout << "EXPLORE success=" << (success?1:0)
              << " power_end=" << player.power()
              << " total_value=" << player.total_value()
              << " progressed=";
    for (size_t i=0;i<progressed.size();++i) {
        if (i) std::cout << ',';
        std::cout << progressed[i];
    }
    std::cout << std::endl;
    return true;
}

static bool cmd_neighbors(int argc, char** argv) {
    if (argc < 4) return save_stdout_error("ERR missing args for neighbors");
    std::string map_path = argv[2];
    int room_id = std::stoi(argv[3]);
    DungeonMap map = DungeonMap::load(map_path);
    auto nbrs = map.neighbors(room_id);
    std::cout << "NEIGH";
    for (int id : nbrs) std::cout << ' ' << id;
    std::cout << std::endl;
    return true;
}

static bool battle_win(const Room* r, int cur_power, std::mt19937& rng) {
    if (!r || r->monster_power <= 0) return true;
    std::uniform_int_distribution<int> bonus(0,5);
    int roll = cur_power + bonus(rng);
    return roll > r->monster_power;
}

static bool cmd_step_move(int argc, char** argv) {
    if (argc < 7) return save_stdout_error("ERR missing args for step_move");
    std::string map_path = argv[2];
    std::string out_path = argv[3];
    int player_pos = std::stoi(argv[4]);
    int next_room = std::stoi(argv[5]);
    int power = std::stoi(argv[6]);
    DungeonMap map = DungeonMap::load(map_path);
    // verify adjacency
    auto nbrs = map.neighbors(player_pos);
    bool adjacent = std::find(nbrs.begin(), nbrs.end(), next_room) != nbrs.end();
    std::mt19937 rng{std::random_device{}()};
    bool moved = false;
    int total_value = 0;
    bool over = false;
    if (adjacent) {
        const Room* r = map.get_room(next_room);
        if (battle_win(r, power, rng)) {
            // collect loot
            Player player(power);
            player.collect_room_loot(map, next_room);
            power = player.power();
            total_value = player.total_value();
            moved = true;
            if (r && r->is_boss) over = true;
            player_pos = next_room;
        }
    }
    // persist map changes
    if (!map.save(out_path)) return save_stdout_error("ERR save failed");
    std::cout << "STATE pos=" << player_pos
              << " power_end=" << power
              << " total_value=" << total_value
              << " moved=" << (moved?1:0)
              << " over=" << (over?1:0)
              << " map=" << out_path << std::endl;
    return true;
}

static bool cmd_bossmove(int argc, char** argv) {
    if (argc < 4) return save_stdout_error("ERR missing args for bossmove");
    std::string map_path = argv[2];
    std::string out_path = argv[3];
    DungeonMap map = DungeonMap::load(map_path);
    std::mt19937 rng{std::random_device{}()};
    int boss = BossManager::move_boss_once(map, rng);
    if (!map.save(out_path)) return save_stdout_error("ERR save failed");
    std::cout << "BOSS " << boss << ' ' << out_path << std::endl;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) { print_usage(); return 2; }
    std::string cmd = argv[1];
    bool ok = false;
    if (cmd == "generate") ok = cmd_generate(argc, argv);
    else if (cmd == "generate_grid") ok = cmd_generate_grid(argc, argv);
    else if (cmd == "shortest") ok = cmd_shortest(argc, argv);
    else if (cmd == "top") ok = cmd_top(argc, argv);
    else if (cmd == "explore_shortest") ok = cmd_explore_shortest(argc, argv);
    else if (cmd == "neighbors") ok = cmd_neighbors(argc, argv);
    else if (cmd == "step_move") ok = cmd_step_move(argc, argv);
    else if (cmd == "bossmove") ok = cmd_bossmove(argc, argv);
    else { print_usage(); return 2; }
    return ok ? 0 : 1;
}
