# Phase 1: 核心数据结构与生成算法 (后端核心)

## 1. 目标概述
实现本项目最核心的数据结构部分：**图 (Map Graph)** 和 **树 (Loot Tree)**。此阶段不涉及前端界面，主要通过 API 和单元测试验证生成的 JSON 数据结构是否正确。

## 2. 详细任务清单

### 2.1. 数据模型定义 (Pydantic Models)
* [ ] **编写 `models.py`**:
    * 定义 `Item`, `ItemType` 枚举。
    * 定义 `Room` (包含 `neighbors` 列表实现邻接表), `MapStructure`。
    * 定义 `PlayerState` 和 `EquipmentSlots`.

### 2.2. 战利品树实现 (Tree Implementation)
* [ ] **编写 `loot_manager.py`**:
    * 实现 `LootNode` 类 (N叉树节点)。
    * 实现 `add_child` 和 `to_dict` (递归序列化) 方法.
    * 构建预设的物品库树结构 (Root -> Equipment -> Weapons -> Iron Sword).
    * *测试点*: 调用 `to_dict()` 能打印出完整的嵌套 JSON。

### 2.3. 地图生成器实现 (Graph Implementation)
* [ ] **编写 `map_gen.py`**:
    * 实现 **主干生成算法**: 保证 Start 到 Boss 的长链存在.
    * 实现 **支线挂载**: 在主干节点随机添加支线房间。
    * 实现 **回环添加**: 随机连接非邻接点增加连通性。
    * 实现 **数值填充**: 根据 `number_counting.md` 的公式，基于 BFS 距离计算每个房间怪物的 Power.
    * *测试点*: 编写脚本检查生成的 Map 是否连通 (BFS/DFS 遍历)，确认 Start 到 Boss 存在路径。

### 2.4. 初始化接口
* [ ] **编写 `/init` 接口**: 接收 `difficulty` 参数，调用 MapGenerator，返回完整的 `GameResponse`.

## 3. 成功验收标准 (Definition of Done)
1.  **API 测试**: 在 Swagger UI (`/docs`) 调用 `/init?difficulty=normal`。
2.  **数据验证**: 返回的 JSON 中 `rooms` 包含至少 10+ 个房间，且 `neighbors` 关系正确对应 (即 A 在 B 的邻居里，B 也必须在 A 的邻居里)。
3.  **数值验证**: 检查 JSON 中越靠近 Boss 的房间，Monster Power 越高。