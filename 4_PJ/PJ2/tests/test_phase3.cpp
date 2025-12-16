#include <cassert>
#include <vector>
#include <iostream>
#include "../include/DungeonMap.h"
#include "../include/PathFinder.h"
#include "../include/Item.h"

static DungeonMap build_small_map() {
    DungeonMap m;
    // Rooms 0..4
    for (int i=0;i<5;++i) {
        Room r; r.id = i; r.monster_power = 0; m.add_room(r);
    }
    // Edges: 0-1-4, 0-2-3-4, 1-2 (triangle)
    m.add_edge(0,1); m.add_edge(1,4);
    m.add_edge(0,2); m.add_edge(2,3); m.add_edge(3,4);
    m.add_edge(1,2);
    // Loot values: path 0-2-3-4 richer
    m.get_room_mut(2)->chest_items.push_back(Item{"Gem", ItemType::MagicStone, 50, 0});
    m.get_room_mut(3)->chest_items.push_back(Item{"Sword", ItemType::Weapon, 30, 5});
    m.set_boss(4);
    return m;
}

static void test_bfs_shortest() {
    auto m = build_small_map();
    auto p = PathFinder::find_shortest_path(m, 0, 4);
    // shortest by hops is 0-1-4 (len 3)
    assert(p.size() == 3);
    assert(p[0]==0 && p[1]==1 && p[2]==4);
}

static void test_all_paths_sorted_by_value() {
    auto m = build_small_map();
    auto infos = PathFinder::find_all_paths_sorted(m, 0, 4, 100);
    assert(!infos.empty());
    // expect first path to include nodes 2 and 3 (richer)
    const auto& best = infos.front();
    bool has2=false, has3=false; for (int x: best.path){ if(x==2) has2=true; if(x==3) has3=true; }
    assert(has2 && has3);
}

static void test_feasibility_with_power() {
    // Construct with a strong monster on node 1
    DungeonMap m;
    for (int i=0;i<3;++i){ Room r; r.id=i; r.monster_power=0; m.add_room(r); }
    m.add_edge(0,1); m.add_edge(1,2); // path 0-1-2
    m.add_edge(0,2); // direct 0-2
    m.get_room_mut(1)->monster_power = 20; // too strong for initial 10
    // Put power bonus on node 0 so it doesn't help before node1
    m.get_room_mut(0)->chest_items.push_back(Item{"Boots", ItemType::Boots, 5, 1});
    m.set_boss(2);
    // Enumerate feasible with initial power=10
    auto paths = PathFinder::find_all_paths_sorted(m, 0, 2, 10, /*initial_power=*/10);
    // 0-1-2 should be infeasible; direct 0-2 should be feasible
    assert(!paths.empty());
    for (const auto& pi : paths) {
        for (size_t i=1;i<pi.path.size();++i){ assert(pi.path[i-1] != pi.path[i]); }
    }
    // ensure none contains node 1 since it's infeasible
    bool contains1=false; for (auto& pi: paths){ for(int v:pi.path){ if(v==1) contains1=true; }}
    assert(!contains1);
}

int main(){
    test_bfs_shortest();
    test_all_paths_sorted_by_value();
    test_feasibility_with_power();
    std::cout << "[TEST PASS] Phase3 tests passed.\n";
    return 0;
}
