# 需求与设计规范

## 1. 核心游戏循环 (Game Loop)

游戏采用 **持久化角色 (Persistent Player) + 随机生成副本 (Roguelite Dungeon)** 的模式。

* **大厅 (Lobby)**:
    * **加载/创建**: 读取 `user_save.json` 加载玩家状态。
    * **战利品收藏馆 (Collection)**: 玩家可以在此查看历史获得过的所有稀有道具（树状结构展示）。
    * **难度选择**: 系统根据玩家当前 `TotalPower` 推荐难度区间，玩家选择后生成地图。

* **生成 (Generation)**:
    * 后端根据难度因子生成地图拓扑（主干+支线）、怪物分布、战利品。
    * 自动归档当前 `dungeon_state.json` 。

* **探索 (Exploration)**:
    * **回合制**: 玩家移动 -> 时间流逝 -> Boss 移动 -> 状态更新。
    * **实时交互**: 战斗损耗 HP，拾取道具更新 Power。

* **结算 (Settlement)**:
    * **通关**: 击败 Boss。保留当前身上装备与新获得的战利品。
    * **死亡**: HP 归零。玩家被强制传回大厅，保留部分战利品，记录一次“失败探索”。

---

## 2. 详细功能模块设计

### 2.1. 玩家系统 (Player System)

玩家属性不再单单是一个整数，而是一个包含装备槽位的对象。

* **基础属性**:
    * `BasePower` (基础武力): 默认为 10 。
    * `HP` (生命值): 默认 100。归零即死。
    * `Gold` (财富): 统计所有道具的总价值。

* **装备槽 (Equipment Slots)**:
    玩家拥有 **5个独立槽位**，每个槽位只能装备一件物品。
    1. **Weapon (兵器)**
    2. **Helmet (头盔)**
    3. **Armor (护甲)**
    4. **Boots (靴子)**
    5. **Magic Stone (魔法石)**

* **武力值计算**:

    $$\text{TotalPower} = \text{BasePower} + \sum (\text{EquippedItems.Power})$$

    *(注：为了简化战斗逻辑，防具也可以折算为提供“战斗力/防御力”，在判定战斗胜负时统一使用 TotalPower，或者设定防具提供 HP 上限加成)*  

### 2.2. 道具与战利品系统 (Item & Loot)

后台数据结构维持 **树 (Tree)** 结构 ，前端提供背包和收藏展示。

* **道具分类 (Item Types)**:
    * **装备类 (Equipable)**: 对应上述5个槽位。
    * 属性: `Name`, `SlotType` (e.g., Helmet), `PowerBonus`, `Value`.
    * `PowerBonus` 参考值：
        * Weapon: 5~50
        * Helmet: 2~20
        * Armor: 3~30
        * Boots: 1~15
        * Magic Stone: 4~25

* **消耗品 (Consumable)**:
    * `Potion`: 恢复 HP。
    * `Scroll`: 驱散迷雾或瞬移。

* **收藏品 (Collectible)**:
    * 属性: `Value` (高额)。无法装备，仅用于“收藏展示”和增加“路线价值”。  
    * 存在成就物品，生成率很低，价值很高。

* **战利品管理 (Loot Manager)**:
    * **数据结构**: **多叉树 (N-ary Tree)**。
    * **结构定义**: （示例）  
        * Root (Inventory)
            * Node: Weapons
                * Leaf: Iron Sword
            * Node: Defense
                * Node: Helmets
                    * Leaf: Leather Cap
                * Node: Armors
                    * Leaf: Chainmail
                * Node: Boots
                    * Leaf: Swift Boots
            * Node: Magic Stones
                * Leaf: Fire Stone
            * Node: Consumable
                * Leaf: Health Potion
            * Node: Collectibles
                * Leaf: Golden Statue

* **功能**:
    * **背包 (In-Game)**: 列表展示，点击装备。
    * **收藏馆 (Showcase)**: 在大厅或侧边栏，展示玩家收藏的道具，满足“支持收藏展示”的要求 。

### 2.3. 地图系统 (Map System)

* **结构**: **无向图 (Undirected Graph)**，邻接表存储。  
    推荐使用 邻接表 (Adjacency List) 实现，方便计算路径。
    * **节点 (Node)**: 房间 (Room)
        * ID: 唯一标识。
        * Type: 入口、普通、BOSS房、特殊房（泉水/营地）。
        * Content:
            * Monster (Power: Int)
            * Chest (Item: Object, Boost: Int)
        * State: 已访问 / 未访问。
        * Passed: 若有怪物，是否已战斗胜利。
    * **边 (Edge)**: 房间连接 (Corridor)
        * 无权重，双向通行。
* **生成算法**: **主干+支线 (Backbone & Branches)**。
    1. 生成一条长链（Start -> ... -> Boss），保证深度。
    2. 在主干节点上随机挂载支线房间。
    3. 添加少量回环（Loop）增加连通性。
    4. **难度控制**: 根据难度因子 $F$ (1.0 ~ 3.0) 调整怪物数值。
        难度分级标准:
        * Easy: 推荐 Power [10, 30]。因子 $F = 1.0$。
        * Normal: 推荐 Power [31, 60]。因子 $F = 1.5$。
        * Hard: 推荐 Power [61, 100]。因子 $F = 2.0$。
        * Abyss: 推荐 Power [101+]。因子 $F = 3.0$。
        * *注：玩家可以“越级”挑战（如Power 20选Hard），高风险高回报。*
* **迷雾机制 (Fog of War)**:
    * 默认全图不可见。
    * `CurrentRoom` 及 `Neighbors` 可见。
    * 已探索节点保持可见（灰色，如果战斗没有成功则保持原色）。

### 2.4. 交互与战斗 (Interaction & Combat)

* **战斗逻辑**:
    * **压制 (Dominance)**: `Player.TotalPower >= Monster.Power` -> 无伤胜利。
    * **苦战 (Struggle)**: `Player.TotalPower < Monster.Power`
        * **战斗 (Fight)**: 扣除 HP。公式: $\text{Cost} = (\text{Monster} - \text{Player}) \times \text{Random}(0.8, 1.2)$ 。  
            *(注：若伤害 > CurrentHP，则禁止战斗或提示死亡风险)*  
    * **逃跑 (Flee)**: 玩家可以选择逃跑，扣除一定HP后，可以逃跑至别的房间，公式: $\text{Cost} = \text{Random}(5, 15) \times F$ 。
    * **策略**: 玩家若 HP 不足，需先喝药或寻找特殊房间，或者去其他房间寻找装备提升 Power 后再来。

* **Boss 行为**:
    * **移动**: 每 $N$ 回合，Boss 向随机邻居移动一格 。
    * **限制**: Boss 不会进入 Start 房间（避免堵门杀）。
    * **通知**: 界面上方提示“地底传来震动，Boss移动到了未知区域...”

### 2.5. 战术指导与多方案排序 (Guide System)

前端专门的 **"Strategy Terminal" (战术终端)** 面板，对应高阶算法要求。

1. **最短路径 (Shortest Path)**:
    * 算法: Dijkstra / BFS。
    * 展示: 地图上 **红色高亮** 连线。

2. **最高价值路径 (Top-K Value)**:
    * 算法: DFS/回溯 (带剪枝)。
    * 逻辑: 搜索 Start -> Boss 的路径，计算 (路径上未拾取道具总价值)。
    * 展示: 地图上 **金色高亮** 推荐路径，并列出 Top 3 方案的价值对比。

---

## 3. 技术实现栈 (Tech Stack)

### 3.1. 后端 (Python/FastAPI)

参考模块：  

* **MapGenerator**: 实现主干支线算法。
* **LootTree**: 实现树的插入、遍历、JSON序列化。
* **PathFinder**: 实现 Dijkstra 和 Top-K DFS。
* **GameEngine**: 维护 `GameState`，处理 `move`, `equip`, `save`。

### 3.2. 前端 (Vue.js 3 + D3.js/Vis.js)

参考模块：

* **Main View**:
    * **Map Canvas**: 使用 Vis.js 网络图绘制房间和连线。支持点击移动，迷雾遮罩。
* **Sidebar**:
    * **Player Stats**: 显示 HP 条, Total Power, 5个装备槽图标。
    * **Inventory/Collection**:
        * Tab 1: **背包** (当前可用)。
        * Tab 2: **收藏树** (Tree View 组件，展示所有战利品分类)。

* **Bottom Panel**:
    * **Adventure Log**: 滚动显示文本日志 。
    * **Strategy Buttons**: "Show Shortest Path", "Show Max Loot Path"。

---

## 4. 数据持久化方案 (Data Persistence)

* **user_save.json**:
    * 存储 `CollectionTree` (已解锁的道具图鉴)。
    * 存储 `PlayerStats` (历史最高分)。

* **dungeon_state.json**:
    * 存储当前地图结构 (Nodes, Edges)。
    * 存储当前实体状态 (Player Pos, HP, Inventory; Boss Pos)。
    * 存储 `Log` (移动历史栈)。
    * **加载逻辑**: 每次操作自动写入。刷新网页后读取此文件恢复现场。

### 4.1. 加载与重置机制

* **Resume (继续)**: 启动时检测是否存在 dungeon_state.json 且 game_over: false。若存在，直接加载该文件恢复现场。
* **New Game (新开)**: 覆盖/删除旧的 dungeon_state.json，重新生成。

---

## 5. 数据结构评分点映射 (Checklist)

1. **图 (Graph)**: 地图结构与连通性检查 。

2. **树 (Tree)**:
    * **战利品管理**: 必须实现一个 Tree Class 来管理背包和收藏 。
    * **地图生成**: 最小生成树(MST)思想用于构建骨架。

3. **排序与搜索 (Sort & Search)**:
    * Dijkstra (最短路)。
    * Top-K 排序 (多方案生成) 。

4. **栈 (Stack)**: 探险日志的回溯 。

---
