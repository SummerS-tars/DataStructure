# PJ2 地下城与勇士 - Phase 4 总结（系统整合与扩展）

本阶段将地图、路径、玩家与日志整合到 CLI 演示中，并实现了 Boss 移动后的动态路径重算。战斗模拟可作为后续可选扩展。

## 新增模块

- BossManager（`include/BossManager.h`）
  - `move_boss_once(map, rng)`：随机选择当前 Boss 的一个邻接房间，将 Boss 移动到该房间；同时维持“Boss 怪力为全图最大”的不变量（旧房降级为普通怪，新房怪力提升）。

## CLI 集成（`src/main.cpp`）

- 在 Phase 2/3 的基础上增加：
  - 打印初始最短路径与高价值路径 Top3（从最小房间 ID 到 Boss）。
  - 调用 `BossManager::move_boss_once` 移动 Boss 一步后，重新计算并展示新的最短路径。

## 测试

- `tests/test_phase4.cpp`
  - `BossMoveNeighbor`：移动后 Boss 新位置必须是原位置的邻居，且 Boss 标记唯一正确。
  - `ReplanAfterMove`：移动后以相同起点对新 Boss 重新规划最短路径（至少可求得）。
- 已集成至 `scripts/test_sample.sh` 与 `make test`；二进制 `bin/test_phase4`。

## 备注与扩展建议

- 战斗模拟：可在 `Player::explore_path` 前插入对每一房的战斗判定（随机或数值对比），失败则中断并记录日志。
- 交互菜单：可继续抽象出 `GameSystem` 提供菜单（生成地图/查看/寻路/收藏/日志/Boss步进等）。
- 重现实验：结合 Phase 1 的 `save/load`，可先加载固定地图再演示 Boss 移动与路径重算。
