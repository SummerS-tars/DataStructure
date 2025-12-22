这份文档是基于 **Project 2: Dungeon Crawler (Web Remake v4.0)** 的需求，为您提供的 **前端设计与实现指南**。我们将使用 **Vue 3 + Vite** 作为基础框架，结合 **Vis.js** 进行图的可视化渲染。

---

# Data Structure Project 2: Frontend Design Specification

## 1. 技术栈与工具 (Tech Stack)

* **Core Framework**: [Vue.js 3](https://vuejs.org/) (Composition API) + [Vite](https://vitejs.dev/)
* **State Management**: [Pinia](https://pinia.vuejs.org/) (用于管理全局 GameState)
* **Graph Visualization**: [Vis-Network](https://visjs.github.io/vis-network/docs/network/) (处理地图拓扑、物理布局、点击事件)
* **UI Components**: 原生 CSS 或轻量级库 (如 Naive UI / Element Plus)，建议保持轻量，手写 CSS 更有利于控制游戏风格。
* **HTTP Client**: [Axios](https://axios-http.com/)

---

## 2. 项目目录结构 (Project Structure)

```text
pj2_frontend/
├── index.html
├── src/
│   ├── assets/              # 图标 (SVG), 样式 (CSS)
│   ├── api/
│   │   └── game.js          # 封装后端 API 请求
│   ├── stores/
│   │   └── gameStore.js     # Pinia Store: 存储 Player, Map, Logs
│   ├── components/
│   │   ├── common/
│   │   │   └── ItemIcon.vue     # 道具图标组件 (复用)
│   │   ├── lobby/
│   │   │   ├── DifficultySelect.vue
│   │   │   └── CollectionTree.vue # 收藏馆树形展示
│   │   ├── game/
│   │   │   ├── DungeonMap.vue   # 核心地图 (Vis.js Wrapper)
│   │   │   ├── PlayerStatus.vue # HP, Power, 装备槽
│   │   │   ├── InventoryPanel.vue # 背包 (Tree View)
│   │   │   ├── StrategyTerminal.vue # 寻路/战术按钮
│   │   │   └── AdventureLog.vue # 日志滚动窗
│   ├── views/
│   │   ├── LobbyView.vue    # 大厅页面
│   │   └── GameView.vue     # 游戏主界面
│   ├── App.vue
│   └── main.js
└── package.json

```

---

## 3. 核心组件设计 (Component Details)

### 3.1. 游戏主界面布局 (`GameView.vue`)

采用经典的 **“三栏式”** 或 **“左图右操作”** 布局。

* **Left (70%)**: `DungeonMap` (占据主要视野)
* **Right (30%)**: 侧边栏
* Top: `PlayerStatus` (头像, HP, 5个装备格)
* Middle: `InventoryPanel` (Tab切换: 背包 / 详情)
* Bottom: `StrategyTerminal` & `AdventureLog`



---

### 3.2. 地图可视化组件 (`DungeonMap.vue`)

这是前端最复杂的部分，负责将后端返回的 `MapStructure` 渲染为图形。

* **Props**:
* `nodes`: Array (房间数据)
* `edges`: Array (连线数据)
* `highlightPath`: Array (用于显示最短路/寻宝路的高亮连线 ID)


* **主要逻辑**:
1. **Vis.js 初始化**: 在 `onMounted` 中挂载 DOM。
2. **迷雾渲染 (Fog of War)**:
* 后端返回的 Room 对象包含 `visible` 和 `visited` 字段。
* **Hidden**: 颜色灰色，无标签，不可点击。
* **Visible (Neighbor)**: 颜色亮起，显示图标（怪/宝箱），可点击。
* **Visited**: 颜色变暗（表示已去过），保持可见。
* **Current**: 高亮边框，显示“玩家小人”图标。


3. **交互事件**:
* `click`: 监听节点点击。如果点击的是 `visible` 的邻居 -> 触发 `gameStore.movePlayer(id)`。
* `hover`: 显示 Tooltip（例如：“怪物强度 50，你的强度 40，危险！”）。




* **样式定义 (Vis.js Options)**:
```javascript
nodes: {
    shape: 'dot',
    size: 20,
    font: { color: '#ffffff' },
    borderWidth: 2
},
edges: {
    color: { color: '#848484', highlight: '#ff0000' }, // 默认灰，高亮红
    width: 2
}

```



---

### 3.3. 玩家状态面板 (`PlayerStatus.vue`)

直观展示生存指标和装备情况。

* **Visuals**:
* **HP Bar**: 红色进度条，带动画过渡 (`width: ${hp}%`)。
* **Power**: 大号数字显示当前 `TotalPower`。
* **Equipment Slots**: 5个固定的正方形容器，分别对应 `weapon`, `helmet`, `armor`, `boots`, `magic_stone`。
* *空状态*: 显示底图轮廓。
* *有装备*: 显示道具图标 + 等级边框 (Tier颜色)。





---

### 3.4. 背包与收藏树 (`InventoryPanel.vue` & `CollectionTree.vue`)

为了满足数据结构要求，这里必须显式使用 **树形控件 (Tree View)**。

* **数据结构映射**:
后端传来的 `LootNode` 是嵌套 JSON：
```json
{ "name": "Equipment", "children": [ { "name": "Weapons", "items": [...] } ] }

```


* **组件实现**:
* `RecursiveTreeItem.vue`: 递归组件。
* 如果是 Folder -> 显示折叠/展开箭头，渲染 `children`。
* 如果是 Item (Leaf) -> 显示道具名、数值。
* **交互**: 点击 Item -> 弹出详情 Modal（"装备" / "丢弃"）。





---

### 3.5. 战术终端 (`StrategyTerminal.vue`)

对应后端的 `pathfinder` 功能。

* **UI**:
* Button A: **"BOSS 直通"** (Style: Red)
* Button B: **"贪婪寻宝"** (Style: Gold)


* **Logic**:
1. 点击按钮 -> 调用 `api.getGuide('shortest')`。
2. 获取路径 ID 列表 `[0, 5, 8, 12]`。
3. 调用 `gameStore.setHighlightPath(path)`。
4. `DungeonMap` 监听到变化，更新 Vis.js 的 edge 颜色。



---

## 4. 状态管理 (Pinia Store)

建立 `stores/game.js` 来统一管理数据流。

```javascript
import { defineStore } from 'pinia';
import { api } from '@/api';

export const useGameStore = defineStore('game', {
    state: () => ({
        player: null,    // 包含 hp, power, equipment
        mapData: { nodes: [], edges: [] },
        logs: [],
        isLoading: false,
        gameStatus: 'playing' // 'playing', 'win', 'dead'
    }),

    actions: {
        async initGame(difficulty) {
            this.isLoading = true;
            const data = await api.init(difficulty);
            this.syncState(data);
            this.isLoading = false;
        },

        async movePlayer(targetRoomId) {
            try {
                const data = await api.move(targetRoomId);
                this.syncState(data);
                // 如果移动成功，自动追加日志
                // data.logs 应该由后端返回
            } catch (error) {
                console.error("Move failed", error);
                // 处理“无法移动”或“战斗死亡”的提示
            }
        },

        syncState(data) {
            this.player = data.player;
            // 转换后端 MapStructure 为 Vis.js 格式
            this.mapData = this.transformMap(data.map_view); 
            this.logs = data.logs;
            this.gameStatus = data.status;
        },

        transformMap(backendMap) {
            // 将 Pydantic 模型转为 { nodes: [], edges: [] }
            // 重点处理 color (根据 visible/visited)
            return {
                nodes: backendMap.rooms.map(r => ({
                    id: r.id,
                    label: r.visible ? (r.type === 'boss' ? 'BOSS' : `${r.id}`) : '?',
                    color: r.id === this.player.current_room_id ? '#FFD700' : 
                           (r.visited ? '#4CAF50' : (r.visible ? '#2196F3' : '#9E9E9E')),
                    // ... 更多视觉属性
                })),
                edges: backendMap.edges.map(e => ({ from: e[0], to: e[1] }))
            };
        }
    }
});

```

---

## 5. API 接口层 (`api/game.js`)

封装 Axios 请求，与后端文档对应。

```javascript
import axios from 'axios';

const request = axios.create({ baseURL: 'http://localhost:8000' });

export default {
    init: (difficulty) => request.post(`/init?difficulty=${difficulty}`),
    
    // 移动
    move: (targetId) => request.post(`/move?target_room_id=${targetId}`),
    
    // 装备
    equip: (itemId) => request.post(`/equip?item_id=${itemId}`),
    
    // 战术指导
    getShortestPath: () => request.get('/guide/shortest'),
    getLootPath: () => request.get('/guide/best_loot'),
    
    // 存档
    loadSave: () => request.get('/save/user'),
    getDungeonState: () => request.get('/save/dungeon') // 恢复现场
};

```

---

## 6. 交互流程示例 (User Flow)

1. **Start**: 用户打开网页 -> `LobbyView`。
* 展示“历史最高分”。
* 展示 `CollectionTree` (已解锁图鉴)。
* 点击 "Start Adventure" -> 弹出难度选择 -> 进入 `GameView`。


2. **Explore**:
* `DungeonMap` 渲染初始 0 号房间（高亮）和邻居（蓝色）。其他全灰。
* 用户点击一个蓝色邻居节点。
* **Frontend**: 立即显示 Loading Spinner。
* **Backend**: 计算战斗结果，扣除 HP，更新位置，更新迷雾。
* **Frontend**: 接收新状态，重绘地图（新房间变黄，旧房间变绿，新邻居变蓝），HP 条扣减。
* **Log**: 底部滚动显示 "You fought a Goblin! Lost 12 HP."。


3. **Strategy**:
* 用户迷路了，点击 "Show Shortest Path"。
* 地图上出现一条红线直通 Boss。用户沿着红线点击。


4. **Loot**:
* 打完怪掉落 "Iron Sword"。
* `InventoryPanel` 的 "Weapons" 节点下出现新 Item。
* 用户点击 Item -> "Equip"。
* `PlayerStatus` 的 Weapon 槽位显示剑图标，Total Power +10。



---

## 7. 样式建议 (Styling)

为了符合“地下城”主题，建议 CSS 风格如下：

* **Theme**: Dark Mode (背景色 `#1a1a1a`)。
* **Colors**:
* Text: `#e0e0e0`
* Accent (Gold): `#ffd700` (用于高价值物品/玩家)
* Danger (Red): `#ff4444` (用于 Boss/低血量)
* Safe (Green): `#4caf50` (用于已探索区域)


* **Font**: Monospace (如 'Courier New', 'Fira Code')，增加复古 RPG 终端的感觉。

这份前端设计文档与后端实现完美契合，能够指导你快速搭建出既符合数据结构课程要求（树、图的可视化），又具备良好游戏体验的 Web 应用。