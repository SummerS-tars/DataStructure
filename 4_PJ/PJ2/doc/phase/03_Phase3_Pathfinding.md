# Phase 3: 路径搜索与排序算法 (Pathfinding & Sorting)

## 目标 (Goal)

实现核心算法模块，计算“最少探索次数”的最短路径，以及计算“价值最高”的最优路径并进行排序。

## 需要实现的模块 (Modules to Implement/Modify)

| 模块名称 | 职责与目标 | 关键数据结构/算法 |
| :--- | :--- | :--- |
| **`PathFinder`** (Class) | 封装寻路逻辑，操作 `DungeonMap` 数据。 | 算法封装 |
| **`FindShortestPath`** | 给定起点和Boss房，寻找经过房间数最少的路径。 | **广度优先搜索 (BFS) + 队列 (Queue)** |
| **`FindAllPaths`** | 寻找起点到Boss房的所有无环路径，并计算每条路径的总收益。 | **深度优先搜索 (DFS) + 栈/递归** |
| **`PathSorter`** | 对 `FindAllPaths` 找到的路径列表，按总价值从高到低排序。 | **排序算法 (QuickSort / MergeSort)** |

## 实现建议 (Implementation Guidance)

1. **BFS 实现**：使用 `std::queue`。维护一个 `visited` 数组和一个 `parent` 映射（用于回溯路径）。当首次访问到目标节点时，停止搜索，通过 `parent` 映射反推路径。
2. **DFS 实现**：使用递归。需要传递当前路径 `currentPath` 和 `visited` 状态。注意回溯（Backtracking）时要将当前节点从 `visited` 中移除，以便寻找其他路径。
3. **排序**：定义一个 `PathInfo` 结构体（包含路径列表和总价值）。重载比较运算符或提供 Comparator，使用排序算法对 `vector<PathInfo>` 进行排序。

## 单元测试要求 (Unit Test Requirements)

* **`BFS_ShortestPathTest`**: 在已知简单图（如环形或链式）中，验证算法返回的确实是最短跳数的路径。
* **`DFS_AllPathsTest`**: 在小规模图（如5个节点）中，验证算法能找到所有可能的到达目标路径。
* **`PathSortingTest`**: 模拟几条价值不同的路径，验证排序后的列表首元素是否为价值最高的那条。

## 阶段完成标识 (Phase Completion Indicator)

输入起点和终点，系统能输出一条最短路径；系统能列出多条推荐路径，且按预期的战利品价值降序排列。
