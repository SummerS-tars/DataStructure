# Phase 2: 地图可视化与基础 UI (前端核心)

## 1. 目标概述
将 Phase 1 生成的数据在前端进行可视化。重点是使用 **Vis.js** 渲染地图，并处理 **迷雾 (Fog of War)** 的视觉逻辑。

## 2. 详细任务清单

### 2.1. 状态管理 (Pinia Store)
* [ ] **编写 `gameStore.js`**:
    * 实现 `initGame(difficulty)` action，调用后端 API 并存储 `mapData` 和 `player`.
    * 实现 `transformMap` 函数：将后端 `MapStructure` 转换为 Vis.js 需要的 `{ nodes, edges }` 格式。

### 2.2. 地图组件 (DungeonMap.vue)
* [ ] **Vis.js 初始化**: 在组件挂载时渲染 canvas。
* [ ] **迷雾渲染逻辑**:
    * 根据节点 `visible` 属性设置颜色 (Visible=蓝色/高亮, Visited=绿色, Hidden=灰色/不可见).
    * 根据节点 `type` 更改形状或标签 (Boss 房特殊标记)。
* [ ] **点击交互桩**: 添加 `click` 事件监听，目前仅需 `console.log` 打印点击的房间 ID。

### 2.3. 基础布局与面板
* [ ] **侧边栏开发**:
    * `PlayerStatus.vue`: 显示 HP 条和 Total Power 数字。
    * `InventoryPanel.vue`: 使用递归组件展示 Phase 1 后端传来的 Tree 数据 (即使目前背包是空的，先展示图鉴结构).

## 3. 成功验收标准 (Definition of Done)
1.  **可视化**: 页面加载后，能看到生成的地下城地图拓扑结构。
2.  **迷雾效果**: 只有 Start 房间及其邻居是亮起的（有 Label），其他房间是灰色的（无 Label）。
3.  **数据同步**: 侧边栏显示的 Player HP/Power 与后端初始值一致。