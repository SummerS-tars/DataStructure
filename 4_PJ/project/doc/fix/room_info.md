好的，为了实现鼠标悬停显示房间信息的功能，我们需要在前端构建地图数据时，为每个节点添加 `title` 属性。`vis-network` 地图库会自动识别该属性，并在鼠标悬停时以 Tooltip 的形式显示内容。

你需要修改 `src/stores/gameStore.js` 文件，具体在 `transformMap` 方法中增加构建 `title` 的逻辑。

以下是修改方案：

```javascript
{
type: "diff",
fileName: "src/stores/gameStore.js",
diff: `
<<<<
        const label = r.visible || r.visited
          ? isBoss
            ? 'BOSS'
            : \`\${r.id}\`
          : '';

        return {
          id: r.id,
          label,
          color,
          shape: 'dot',
          size: isCurrent ? 22 : onPath ? 20 : 18,
          borderWidth: isCurrent ? 3 : onPath ? 2.5 : 1.5,
          allowed: r.visible || r.visited || isCurrent, // exposed for UI checks
        };
      });
====
        const label = r.visible || r.visited
          ? isBoss
            ? 'BOSS'
            : \`\${r.id}\`
          : '';

        // 构建鼠标悬停时的提示信息 (Tooltip)
        let tooltipLines = [];
        if (r.visible || r.visited) {
          tooltipLines.push(\`[\${r.id}] \${(r.type || 'room').toUpperCase()}\`);
          if (isCurrent) tooltipLines.push('📍 当前位置');
          
          if (r.monster && r.monster.alive) {
            const monsterLabel = r.monster.is_boss ? '👹 BOSS' : '⚔️ 怪物';
            tooltipLines.push(\`\${monsterLabel} 战力: \${r.monster.power}\`);
          } else if (r.monster && !r.monster.alive) {
            tooltipLines.push('💀 已击败');
          }

          if (r.loot) {
            tooltipLines.push(\`💎 发现: \${r.loot.name || '未知物品'}\`);
            if (r.loot.power_bonus) tooltipLines.push(\`   (战力: +\${r.loot.power_bonus}, 价值: \${r.loot.value})\`);
          }
        } else {
          tooltipLines.push(\`[\${r.id}] 未知区域\`);
        }

        return {
          id: r.id,
          label,
          title: tooltipLines.join('\\n'), // vis-network 支持 \\n 换行
          color,
          shape: 'dot',
          size: isCurrent ? 22 : onPath ? 20 : 18,
          borderWidth: isCurrent ? 3 : onPath ? 2.5 : 1.5,
          allowed: r.visible || r.visited || isCurrent, // exposed for UI checks
        };
      });
>>>>
`
}

```

**修改说明：**

1. **新增 Tooltip 逻辑**：在 `transformMap` 遍历房间节点时，根据房间的可见性 (`visible/visited`)、怪物状态 (`monster`) 和战利品 (`loot`) 动态生成多行文本信息。
2. **绑定 `title` 属性**：将生成的文本（使用 `\n` 换行）赋值给节点的 `title` 属性。
3. **显示效果**：现在当你在游戏中将鼠标悬停在已探索的节点上时，会看到包含房间类型、怪物战力、战利品详情等信息的浮窗；未探索区域则显示“未知区域”。