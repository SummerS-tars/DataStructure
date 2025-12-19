## Phase 4: 游戏闭环 - 交互与动态规划

**Prompt 目标：** 串联所有功能，实现“移动 -> 战斗 -> Boss 移动 -> 路径重规划”的完整游戏循环。

**涉及模块/文件：**

* `PJ2/web/static/app.js` (前端逻辑)
* `PJ2/src/GameSystem.cpp` (逻辑迁移)

**技术修改点：**

1. **实时路径高亮**:
* 前端维护一个 `highlightEdges` 数组。
* 每次 Boss 移动或玩家移动后，自动请求 `/shortest`。
* 获取返回的 Path ID 列表，在 Vis.js 中将对应的 Edge 颜色设为**红色粗线** (Red, Width=3)。


2. **动态 Boss 移动反馈**:
* 在前端实现一个“回合制”逻辑：玩家移动 3 步，或点击“等待”按钮，触发一次 `/boss/tick`。
* 动画效果：Boss 节点改变时，Vis.js 使用 `network.moveNode()` 或颜色渐变动画提示玩家威胁已转移。


3. **游戏结束判定**:
* 前端根据后端返回的 `status` ("VICTORY" / "GAME_OVER") 弹出 Modal 窗口，禁止后续操作并提供“重新生成地图”按钮。