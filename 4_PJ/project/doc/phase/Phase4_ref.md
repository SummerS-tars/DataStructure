# Phase 4: 高阶算法、持久化与优化 (Polish)

## 1. 目标概述
完成课程要求的高分点：最短路径算法、Top-K 价值路径算法，以及存读档功能。

## 2. 详细任务清单

### 2.1. 寻路算法 (Backend Algorithms)
* [ ] **编写 `pathfinder.py`**:
    * 实现 **Dijkstra**: 计算 Current Room 到 Boss Room 的最短路径.
    * 实现 **Top-K DFS**: 搜索到达 Boss 的路径中，净收益 (宝箱价值 - 战斗损耗) 最高的 Top 3 路径.
    * 暴露为 API: `/guide/shortest` 和 `/guide/best_loot`。

### 2.2. 战术终端 (Frontend Strategy)
* [ ] **开发 `StrategyTerminal.vue`**: 添加两个按钮。
* [ ] **高亮显示**:
    * 点击按钮获取路径 ID 列表。
    * 调用 `DungeonMap` 的高亮方法，将对应的 Edge 染成红色或金色.

### 2.3. 数据持久化 (Persistence)
* [ ] **存档/读档**:
    * 后端实现每次 Move 后自动写入 `dungeon_state.json`。
    * 前端页面加载时检查是否有存档，有则恢复，无则新开.
* [ ] **图鉴持久化**: 实现 `user_save.json` 记录玩家见过的所有 Item。

## 3. 成功验收标准 (Definition of Done)
1.  **算法演示**: 点击“最短路径”，地图上会出现一条红线直通 Boss。
2.  **数据保存**: 刷新网页 (F5)，玩家位置、血量、地图探索进度保持不变，不会重置回起点。
3.  **最终检查**: 对照 `project_detail.md` 中的 "数据结构评分点映射 (Checklist)"，确认图、树、排序、搜索均已实现.