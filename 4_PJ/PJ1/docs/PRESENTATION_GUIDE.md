# 迷宫路径分析系统 - 课堂讲解文档

## 一、数据结构设计 (10分)

### 1.1 核心数据结构及其选择理由

#### 📚 Stack - 撤销功能
```cpp
std::stack<std::pair<int, int>> move_history;  // Player类
```
**为什么用Stack？**
- ✅ **LIFO特性**：撤销操作天然符合"后进先出"
- ✅ **O(1)复杂度**：压栈和出栈都是常数时间
- ✅ **内存高效**：只存储必要的位置信息

**实际应用**：
- 每次移动：`move_history.push({new_x, new_y})`
- 撤销时：`move_history.pop()`，恢复到栈顶位置

---

#### 📋 Vector - 路径记录与回放
```cpp
std::vector<char> command_history;           // Player类：命令序列
std::vector<std::string> maze_grid;          // Maze类：地图存储
std::vector<std::pair<int,int>> path;        // 完整路径
```
**为什么用Vector？**
- ✅ **顺序存储**：保持命令执行顺序（回放功能需要）
- ✅ **随机访问**：O(1)时间访问任意位置
- ✅ **动态扩展**：路径长度不固定

**实际应用**：
- 命令记录：`command_history.push_back(cmd)`
- 路径回放：`for(char cmd : command_history) { execute(cmd); }`
- 地图存储：`maze_grid[y][x]` 访问任意格子

---

#### 📝 String - 灵活字符串处理
```cpp
std::string maze_file;                       // 文件路径
std::string input;                           // 用户输入
std::vector<std::string> maze_grid;          // 每行是一个string
```
**为什么用String？**
- ✅ **动态长度**：迷宫每行宽度可能不同
- ✅ **字符串操作**：支持substr、find等方法
- ✅ **内存管理**：自动管理，避免手动new/delete

**实际应用**：
- 地图每行：`maze_grid[y]` 返回一个完整行字符串
- 字符访问：`maze_grid[y][x]` 访问单个格子
- 文件名处理：`maze_file.substr(maze_file.find_last_of('/') + 1)`

---

#### 🔗 Pair - 坐标表示
```cpp
std::stack<std::pair<int, int>> move_history;      // 位置栈
std::vector<std::pair<int, int>> path;             // 路径序列
```
**为什么用Pair？**
- ✅ **语义清晰**：`pair<int, int>` 明确表示(x,y)坐标
- ✅ **标准库支持**：无需自定义struct
- ✅ **轻量级**：只是两个int的简单组合

**实际应用**：
- 保存位置：`move_history.push({x, y})`
- 访问坐标：`auto [x, y] = move_history.top()` (C++17)
- 路径遍历：`for(auto [x, y] : path) { ... }`

**为什么不用struct？**
- Pair足够简单，不需要额外命名
- 如果需要更多字段（如方向、时间戳），才考虑struct

---

### 1.2 数据结构对比

| 需求 | 可选方案 | 最终选择 | 原因 |
|------|---------|---------|------|
| 撤销功能 | Stack / Vector | **Stack** | LIFO完美匹配，代码更简洁 |
| 命令历史 | Vector / List / Deque | **Vector** | 需要顺序遍历，连续内存更快 |
| 地图存储 | 2D数组 / Vector<string> | **Vector<string>** | 动态大小，每行独立管理 |
| 坐标表示 | struct / pair / array | **pair<int,int>** | 简单场景，标准库直接支持 |
| 文件路径 | char* / string | **string** | 自动内存管理，丰富操作 |

---

### 1.3 数据结构完整清单

项目共使用 **5种** 标准库数据结构：

| 数据结构 | 使用场景 | 数量 | 复杂度优势 |
|---------|---------|------|-----------|
| **Stack** | 撤销功能 | 2个（每个玩家） | O(1) push/pop |
| **Vector** | 命令记录、路径、地图行 | 10+ | O(1) 追加/访问 |
| **String** | 地图行、文件名、用户输入 | 20+ | 动态管理 |
| **Pair** | 坐标(x,y) | Stack/Vector元素 | 轻量级组合 |
| **Array** | 双玩家数组 | `Player players_[2]` | 固定大小 |

**满足要求**：✅ 至少2种不同的数据结构（实际使用5种）

---

## 二、系统架构设计

### 2.1 模块划分（MVC模式）

```
┌─────────────────────────────────────┐
│         MenuManager (View)          │  ← 用户界面
│  - 主菜单                            │
│  - 文件选择                          │
│  - 模式选择                          │
└──────────┬──────────────────────────┘
           │
           ▼
┌─────────────────────────────────────┐
│         Game (Controller)           │  ← 游戏逻辑
│  - 主循环                            │
│  - 命令解析                          │
│  - 状态管理                          │
└──────────┬──────────────────────────┘
           │
     ┌─────┴─────┐
     ▼           ▼
┌─────────┐  ┌──────────┐
│  Maze   │  │  Player  │              ← 数据模型
│ (Model) │  │ (Model)  │
└─────────┘  └──────────┘
```

**优点**：
- 职责分离，易于维护
- 每个类专注单一功能
- Game.cpp从779行优化到450行

---

### 2.2 类设计与职责

| 类名 | 数据结构 | 主要职责 |
|-----|---------|---------|
| **Player** | Stack + Vector | 位置管理、移动历史、命令记录 |
| **Maze** | Vector<string> | 地图加载、显示、碰撞检测 |
| **Game** | - | 游戏循环、命令解析、胜利检测 |
| **SaveManager** | 文件I/O | 多槽位存档、元数据管理 |
| **MenuManager** | - | 用户界面、输入验证 |
| **MazeGenerator** | DFS Stack | 迷宫生成算法 |

---

## 三、关键功能实现

### 3.1 撤销功能 (Stack应用)

```cpp
// 移动时压栈
void Player::move_to(int x, int y, char cmd) {
    move_history_.push({x_, y_});  // 保存当前位置
    x_ = x;
    y_ = y;
    command_history_.push_back(cmd);
}

// 撤销时出栈
bool Player::undo() {
    if (move_history_.empty()) return false;
    
    auto prev = move_history_.top();  // O(1)获取
    move_history_.pop();              // O(1)删除
    x_ = prev.first;
    y_ = prev.second;
    command_history_.pop_back();
    return true;
}
```

**复杂度分析**：
- 时间：O(1)
- 空间：O(n)，n为移动步数

---

### 3.2 路径回放 (Vector应用)

```cpp
void Game::replay_moves() {
    vector<char> commands = player_.get_commands();  // 复制命令
    
    // 重置到起点
    player_ = Player(start_x, start_y);
    
    // 顺序执行所有命令
    for (char cmd : commands) {  // O(n)遍历
        process_command(cmd);
        display();
        sleep(0.5);  // 动画效果
    }
}
```

**为什么不用Stack？**
- 回放需要**从头到尾**执行
- Stack是LIFO，会导致**倒序**执行
- Vector保持**插入顺序**

---

### 3.3 双人模式 (多实例管理)

```cpp
class Game {
    Player players_[2];  // 两个独立Player实例
    int current_player_;  // 当前回合玩家
    bool multiplayer_mode_;
};

// 每个玩家独立的历史栈
// Player 1: Stack{(1,1), (1,2), (1,3)}
// Player 2: Stack{(1,1), (2,1), (3,1)}
```

**设计亮点**：
- 每个玩家独立的Stack和Vector
- 回合制确保公平（一次一步）
- 独立统计步数

---

## 四、优化措施

### 4.1 代码重构

**问题**：Game.cpp 779行，菜单、存档逻辑混杂

**优化**：
```
Game.cpp (779行)
    ↓ 提取
MenuManager.cpp (320行)  ← 菜单逻辑
SaveManager.cpp (180行)  ← 存档逻辑
Game.cpp (450行)         ← 核心游戏逻辑
```

**效果**：
- ✅ 代码可读性提升
- ✅ 维护成本降低
- ✅ 职责更清晰

---

### 4.2 输入验证优化

**问题**：输入非数字导致无限循环

```cpp
// 修复前
cin >> choice;  // 输入"abc"会导致cin.fail()

// 修复后
cin >> choice;
if (cin.fail()) {
    cin.clear();  // 清除错误标志
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input!\n";
    continue;
}
```

---

### 4.3 显示优化

**问题**：UTF-8字符(·)显示为乱码(�)

**优化**：
```
· (UTF-8) → ' ' (ASCII)
```

**结果**：跨平台兼容性提升

---

## 五、演示流程建议

### 5.1 基础功能演示 (3分钟)

1. **加载迷宫** → 展示Vector存储
2. **移动角色** → 展示命令解析
3. **撤销操作** → **重点展示Stack**
4. **打印路径** → 展示Vector遍历
5. **到达终点** → 展示胜利检测

### 5.2 扩展功能演示 (2分钟)

1. **路径回放** → 展示Vector顺序执行
2. **保存/加载** → 展示多槽位系统
3. **生成迷宫** → 展示DFS算法
4. **双人模式** → 展示多实例管理

### 5.3 讲解重点 (2分钟)

**数据结构选择（5种）**：
1. **Stack** - "我选择Stack实现撤销，因为LIFO特性完美匹配，O(1)时间复杂度"
2. **Vector** - "Vector用于路径记录和命令历史，因为需要保持顺序且支持随机访问"
3. **String** - "String用于迷宫每一行，动态长度适应不同宽度的迷宫"
4. **Pair** - "Pair<int,int>表示坐标，语义清晰且轻量级"
5. **Array** - "固定大小数组存储两个玩家实例，简单高效"

**为什么超过2种？**
- "项目要求至少2种，我使用了5种标准库数据结构"
- "每种数据结构都有其最适合的场景"
- "体现对STL的深入理解和灵活运用"

**优化亮点**：
- "通过模块化重构，将779行代码精简到450行"
- "双人模式使用独立的Stack实例，确保每个玩家独立追踪"
- "String配合Vector实现灵活的二维迷宫存储"

---

## 六、总结

### 得分点总结

| 评分项 | 分数 | 实现情况 |
|--------|------|---------|
| 基础功能 | 50 | ✅ 5/5 完成 |
| 数据结构设计 | 10 | ✅ Stack+Vector合理应用 |
| 按时提交检查 | 10 | ✅ 11月16日前 |
| 扩展功能 | 30 | ✅ 4个扩展(25分) |
| **总计** | **100** | **85+分** |

### 核心竞争力

1. **Stack实现撤销** - O(1)高效操作
2. **Vector实现回放** - 保持命令顺序
3. **模块化设计** - 代码重构优化
4. **双人竞速** - 多实例独立管理

---

## 附：快速问答准备

**Q: 为什么用Stack而不是Vector实现撤销？**
A: Stack的LIFO特性完美匹配撤销逻辑，push/pop都是O(1)。虽然Vector也能实现，但Stack语义更清晰，代码更简洁。

**Q: 如何保证两个玩家的历史不冲突？**
A: 每个Player对象有独立的Stack和Vector成员变量，在Game类中维护两个Player实例（players_[0]和players_[1]），互不干扰。

**Q: 路径回放为什么不用Stack？**
A: 回放需要从第一个命令开始按顺序执行。Stack是LIFO会倒序执行，而Vector保持插入顺序，更适合顺序遍历。

**Q: 最大的优化是什么？**
A: 代码重构。将Game.cpp从779行拆分为MenuManager和SaveManager，降低到450行，提高了可维护性和可读性。
