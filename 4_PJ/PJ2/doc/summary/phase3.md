# PJ2 地下城与勇士 - Phase 3 总结（路径搜索与排序）

本阶段实现了路径搜索算法：基于 BFS 的最少探索次数路径、基于 DFS 的全路径枚举与按战利品价值排序，并提供可选的“玩家武力约束”可行性过滤。

## 模块与接口

- PathFinder（`include/PathFinder.h` + `src/PathFinder.cpp`）
  - `find_shortest_path(map, start, goal)`：BFS，返回最少房间数路径（不可达返回空）。
  - `find_all_paths_sorted(map, start, goal, max_paths=1000, initial_power=-1)`：DFS 枚举无环路径，计算每条路径的战利品总价值/武力加成，按价值降序排序；当 `initial_power>=0` 时仅返回可行路径（沿途玩家武力需严格大于房间怪物武力）。
  - `evaluate_path(map, path, initial_power)`：计算单条路径的价值、加成与可行性。
- 辅助结构
  - `PathInfo`：包含 `path`、`total_value`、`total_power_bonus`、`feasible`。

## 设计要点

- BFS 最短路径：维护 `parent` 回溯，首次到达目标即收束以保证最少跳数。
- DFS 全路径：使用 `visited` 防止环，提供 `max_paths` 上限避免组合爆炸；排序规则为“价值降序，若相同则较短优先”。
- 价值计算：对路径中的每个房间累加其 `chest_items` 的 `value` 与 `power_bonus`；不修改原地图数据。
- 可行性（可选）：当给出 `initial_power` 时，检查沿途 `current_power > monster_power`，并在路径行进中累加 `power_bonus`（模拟获得加成后更强）。

## CLI 演示（`src/main.cpp`）

- 生成地图后，从最小房间 ID 到 Boss：
  - 打印“Shortest Path”。
  - 列出“Top Value Paths (top 3)”及其 `val/pow+` 与具体路径。

## 测试

- `tests/test_phase3.cpp`
  - `BfsShortest`：在手工小图中验证最短路径为 0-1-4。
  - `AllPathsSortedByValue`：验证价值排序的 Top1 覆盖高价值节点。
  - `FeasibilityWithPower`：当 `initial_power=10` 且某节点怪物过强时，含该节点的路径被过滤。
- 已集成至 `scripts/test_sample.sh` 与 `make test`；生成二进制 `bin/test_phase3`。

## 下一步（进入 Phase 4）

- 将 PathFinder 与 GameSystem 集成，支持命令式选择起点/终点，实时展示最短路径与价值最高路径。
- 可选：在 Boss 移动后自动重算最短路径；在战斗模拟中综合胜负逻辑。
