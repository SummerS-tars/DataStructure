这份开发指导文档是为你现有的代码架构量身定制的。它旨在修复数据混淆问题，并利用现有的组件（如 `InventoryPanel`）快速构建一个功能完备的“场外大厅（Lobby）”。

你可以将以下文档直接交给你的 AI Agent 或开发协作者。

---

# 开发任务书：Dungeon Crawler 场外大厅 (Lobby) 模式实现

## 1. 任务概述

**目标**：在现有前端项目中添加“场外大厅（Lobby）”界面。
**现状**：目前 `LobbyView.vue` 仅为占位符，且 `App.vue` 强制显示 `GameView`。前端存在将“永久存档”与“当前背包”混淆的 Bug。
**需求**：

1. 实现视图切换：玩家启动时进入大厅，点击开始后进入地牢。
2. 修复数据源：大厅展示“永久收藏（Collection）”，地牢展示“当前背包（Inventory）”。
3. 大厅功能：查看已收集的物品图鉴、选择难度开始新游戏。

---

## 2. 核心修改步骤 (Step-by-Step)

### 步骤一：重构 Pinia Store (`src/stores/gameStore.js`)

**目的**：分离“当前游戏状态”和“永久用户数据”，并管理视图状态。

1. **新增 State 字段**：
* `viewMode`: 字符串，枚举值 `'lobby'` | `'game'`，默认为 `'lobby'`。
* `permanentCollection`: 对象，用于存储从 `/save/user` 或 `/collection` 获取的完整图鉴树。


2. **修改 Actions**：
* **新建 `fetchUserCollection()**`：
* 调用 `gameApi.loadSave()` 或 `gameApi.getCollection()`。
* 将返回的 `collection_tree` 存入 `this.permanentCollection`。


* **修正 `resumeGame()` (关键修复)**：
* **删除**原有的“如果地牢没数据就合并用户数据”的逻辑（即删除 `if (!merged.collection_tree && userCollectionTree) ...` 块）。
* 逻辑变更为：尝试加载地牢 -> 如果有活跃地牢数据 -> `this.viewMode = 'game'`。
* 如果无活跃地牢数据 -> `this.viewMode = 'lobby'` -> 调用 `this.fetchUserCollection()`。


* **修改 `initGame(difficulty)**`：
* 调用 API 初始化成功后，设置 `this.viewMode = 'game'`。


* **新建 `returnToLobby()**`：
* 设置 `this.viewMode = 'lobby'`。
* 调用 `this.fetchUserCollection()` 刷新图鉴。





### 步骤二：实现 LobbyView (`src/views/LobbyView.vue`)

**目的**：创建一个利用现有组件的各类大厅界面。

1. **布局设计**：
* 采用两栏布局（类似 `GameView`）。
* **左侧**：操作区。
* 标题：“冒险家公会 / Hall of Heroes”。
* 难度选择按钮组（Easy, Normal, Hard, Abyss）。
* “开始新的冒险”按钮（调用 `store.initGame`）。
* “继续冒险”按钮（仅当 `store.status === 'playing'` 时显示）。


* **右侧**：图鉴展示区。
* 复用 `src/components/game/InventoryPanel.vue`。
* 传入 props: `:tree="store.permanentCollection"`。
* **注意**：在大厅模式下，点击物品不需要触发装备逻辑（或者可以显示物品详情弹窗），目前只需展示。





### 步骤三：修改根组件 (`src/App.vue`)

**目的**：根据 Store 状态控制显示哪个视图。

1. **模板逻辑 (`template`)**：
* 移除 `header` 中的难度按钮（这些按钮移动到 Lobby 中）。
* 使用 `v-if/v-else` 根据 `store.viewMode` 切换显示：
```html
<LobbyView v-if="viewMode === 'lobby'" />
<GameView v-else />

```




2. **Header 调整**：
* 在 `GameView` 激活时，Header 应该显示“返回大厅”或“放弃探险”按钮（调用 `store.returnToLobby`）。



### 步骤四：完善 GameView (`src/views/GameView.vue`)

**目的**：增强游戏结束后的交互。

1. **游戏结束处理**：
* 监听 `store.status`。
* 如果状态变为 `win` 或 `dead`，在界面显眼处（或弹窗）显示“结算面板”。
* 提供“返回大厅”按钮，触发 `store.returnToLobby()`。



---

## 3. 技术细节与注意事项

### A. 关于 InventoryPanel 的复用

`InventoryPanel.vue` 目前绑定了 `@equip` 事件。在大厅模式下，后端不支持 `/equip` 接口（因为不在地牢里）。

* **指导**：在使用该组件时，父组件（LobbyView）接收 `@equip` 事件但不调用 `store.equipItem`。可以选择 console.log 打印“查看物品详情：xxx”作为后续功能的预留。

### B. 后端接口确认

后端 `app/main.py` 已经提供了 `/collection` 接口：

```python
@app.get("/collection")
def get_collection_tree():
    return { "collection_tree": engine.loot_manager.to_dict() }

```

前端 Store 应使用此接口或 `/save/user` 来填充大厅的图鉴数据。

### C. 修正后的 Store 数据流示意

* **Lobby 状态**：
* `viewMode`: 'lobby'
* `permanentCollection`: **有数据** (来自 user_save)
* `player`: null (或者上一次的快照，但不用于显示)


* **Game 状态**：
* `viewMode`: 'game'
* `player.inventory`: **有数据** (来自 dungeon_state，初始为空)
* `collectionTree`: **有数据** (根据 inventory 动态生成，或者显示为空)



---

## 4. 给 Agent 的 Prompt (你可以直接复制这个发给 AI)

```text
请根据上述《开发任务书》，对我现有的 Vue 前端代码进行修改。

具体要求：
1. 修改 src/stores/gameStore.js：
   - 添加 viewMode 状态 ('lobby'/'game')。
   - 添加 permanentCollection 状态。
   - 修复 resumeGame 逻辑，不再将用户存档混合进地牢背包，而是根据是否有活跃地牢来决定进入 Lobby 还是 Game。
   - 添加 fetchUserCollection Action。

2. 重写 src/views/LobbyView.vue：
   - 实现一个包含难度选择和 Collection 展示的界面。
   - 复用 InventoryPanel 组件来展示 store.permanentCollection。

3. 修改 src/App.vue：
   - 使用 v-if 根据 viewMode 切换 LobbyView 和 GameView。
   - 将顶部的控制按钮移至 LobbyView 内部，GameView 顶部只保留状态信息和“返回大厅”按钮。

请保持现有的暗黑风格 UI 设计 (assets.css)。

```