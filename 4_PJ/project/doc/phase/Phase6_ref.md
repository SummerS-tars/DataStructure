这个需求把游戏从纯粹的 Roguelike 变成了类似《逃离塔科夫》或《暗区突围》的**“带入/撤离”（Extraction）机制**。这是一个非常棒的玩法升级！

这意味着：

1. **风险（Risk）**：玩家在大厅选择装备带入地牢。
2. **消耗（Loss）**：进入地牢的那一刻，这些装备就从“仓库”转移到了“角色”身上。如果角色死亡，装备也就没了（因为仓库里已经扣除了，而角色也没带回来）。
3. **收益（Reward）**：如果获胜或成功撤离，身上的装备（原有的+新捡的）需要合并回仓库。

为了实现这个机制，我们需要修改后端以支持“携带物品初始化”，并修改前端以支持“备战选择”。

以下是交给 Agent 的开发指导文档。

---

# 开发任务书：实现“备战与装备带入”机制 (Loadout & Permadeath)

## 1. 机制定义

* **备战阶段（Lobby）**：玩家可以在大厅的“收藏图鉴”中勾选想要带入地牢的装备（Loadout）。
* **进入地牢（Init）**：选中的装备会从“永久收藏（User Save）”中**移除**，并直接出现在玩家初始的“背包（Inventory）”中。
* **结算规则（Result）**：
* **胜利/撤离**：玩家背包内剩余的所有物品（带入的 + 捡到的）自动存回“永久收藏”。
* **死亡**：玩家背包内的物品全部丢失（永久收藏里已被扣除，无法恢复）。



---

## 2. 后端修改规范 (Python/FastAPI)

### 2.1 修改 `app/models.py`

需要更新请求模型，允许前端传递装备列表。

* **新增** `InitGameRequest` 模型（或者直接修改 API 参数）：
```python
class InitGameRequest(BaseModel):
    difficulty: str = "easy"
    loadout_ids: List[int] = []  # 新增：玩家决定带入的物品ID列表

```



### 2.2 修改 `app/core/loot_manager.py`

`LootManager` 需要支持“提取物品”的操作。

* **新增方法** `extract_items(self, item_ids: List[int]) -> List[Item]`:
1. 遍历 `item_ids`。
2. 在 `self.root` 树中查找对应的 Item。
3. 找到后，将 Item 从树/列表中**删除**（pop/remove）。
4. 返回被删除的 Item 对象列表。
*注意：必须确保这是一个“剪切”操作，而不是“复制”，否则无法实现死亡掉落。*



### 2.3 修改 `app/core/game_engine.py`

这是核心逻辑变更的地方。

* **修改 `new_game` 方法**：
* 签名变更：`def new_game(self, difficulty: str, loadout_ids: List[int] = []) -> GameResponse:`
* 逻辑变更：
1. 调用 `self.loot_manager.extract_items(loadout_ids)` 获取物品对象列表。
2. 将这些物品添加到新创建的 `PlayerState.inventory` 中。
3. **关键步骤**：立即调用 `self._persist_user_collection()`。
* *原因*：一旦游戏开始，仓库文件必须立即更新，把带走的装备扣掉。防止玩家刷新网页后装备还在仓库里（刷装备 Bug）。






* **修改 `_enter_room` (胜利逻辑)**：
* 当 `state.status` 变为 `"win"` 时：
1. 获取当前玩家背包 `player.inventory` 及身上装备 `player.equipment`。
2. 调用 `self.loot_manager.ingest_items(...)` 把这些物品存回仓库。
3. 调用 `self._persist_user_collection()` 保存仓库。




* **关于死亡**：
* 不需要额外代码。因为 `new_game` 时已经扣除了仓库，如果死掉了，不执行“存回仓库”的操作，装备自然就消失了。



### 2.4 修改 `app/main.py`

* 更新 `/init` 接口：
* 接收 `loadout_ids` 参数。
* 传递给 `engine.start_game` -> `engine.new_game`。



---

## 3. 前端修改规范 (Vue)

### 3.1 修改 `src/stores/gameStore.js`

* **State 新增**：
* `selectedLoadout`: `Set<number>` 或 `Array<number>`，用于存储在大厅选中的物品 ID。


* **Actions 修改**：
* `toggleLoadoutItem(itemId)`: 切换物品的选中状态。
* `initGame(difficulty)`: 调用 API 时，将 `Array.from(this.selectedLoadout)` 作为参数传递给后端。



### 3.2 修改 `src/views/LobbyView.vue`

* **交互逻辑**：
* 点击右侧 `InventoryPanel` 的物品时，不再是 `console.log`，而是调用 `store.toggleLoadoutItem(item.id)`。


* **视觉反馈**：
* 需要高亮显示已选中的物品（例如给 Item 添加一个金色边框或 ✅ 图标）。


* **开始按钮**：
* 在“开始冒险”按钮旁显示：“已选择 N 件装备带入”。
* 添加警告提示：“注意：死亡将丢失所有带入的装备！”



### 3.3 修改 `src/components/game/InventoryPanel.vue`

* **Props 新增**：
* `selectedIds`: `Array` 或 `Set`，用于判断哪些物品被选中了。


* **样式更新**：
* 为 `selectedIds.includes(item.id)` 的物品添加 `.selected` 样式类。



---

## 4. 给 Agent 的 Prompt

你可以直接复制下面这段话给你的 AI 助手：

```text
请根据上述《开发任务书：实现“备战与装备带入”机制》，对我现有的前后端代码进行重构。

主要文件修改点如下：

【后端 Python】
1. app/core/loot_manager.py: 添加 extract_items(ids) 方法，实现从 collection 树中移除并返回物品对象。
2. app/core/game_engine.py: 
   - 修改 new_game 方法，接收 loadout_ids。
   - 初始化玩家时，先从 loot_manager 提取这些物品放入 player.inventory。
   - 立即执行 _persist_user_collection() 以保存“装备已移除”的状态。
   - 在 _enter_room 中，如果检测到游戏胜利 (win)，将玩家当前 inventory 和 equipment 合并回 loot_manager 并保存。
3. app/main.py: 更新 /init 接口，支持 POST 请求体包含 { difficulty, loadout_ids }。

【前端 Vue】
1. src/stores/gameStore.js: 添加 selectedLoadout 状态，更新 initGame action 以发送 loadout_ids。
2. src/components/game/InventoryPanel.vue: 接收 selectedIds prop，并在 item 上渲染选中样式。
3. src/views/LobbyView.vue: 
   - 点击物品触发 store.toggleLoadoutItem。
   - 将选中的 ID 集合传给 InventoryPanel。
   - 在开始游戏按钮处增加“死亡掉落”的风险提示。

请确保逻辑符合“Roguelike/Extraction”模式：带入即从仓库扣除，死亡不退还，胜利才带回。

```