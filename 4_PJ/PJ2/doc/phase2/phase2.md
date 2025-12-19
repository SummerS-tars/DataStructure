## Phase 2: 中间层状态管理 - Python API 升级

**Prompt 目标：** 让 Python 后端不再只是“透传”命令，而是维护简单的游戏状态（Stateful），处理存档读写。

**涉及模块/文件：**

* `PJ2/web/server.py`

**技术修改点：**

1. **全局状态管理**:
* 在 Python 中维护一个 `current_map_path` 变量或 Session 机制。
* 实现 `load_map_state()` 和 `save_map_state()` 辅助函数，确保每次操作都是基于最新存档。

2. **API 逻辑增强**:
* **`POST /action/move`**: 接收 `{target_room_id}`。
1. 调用 C++ 引擎验证移动合法性（是否邻接）。
2. 执行战斗计算（Python 端简单模拟或调用 C++ 战斗接口）。
3. 更新玩家位置和状态。
4. 返回包含 `player_status`, `map_updates`, `logs` 的完整 JSON。

* **`POST /boss/tick`**: 专门的接口触发 Boss 移动。调用 C++ `bossmove` 后，立即自动调用 `shortest` 重新计算路径，将**新 Boss 位置**和**新路径**一同返回。
