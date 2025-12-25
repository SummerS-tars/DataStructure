# Phase 3: 游戏循环与交互逻辑 (Game Loop)

## 1. 目标概述
打通“移动-战斗-拾取-成长”的完整闭环。实现后端的逻辑判断和前端的状态同步。

## 2. 详细任务清单

### 3.1. 核心游戏引擎 (Backend GameEngine)
* [ ] **编写 `game_engine.py`**:
    * 实现 `move(target_id)`:
        * 验证 `target_id` 是否为当前房间的邻居。
        * **战斗计算**: 应用 `number_counting.md` 中的战斗公式 (压制/苦战)。
        * **状态更新**: 扣除 HP，更新 `current_room`，更新 `visible` 区域 (迷雾揭开)。
        * **Boss 移动**: 实现 Boss 每 N 回合移动逻辑。
    * 实现 `equip(item_id)`: 更新装备槽，重新计算 `TotalPower`。

### 3.2. 前端交互集成
* [ ] **移动交互**: 在 `DungeonMap.vue` 的点击事件中调用 `gameStore.movePlayer(id)`。
* [ ] **动态更新**: 移动后，前端根据返回的新 State 重新渲染地图颜色 (新房间变当前，旧房间变已访问)。
* [ ] **背包交互**:
    * 在 `InventoryPanel.vue` 中实现点击装备 -> 调用 `/equip` 接口。
    * 装备成功后，`PlayerStatus` 显示对应装备图标，Power 数值上升。
* [ ] **日志系统**: 实现 `AdventureLog.vue`，显示后端返回的战斗/移动文本日志。

## 3. 成功验收标准 (Definition of Done)
1.  **可玩性**: 玩家可以点击地图移动，血量会扣减，地图迷雾会随移动散开。
2.  **战斗结果**: 遇到弱怪不掉血，遇到强怪掉血。HP 归零时后端状态变为 "dead"。
3.  **装备系统**: 捡到装备后能穿上，且 Power 值确实增加了。