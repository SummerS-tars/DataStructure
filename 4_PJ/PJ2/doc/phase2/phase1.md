# Phase 1: 数据层重构 - 线性地图生成与 JSON 契约

**Prompt 目标：** 1.  重写地图生成算法，从“随机连通图”改为**“长主干+分支”**结构，显著降低连通密度，增加探索深度。
2.  改造 C++ 引擎输出，支持包含层级信息 (`tier`) 和房间类型 (`type`) 的 JSON 格式。

**涉及模块/文件：**

* `PJ2/include/Room.h`
* `PJ2/src/MapGenerator.cpp`
* `PJ2/include/MapGenerator.h`
* `PJ2/src/DungeonMap.cpp`
* `PJ2/src/engine_main.cpp`

**技术修改点详解：**

### 1. Room 结构增强 (Hierarchy & Type)

* **文件**: `PJ2/include/Room.h`
* **修改**:
* 增加枚举 `RoomType { START, NORMAL, BOSS, TREASURE }`。
* 增加字段 `int tier` (表示距离起点的深度/步数)。
* 增加字段 `RoomType type`。
* (可选) 增加 `std::string label` 用于前端显示自定义名称（如 "Armory", "Nest"）。

### 2. **核心修改：线性地图生成算法 (Linear Dungeon Generation)**

* **文件**: `PJ2/src/MapGenerator.cpp`
* **算法逻辑**: 废弃原有的 `shuffle` + `iota` 生成树逻辑，改为以下步骤：

    1. **确立主干 (The Backbone)**:
        * 从总房间数中划出约 50%-60% 的房间作为“主干节点”。
        * 从 ID 0 (Start) 开始，依次连接下一个随机的主干节点，形成一条长链：`Start -> A -> B -> ... -> Boss`。
        * **效果**: 保证了 Start 到 Boss 的最短路径至少有一定长度（例如 10 步），避免“出门就到 Boss”。

    2. **挂载分支 (The Branches)**:
        * 将剩余的房间（非主干）作为“分支节点”。
        * 遍历这些分支节点，随机选择一个**已存在于地图中**的房间作为父节点进行连接。
        * *优化策略*: 可以在分支末端（叶子节点）高概率放置高价值宝箱 (`TREASURE` 房)。

    3. **受控添加回路 (Controlled Loops)**:
        * `extra_edges` 的数量大幅减少（建议设为房间数的 10%-20%）。
        * **关键限制**: 添加随机边 `(u, v)` 时，限制 `abs(tier[u] - tier[v]) <= 2`。
        * **目的**: 允许局部的小回路（增加战术绕路的可能性），但严禁连接“浅层”和“深层”房间（防止出现直通 Boss 的捷径）。

    4. **计算层级 (Tier Calculation)**:
        * 生成完成后，运行一次 BFS，从 Start 节点出发计算所有节点的距离，赋值给 `Room.tier`。

### 3. JSON 序列化 (JSON Serialization)

* **文件**: `PJ2/src/DungeonMap.cpp` (修改 `save` 或新增 `to_json`)
* **输出格式**:  

    ```json
    {
    "rooms": [
        {
        "id": 0, 
        "type": "START", 
        "tier": 0, 
        "power": 0, 
        "items": [],
        "label": "Entrance"
        },
        {
        "id": 1, 
        "type": "NORMAL", 
        "tier": 1, 
        "power": 15, 
        "items": [{"name": "Dagger", "type": "Weapon", "val": 10}]
        },
        ...
        {
        "id": 19, 
        "type": "BOSS", 
        "tier": 12, 
        "power": 99, 
        "items": []
        }
    ],
    "edges": [
        {"source": 0, "target": 1},
        ...
    ],
    "bossId": 19,
    "connected": true
    }
    ```

*(注: `source`/`target` 对应 Vis.js 的边格式，比 `u`/`v` 更直接)*

### 4. CLI Engine 适配

* **文件**: `PJ2/src/engine_main.cpp`
* **修改**:
* `cmd_generate`: 调用新算法生成地图，并直接打印 JSON 到标准输出 (stdout)。
* `cmd_shortest`: 保持逻辑不变，但确保其能在新的深层地图上正确工作（BFS 天生支持）。

### 如何给 Agent 下指令

你可以直接复制下面的这段话给 IDE Agent：

> "Agent，我们开始 **Phase 1: 数据层重构**。重点是改进地图生成逻辑。
> 1. 请修改 `Room.h`，添加 `tier` (int) 和 `type` (Enum: START, NORMAL, BOSS, TREASURE)。
> 2. 重写 `MapGenerator.cpp`。**废弃之前的随机生成树算法**，实现一个新的**“主干+分支”算法**：
> * 选出约 60% 的节点构建一条从 Start 到 Boss 的长链（主干）。
> * 将剩余节点随机挂载到主干或已有的分支节点上。
> * 添加少量随机边（extra edges），但要避免连接 tier 差距过大的节点（防止捷径）。
> * 最后运行 BFS 计算每个房间的 `tier`。
> 
> 
> 3. 修改 `DungeonMap`，增加 `to_json()` 方法，输出包含 tier 和 type 的 JSON 字符串。
> 4. 更新 `engine_main.cpp` 的 generate 命令以输出此 JSON。"
> 
>