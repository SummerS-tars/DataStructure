#include <cassert>
#include <sstream>
#include <string>
#include "../include/LootTree.h"
#include "../include/AdventureLog.h"
#include "../include/Item.h"

static void test_loot_insert_and_display() {
    LootTree lt;
    Item sword{ "Excalibur", ItemType::Weapon, 100, 20 };
    int id = lt.add_item(sword);
    assert(id > 0);
    auto totals = lt.totals();
    assert(totals.first >= 100);
    assert(totals.second >= 20);
    std::ostringstream os;
    lt.print(os, false);
    std::string out = os.str();
    assert(out.find("Weapons") != std::string::npos);
    assert(out.find("Excalibur") != std::string::npos);
}

static void test_favorites() {
    LootTree lt;
    int id1 = lt.add_item(Item{ "A", ItemType::Armor, 10, 1 });
    int id2 = lt.add_item(Item{ "B", ItemType::Boots, 5, 2 });
    assert(lt.mark_favorite(id2, true));
    std::ostringstream favout;
    lt.print(favout, true);
    std::string s = favout.str();
    // 仅收藏视图应包含 "- B (" 不包含 "- A ("
    assert(s.find("- B (") != std::string::npos);
    assert(s.find("- A (") == std::string::npos);
}

static void test_adventure_log_basic() {
    AdventureLog log;
    LogEntry e; e.timestamp = "t"; e.start_room = 0; e.end_room = 2; e.path = {0,1,2}; e.final_power = 42; e.total_value = 30;
    log.append(e);
    log.append(e);
    assert(log.entries().size() == 2);
    assert(log.entries()[0].id == 1);
    assert(log.entries()[1].id == 2);
}

int main() {
    test_loot_insert_and_display();
    test_favorites();
    test_adventure_log_basic();
    return 0;
}
