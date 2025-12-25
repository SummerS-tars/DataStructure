要实现“列出前几个最好的路径供玩家选择”，我们需要做两件事：

1. **修改 Store (`src/stores/gameStore.js`)**：在状态中增加 `guidePaths` 来存储后端返回的多条路径数据，并在获取到“贪婪寻宝”的数据时更新该状态，而不是直接高亮某一条。
2. **修改 UI 组件 (`src/components/game/StrategyTerminal.vue`)**：在界面上展示这些路径的列表（显示价值和步数），并允许玩家点击列表项来在地图上高亮对应的路径。

以下是具体的代码修改方案：

### 1. 修改 `src/stores/gameStore.js`

我们需要在 `state` 中添加 `guidePaths`，并重写 `fetchGuide` 方法来区分处理“最短路径”（直接高亮）和“贪婪寻宝”（存入列表供选择）。

```javascript
{
type: "diff",
fileName: "src/stores/gameStore.js",
diff: `
<<<<
    viewMode: 'lobby',
    selectedLoadout: [],
    mapViewRaw: null,
    highlightPath: [],
  }),
  actions: {
    setLoading(flag) {
====
    viewMode: 'lobby',
    selectedLoadout: [],
    mapViewRaw: null,
    highlightPath: [],
    guidePaths: [], // 新增：存储候选路径列表
  }),
  actions: {
    setLoading(flag) {
>>>>
<<<<
    async fetchGuide(type = 'shortest') {
      try {
        const res =
          type === 'loot' ? await gameApi.guideLoot() : await gameApi.guideShortest();
        const path = res?.data?.path || res?.data?.nodes || res?.data;
        if (Array.isArray(path)) {
          this.setHighlightPath(path);
        }
      } catch (err) {
        console.error('Fetch guide failed', err);
      }
    },
====
    async fetchGuide(type = 'shortest') {
      // 每次请求前先重置状态
      this.guidePaths = [];
      this.highlightPath = [];
      
      try {
        if (type === 'loot') {
          const res = await gameApi.guideLoot();
          // 贪婪模式：将返回的路径列表存入 guidePaths，供 UI 展示选择
          if (res?.data?.paths && Array.isArray(res.data.paths)) {
            this.guidePaths = res.data.paths;
            // 可选：默认高亮第一条（价值最高的）
            if (this.guidePaths.length > 0) {
              this.setHighlightPath(this.guidePaths[0].path);
            }
          }
        } else {
          // 最短路径模式：保持原有逻辑，直接高亮
          const res = await gameApi.guideShortest();
          const path = res?.data?.path || res?.data?.nodes || res?.data;
          if (Array.isArray(path)) {
            this.setHighlightPath(path);
          }
        }
      } catch (err) {
        console.error('Fetch guide failed', err);
      }
    },
>>>>
`
}

```

### 2. 修改 `src/components/game/StrategyTerminal.vue`

在终端面板中增加一个列表区域。当检测到 `guidePaths` 有数据时，显示这些路径选项。点击选项调用 `store.setHighlightPath`。

```javascript
{
type: "diff",
fileName: "src/components/game/StrategyTerminal.vue",
diff: `
<<<<
    <div class="tip">
      {{ loading ? '请求中...' : '点击节点移动，路径高亮仅作参考' }}
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue';
import { useGameStore } from '../../stores/gameStore';

const store = useGameStore();
const loading = computed(() => store.loading);

const guide = (type) => store.fetchGuide(type === 'loot' ? 'loot' : 'shortest');
</script>

<style scoped>
====
    <div class="tip">
      {{ loading ? '请求中...' : '点击节点移动，路径高亮仅作参考' }}
    </div>

    <div v-if="guidePaths.length" class="path-list">
      <div class="list-label">检测到多条高价值路径：</div>
      <div 
        v-for="(item, idx) in guidePaths" 
        :key="idx"
        class="path-item"
        @click="previewPath(item.path)"
      >
        <div class="path-info">
          <span class="path-name">方案 {{ String.fromCharCode(65 + idx) }}</span>
          <span class="path-meta">{{ item.path.length }} 步</span>
        </div>
        <div class="path-value">
          <span class="coin-icon">⟡</span> {{ item.value.toFixed(0) }}
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue';
import { useGameStore } from '../../stores/gameStore';

const store = useGameStore();
const loading = computed(() => store.loading);
const guidePaths = computed(() => store.guidePaths);

const guide = (type) => store.fetchGuide(type === 'loot' ? 'loot' : 'shortest');
const previewPath = (path) => store.setHighlightPath(path);
</script>

<style scoped>
>>>>
<<<<
.tip {
  color: #9aa4b8;
  font-size: 12px;
}
</style>
====
.tip {
  color: #9aa4b8;
  font-size: 12px;
}

.path-list {
  margin-top: 10px;
  border-top: 1px solid #2a3142;
  padding-top: 10px;
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.list-label {
  font-size: 12px;
  color: #9aa4b8;
  margin-bottom: 4px;
}

.path-item {
  background: #1f2533;
  border: 1px solid #2f3647;
  border-radius: 8px;
  padding: 8px 12px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  cursor: pointer;
  transition: all 0.2s;
}

.path-item:hover {
  background: #2b3345;
  border-color: #3c4760;
}

.path-info {
  display: flex;
  flex-direction: column;
}

.path-name {
  font-weight: 700;
  color: #7ad7ff;
  font-size: 13px;
}

.path-meta {
  font-size: 11px;
  color: #6b7280;
}

.path-value {
  font-family: monospace;
  font-weight: 700;
  color: #ffd700;
  font-size: 14px;
}

.coin-icon {
  font-size: 12px;
  margin-right: 2px;
}
</style>
>>>>
`
}

```

### 实现效果说明

1. **数据层**：Store 现在的 `guidePaths` 数组会保存后端返回的所有候选路径。
2. **交互层**：
* 点击“BOSS 直通”时，行为不变（直接高亮最短路径，不显示列表）。
* 点击“贪婪寻宝”时，Store 会默认高亮价值最高的一条，并在下方显示一个列表（方案 A、方案 B...）。
* 玩家点击列表中的任意一项，地图上的高亮路径会实时切换到该方案。