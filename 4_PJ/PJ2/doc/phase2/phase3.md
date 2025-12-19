## Phase 3: 前端可视化 - 引入 Vis.js

**Prompt 目标：** 废弃纯文本展示，引入 `vis-network` 实现可拖拽、分层的地图展示，以及战利品树的 UI 渲染。

**涉及模块/文件：**

* `PJ2/web/static/index.html`
* `PJ2/web/static/app.js`

**技术修改点：**

1. **引入库**: 在 HTML 中通过 CDN 引入 `vis-network.min.js`。
2. **地图渲染 (Map Visualization)**:
* 编写 `renderMap(json)` 函数。将 JSON 转换为 Vis.js 的 `nodes` 和 `edges` DataSet。
* **样式映射**:
* `tier` -> `level` (用于 `layout: { hierarchical: ... }`)。
* `type` -> `color` (Start=Green, Boss=Red, Visited=Grey)。
* `items.length > 0` -> 节点 Label 上追加 "📦"。


* **交互**: 监听 `click` 事件，点击邻接节点触发 `step_move` API。


3. **战利品树 (Loot Tree)**:
* 使用 HTML `<details>` 和 `<summary>` 标签递归渲染 JSON 树结构，实现可折叠的背包视图。