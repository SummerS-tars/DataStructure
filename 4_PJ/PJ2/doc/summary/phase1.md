# PJ2 地下城与勇士 - Phase 1 总结与运行说明

本说明文档总结 Phase 1 的最终实现（含收尾改进），并提供一键构建与测试方式。

## 最终实现范围（Phase 1）

- 语言/标准：C++23，使用 STL 容器与算法。
- 模块与数据结构：
  - `Item`（`include/Item.h`）
    - 属性：`name`，`type`（Helmet/Armor/Boots/Weapon/MagicStone），`value`（价值）与 `power_bonus`（武力加成）。
    - 序列化辅助：`item_type_to_int` / `item_type_from_int`。
  - `Room`（`include/Room.h`）
    - 属性：`id`，`monster_power`（0 表示无怪），`is_boss`，`chest_items`（房间宝箱中物品列表）。
  - `DungeonMap`（`include/DungeonMap.h` + `src/DungeonMap.cpp`）
    - 邻接表图：`std::unordered_map<int, std::vector<int>>`。
    - 支持：`add_room`，`add_edge(u,v)`（无向去重），`neighbors(id)`，`get_room(id)`，`room_ids()`，`rooms()`。
    - 工具：`is_connected()`（BFS 连通性检查），`to_string()`（文本展示）。
    - 序列化：`save(path)` / `static load(path)`（自定义文本格式）。
  - `MapGenerator`（`include/MapGenerator.h` + `src/MapGenerator.cpp`）
    - 随机生成 `room_num`（默认 20）个房间，分配怪物武力与 0~3 个宝箱物品。
    - 生成主干树（随机生成树）保证图连通；再加 `extra_edges` 条随机边增加复杂度。
    - Boss = 怪物武力最大者；`DungeonMap::set_boss` 负责标记。
  - `main.cpp`（`src/main.cpp`）
    - 入口程序：生成地图并打印，输出连通性检查结果。

## 地图生成与收尾改进

1. 房间与怪物
   - `GenConfig` 可配置 `room_num`、`monster_min/max`、`seed`、`extra_edges`。
   - 每个房间分配 `monster_power` ∈ [`monster_min`, `monster_max`] 的均匀随机值。
2. 宝箱与物品
   - 每个房间随机生成 0~3 件物品；物品随机类型（五类），`value`∈[1,50]，`power_bonus`∈[0,20]。
3. 连通性保证
   - 先打乱节点顺序，按“新节点随机连接一个已在生成树中的旧节点”的方式形成主干树（spanning tree）。
   - 再额外添加 `extra_edges` 条随机边（忽略自环），之后邻接表去重。
4. Boss 判定
   - 遍历所有房间，`monster_power` 最大的房间标记为 Boss，保证 Boss 唯一。
5. 序列化（新增）
   - 文本格式：
     - 第一行：房间数 n
     - 接下来 n 行：`id monster is_boss chest_cnt [t value bonus name]*`
     - 下一行：无向边数 m（去重后 u<v）
     - 接下来 m 行：`u v`
     - 最后一行：`boss_id`
   - `save(path)` 写出上述格式；`load(path)` 读回并重建地图与 Boss 标记。

## CLI 运行说明

- 运行入口：`bin/pj2`（由 Makefile 生成）。
- 可选参数：`<room_num>`，小于 20 会自动提升到 20。
- 输出内容（示例项）：
  - 每个房间：`Room <id> [monster=<power>, BOSS?] chest_items=<count>` 与邻接列表 `-> ...`。
  - 最后两行：`BossRoom: <id>` 与 `Connected/NotConnected`。

## 一键构建与测试

- Makefile 目标：`debug`（默认）、`release`、`run`（透传 `ARGS`）、`test`（执行脚本 + 运行 C++ 最小单测）、`clean`、`help`。
- 最小测试脚本：`scripts/test_sample.sh`：
  - 构建可执行，运行一次并校验输出包含 `BossRoom:` 与 `Connected`。
  - 运行 `bin/test_phase1` 单测二进制。
- C++ 最小单测（二进制 `bin/test_phase1`，源码 `tests/test_phase1.cpp`）：
  - `ConnectivityAndBoss`：断言 `room_count>=20`、`is_connected==true`、Boss 唯一且为最大怪力。
  - `SaveAndLoadRoundtrip`：保存到文件并读回，断言房间数、连通性与 Boss ID 一致。

### 常用命令（在 `4_PJ/PJ2/` 目录下）

```bash
make                 # 等价于 make debug
make run ARGS="30"      # 生成 30 房间并运行
make test            # 运行最小测试脚本 + C++ 单测
make clean
```

## 阶段完成标识（Phase 1 完成）
- 地图自动生成且连通，Boss 唯一且为最大怪力。
- 文本展示完整邻接信息与房间属性。
- 提供保存/加载能力，便于复现实验。
- 一键构建、运行与最小测试可用。
