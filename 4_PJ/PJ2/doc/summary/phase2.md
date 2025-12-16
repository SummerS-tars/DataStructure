# PJ2 地下城与勇士 - Phase 2 总结（战利品树与探险日志）

本阶段实现了战利品分类树（LootTree）与探险日志（AdventureLog），并在 CLI 中提供最小演示。

## 模块与数据结构

- LootTree（`include/LootTree.h` + `src/LootTree.cpp`）
  - 结构：多叉树（Root=Backpack；二级为固定分类：Weapons/Armors/Boots/Helmets/MagicStones；叶为具体物品）。
  - 能力：`add_item` 插入物品并归类；`mark_favorite` 收藏标记；`print(only_favorites)` 递归展示；`totals()` 汇总总价值与总武力加成。
- AdventureLog（`include/AdventureLog.h`）
  - 结构：线性表（`std::vector<LogEntry>`）。
  - 能力：`append` 自动分配自增 id；`print` 顺序输出摘要（时间、起终点、路径长度、最终武力、总价值）。
- Player（`include/Player.h`）
  - 状态：`power`（初始10）、`total_value`、`loot`、`log`。
  - 能力：`collect_room_loot(map, room_id)` 自动将房间宝箱物品加入背包并清空房间宝箱；`explore_path(map, path)` 按路径收集并记录日志（不含战斗判定，战斗留待扩展）。

## CLI 演示（`src/main.cpp`）

- 生成地图并打印连通性后，构造 `Player` 并用 `room_ids` 的前三个房间作为示例路径：
  - 自动收集沿途宝箱 → `LootTree.print()` 展示持有物品分类树与 `Totals` 汇总。
  - 展示收藏视图（默认无收藏）。
  - 打印 `Adventure Log` 列表（包含时间戳与路径摘要）。

## 测试

- 最小脚本：`scripts/test_sample.sh`（`make test` 会调用）
  - 校验地图输出关键标识（BossRoom、Connected）。
  - 运行 `bin/test_phase1` 与 `bin/test_phase2`。
- C++ 单测：
  - `tests/test_phase2.cpp`
    - `LootTreeInsertAndDisplay`：插入武器并检查展示与统计。
    - `Favorites`：标记收藏并检查仅收藏视图。
    - `AdventureLogBasic`：追加两条记录并检查 id 自增。

## 下一步（进入 Phase 3）

- PathFinder：BFS 求最少探索次数路径；DFS 枚举所有可行路径并按总价值排序。
- 将 `Player::explore_path` 与实际路径搜索集成，考虑怪物武力与玩家武力对可达性的影响。
