#include "../include/GameSystem.h"
#include <iostream>
#include <limits>

void GameSystem::menu_print() const {
    std::cout << "\n=== Dungeon & Hero ===\n"
              << "1. 生成新地图\n"
              << "2. 查看地图\n"
              << "3. 最短路径(BFS)\n"
              << "4. 最高价值路径(Top3)\n"
              << "5. 探索并战斗(路径: 起点->Boss)\n"
              << "6. 查看背包\n"
              << "7. 查看收藏\n"
              << "8. 切换收藏(按物品id)\n"
              << "9. 查看探险日志\n"
              << "10. Boss移动并重算最短路径\n"
              << "11. 保存/加载地图\n"
              << "0. 退出\n";
}

void GameSystem::action_generate_map() {
    GenConfig cfg; cfg.room_num = 20; cfg.extra_edges = 20; cfg.seed = std::random_device{}();
    map_ = MapGenerator::generate(cfg);
    player_ = Player(10);
    std::cout << "[OK] 新地图生成完成 (rooms=" << map_.room_count() << ")\n";
}

void GameSystem::action_view_map() const {
    std::cout << map_.to_string() << "\n";
    std::cout << (map_.is_connected()?"Connected":"NotConnected") << "\n";
}

void GameSystem::action_shortest_path() const {
    auto ids = map_.room_ids(); if (ids.empty() || map_.boss_room_id()==-1) { std::cout << "[WARN] 地图未就绪\n"; return; }
    int start = ids.front(); int goal = map_.boss_room_id();
    auto sp = PathFinder::find_shortest_path(map_, start, goal);
    if (sp.empty()) { std::cout << "[FAIL] 不可达\n"; return; }
    std::cout << "Shortest: "; for(size_t i=0;i<sp.size();++i){ if(i) std::cout<<"->"; std::cout<<sp[i]; } std::cout<<"\n";
}

void GameSystem::action_top_value_paths() const {
    auto ids = map_.room_ids(); if (ids.empty() || map_.boss_room_id()==-1) { std::cout << "[WARN] 地图未就绪\n"; return; }
    int start = ids.front(); int goal = map_.boss_room_id();
    auto paths = PathFinder::find_all_paths_sorted(map_, start, goal, 200);
    size_t show = std::min<size_t>(3, paths.size());
    for (size_t i=0;i<show;++i){ const auto& p=paths[i]; std::cout << "#"<<i+1<<" val="<<p.total_value<<" pow+="<<p.total_power_bonus<<" ";
        for(size_t j=0;j<p.path.size();++j){ if(j) std::cout<<"->"; std::cout<<p.path[j]; } std::cout<<"\n"; }
}

bool GameSystem::battle_room(const Room& r, int& cur_power) {
    if (r.monster_power<=0) return true;
    // 简易战斗：cur_power + random(0..5) 与 r.monster_power 比较
    std::uniform_int_distribution<int> bonus(0,5);
    int roll = cur_power + bonus(rng_);
    bool win = roll > r.monster_power;
    std::cout << "战斗: 我方="<<roll<<" 敌方="<<r.monster_power<<" => "<<(win?"胜":"负")<<"\n";
    return win;
}

void GameSystem::action_explore_with_battle() {
    auto ids = map_.room_ids(); if (ids.empty() || map_.boss_room_id()==-1) { std::cout << "[WARN] 地图未就绪\n"; return; }
    int start = ids.front(); int goal = map_.boss_room_id();
    auto sp = PathFinder::find_shortest_path(map_, start, goal);
    if (sp.empty()) { std::cout << "[FAIL] 不可达\n"; return; }
    std::cout << "探索路径: "; for(size_t i=0;i<sp.size();++i){ if(i) std::cout<<"->"; std::cout<<sp[i]; } std::cout<<"\n";
    // 战斗+收集
    int cur_power = player_.power(); std::vector<int> progressed;
    for (int id : sp) {
        const Room* r = map_.get_room(id);
        if (!r) continue;
        if (!battle_room(*r, cur_power)) { std::cout << "[STOP] 战斗失败，中断探索。\n"; break; }
        // 胜利后收集
        player_.collect_room_loot(map_, id);
        progressed.push_back(id);
        cur_power = player_.power();
    }
    if (!progressed.empty()) player_.explore_path(map_, progressed);
    std::cout << "当前武力="<<player_.power()<<" 总价值="<<player_.total_value()<<"\n";
}

void GameSystem::action_view_loot(bool only_fav) const {
    player_.loot().print(std::cout, only_fav);
    auto [tv,tp]=player_.loot().totals();
    std::cout << "Totals: value="<<tv<<" power_bonus="<<tp<<"\n";
}

void GameSystem::action_toggle_favorite() {
    std::cout << "输入物品id以切换收藏: "; int id=0; if(!(std::cin>>id)){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); return; }
    // 简单尝试设置收藏（LootTree 提供 mark_favorite）
    if (player_.loot_mut().mark_favorite(id,true)) std::cout << "[OK] 已收藏\n"; else std::cout << "[WARN] 未找到该物品\n";
}

void GameSystem::action_view_logs() const {
    player_.log().print(std::cout);
}

void GameSystem::action_boss_move_and_replan() {
    int cur = map_.boss_room_id(); if (cur==-1){ std::cout << "[WARN] Boss 未就绪\n"; return; }
    int nxt = BossManager::move_boss_once(map_, rng_);
    if (nxt==-1){ std::cout << "[WARN] Boss 移动失败\n"; return; }
    std::cout << "Boss 从 "<<cur<<" 移动到 "<<nxt<<"\n";
    action_shortest_path();
}

void GameSystem::action_save_load() {
    std::cout << "1) 保存  2) 加载 : "; int op=0; if(!(std::cin>>op)){ std::cin.clear(); return; }
    if (op==1){ std::string p="build/map_save.txt"; if(map_.save(p)) std::cout<<"[OK] Saved to "<<p<<"\n"; else std::cout<<"[FAIL] 保存失败\n"; }
    else if (op==2){ std::string p="build/map_save.txt"; map_ = DungeonMap::load(p); std::cout<<"[OK] Loaded. rooms="<<map_.room_count()<<"\n"; }
}

void GameSystem::run() {
    // 启动时自动生成并展示地图，兼容现有测试对输出的检查
    action_generate_map();
    action_view_map();
    bool running = true;
    while (running) {
        menu_print();
        std::cout << "> 选择: "; int choice=0; if(!(std::cin>>choice)){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); continue; }
        switch (choice) {
            case 1: action_generate_map(); break;
            case 2: action_view_map(); break;
            case 3: action_shortest_path(); break;
            case 4: action_top_value_paths(); break;
            case 5: action_explore_with_battle(); break;
            case 6: action_view_loot(false); break;
            case 7: action_view_loot(true); break;
            case 8: action_toggle_favorite(); break;
            case 9: action_view_logs(); break;
            case 10: action_boss_move_and_replan(); break;
            case 11: action_save_load(); break;
            case 0: running=false; break;
            default: std::cout << "[WARN] 无效选择\n"; break;
        }
    }
}
