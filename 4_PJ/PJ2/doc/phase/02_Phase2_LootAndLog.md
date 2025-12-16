# Phase 2: 战利品管理树与探险日志 (Loot Tree & Adventure Log)

## 目标 (Goal)

实现基于树结构的物品分类管理系统，以及基于线性表的探险日志记录系统，用于存储和展示玩家的状态变化。

## 需要实现的模块 (Modules to Implement/Modify)

| 模块名称 | 职责与目标 | 关键数据结构/算法 |
| :--- | :--- | :--- |
| **`LootTree`** (Class) | 管理玩家获得的所有道具。根节点为“背包”，下分“武器”、“防具”等类别节点，叶子节点为具体物品。 | **树 (Tree) - 多叉树表示法** |
| **`AdventureLog`** (Class) | 记录每次游戏的摘要（路径、收益、时间）。支持追加记录和按序打印。 | **线性表 (Linear List)** - `std::vector` 或 链表 |
| **`Player`** (Class) | 维护玩家当前状态（武力值、持有物品），连接 `LootTree` 和 `AdventureLog`。 | 状态管理 |

## 实现建议 (Implementation Guidance)

1. **树的构建**：`LootTree` 在初始化时应硬编码生成固定的分类节点（如：Root -> [Weapons, Armors, Potions]）。
2. **物品插入**：当玩家获得 `Item` 时，根据 `Item.type` 找到树中对应的分类节点，将该物品作为子节点挂载。
3. **递归展示**：编写一个递归函数 `TraversePrint(Node* node, int depth)`，利用缩进展示树的层级结构。
4. **日志记录**：设计一个 `LogEntry` 结构体，包含 `timestamp`, `path_string`, `total_value`。每次探险结束时，创建一个实例并 `push_back` 到线性表中。

## 单元测试要求 (Unit Test Requirements)

* **`LootTreeInsertTest`**: 模拟获得一把“屠龙刀”，验证其是否正确出现在“武器”分类节点下。
* **`LootTreeDisplayTest`**: 验证树的遍历输出是否覆盖了所有已获得的物品，且层级正确。
* **`LogAppendTest`**: 模拟完成两次探险，验证日志列表中包含2条记录，且内容与模拟数据一致。

## 阶段完成标识 (Phase Completion Indicator)

玩家获得物品后，能通过命令打印出分类清晰的树状背包列表；探险结束后，能查询到过往的探险历史记录。
