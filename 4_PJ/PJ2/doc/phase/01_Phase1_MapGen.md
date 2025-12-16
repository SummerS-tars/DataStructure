# Phase 1: 地下城地图生成与图结构 (Map Generation & Graph Structure)

## 目标 (Goal)

构建地下城的核心数据结构（图），实现不少于20个房间的自动生成，确保图的连通性，并正确放置怪物（含Boss）和宝箱。

## 需要实现的模块 (Modules to Implement/Modify)

| 模块名称 | 职责与目标 | 关键数据结构/算法 |
| :--- | :--- | :--- |
| **`Item` / `Loot`** (Class) | 定义道具的基本属性（名称、类型、价值、武力加成）。 | 对象封装 |
| **`Room`** (Class) | 代表图中的节点。包含房间ID、怪物武力值（若无则为0）、宝箱对象。 | 节点 (Vertex) |
| **`DungeonMap`** (Class) | 核心地图类。管理所有 `Room` 及其连接关系。 | **图 (Graph) - 邻接表 (Adjacency List)** |
| **`MapGenerator`** (Function) | 随机生成 $\ge 20$ 个房间，随机添加边，并确保图是连通的。分配怪物数值，标记最大值为Boss。 | 随机算法, **并查集 (Union-Find)** 或 **BFS/DFS** 连通性检测 |

## 实现建议 (Implementation Guidance)

1. **图的存储**：推荐使用 `std::vector<std::list<int>>` 或 `std::map<int, std::vector<int>>` 来实现邻接表，存储房间ID的连接关系。
2. **连通性保证**：
    * 方法A（生成树法）：先生成一条贯穿所有节点的“主干”路径，再随机添加额外的边以增加复杂度。
    * 方法B（随机连接+检测）：随机连接节点，然后使用 BFS 从 Room 0 出发，检查是否能访问到所有节点。如果不行，继续添加边直到连通。
3. **Boss 生成**：遍历所有房间的怪物武力值，找到最大值，将其所在的房间标记为 Boss Room。

## 单元测试要求 (Unit Test Requirements)

* **`MapConnectivityTest`**: 生成一张地图，断言房间数量 $\ge 20$，且从任意房间出发可以通过路径到达任意其他房间。
* **`BossExistenceTest`**: 验证地图中必然存在且仅存在一个 Boss，且其武力值高于其他所有普通怪物。
* **`RoomAttributeTest`**: 验证房间内的宝箱和怪物数据已正确初始化，不是空指针或垃圾值。

## 阶段完成标识 (Phase Completion Indicator)

程序能够成功打印出地图的邻接表结构（或可视化文本），显示所有房间及其连接，且没有任何孤立房间。
