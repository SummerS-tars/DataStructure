# Phase 0: 总体项目概览与设计锚定 (Project Overview & Design Anchors)

## 目标 (Goal)

定义项目的技术栈、编程规范、基础设计框架和测试策略，确保所有开发人员（即你自己）在后续阶段能保持一致性和高效率。

## 技术栈与规范 (Tech Stack & Conventions)

| 方面 | 规范与选择 | 描述 |
| :--- | :--- | :--- |
| **编程语言** | **C++ (推荐 C++11/14/17 标准)** | 利用 C++ 的面向对象特性实现数据结构封装，使用 STL 容器（如 `std::vector`, `std::map`, `std::queue`, `std::stack`）作为实现基础。 |
| **设计范式** | **面向对象编程 (OOP)** | 将世界元素（Room, Item, Player）抽象为类，将核心逻辑（Map, PathFinder, LootTree）作为独立的服务或管理器类。 |
| **文件结构** | **Header/Implementation 分离** | 关键类使用 `.h` (声明) 和 `.cpp` (定义) 文件分离。所有类应有清晰的公共（Public）接口。 |
| **命名规范** | **统一风格** | **类名/结构体名**: `CamelCase` (如 `DungeonMap`, `AdventureLog`)。 **方法名/变量名**: `snake_case` (如 `find_shortest_path`, `room_id`)。 **常量**: `ALL_CAPS` (如 `MAX_ROOMS_COUNT`)。 |
| **依赖管理** | **标准库优先** | 优先使用 C++ 标准库 (`<iostream>`, `<vector>`, `<algorithm>`)，不引入第三方库，以保持项目简洁。 |

## 基础设计框架 (Fundamental Design Framework)

我们将采用**组件化/服务化**的设计模式，将数据与逻辑分离，便于单元测试和未来的重构。

| 抽象组件/服务 | 职责与关键接口 | 锚定数据结构 |
| :--- | :--- | :--- |
| **Data Entities** | 核心数据对象，仅负责数据封装。 | `Room`, `Item`, `LogEntry` |
| **`DungeonMap`** | 负责地图的创建、存储、查询。接口: `get_room(id)`, `add_edge(u, v)`, `is_connected()`. | **邻接表 (Graph)** |
| **`PathFinder`** | 负责所有寻路算法的实现。接口: `find_shortest_path()`, `find_all_paths_sorted()`. | **BFS/DFS 算法** |
| **`LootManager`** | 负责玩家战利品的管理和分类。接口: `add_item(item)`, `display_loot()`. | **多叉树 (Tree)** |
| **`GameSystem`** | 程序的入口和主循环，负责调度所有服务，处理用户输入。 | 组合模式 (Composition) |

## 测试框架锚定 (Testing Framework Anchor)

本项目要求实现单元测试来验证数据结构和算法的正确性。

* **测试策略**：采用**单元测试 (Unit Testing)** 为主，验证每个模块（如 `DungeonMap`, `PathFinder`）的独立功能。
* **实现方式**：
    1. **基础**：使用 C++ 原生 `assert()` 函数或简单的自定义测试宏。
    2. **推荐**：若时间允许，使用 Google Test / Catch2 等成熟框架，能提供更清晰的测试报告和 Setup/Teardown 机制。
* **测试重点**：数据结构的边界情况（空图、满图、单节点树）和核心算法的正确性（BFS最短路径、DFS路径遍历的完备性）。

## 任务：构建脚本与测试脚本规范（Make + sh）

目的：统一 PJ2 的构建与测试脚本约定，保持与工作区配置一致，便于后续一键构建与评测。

| 方面 | 约定 | 备注 |
| :--- | :--- | :--- |
| 构建工具/标准 | GNU Make + g++ C++23，`-Wall` | 与 VS Code 任务参数对齐 |
| 目录结构 | `src/`, `include/`, `build/`, `bin/`, `scripts/`, `tests/data/` | 可执行默认 `bin/pj2` |
| 构建目标 | `all`(=debug), `debug`(-g -O0), `release`(-O2 -DNDEBUG), `run`(透传 `ARGS`), `test`(调用 `scripts/`), `clean`, `help` | 简洁可读 |
| 编译规则 | `src/*.cpp` → `build/*.o` → `bin/pj2`；`-Iinclude`；模式规则 | 保持可增量构建 |
| 脚本规范 | bash；shebang；`set -Eeuo pipefail`；`-h/--help`；`mktemp`+`trap` 清理；退出码 0/非 0 | 允许 `BIN` 覆盖默认可执行 |

阶段完成标识：

* 在 PJ2 根目录可成功执行：`make debug`/`make release`/`make test`/`make clean`；`make run ARGS="..."` 可透传参数。
* 在 `scripts/` 下有最小测试脚本（命名 `test_*.sh`），可调用默认可执行 `bin/pj2` 并输出清晰结果。
* 在 `README.md` 或本文件保留一句“如何构建与运行测试”的说明（无需展开样板代码）。

下一步（可选）：在 `4_PJ/PJ2/` 根目录添加最小 `Makefile` 与 `scripts/test_sample.sh` 样板以落地上述约定。

## 阶段完成标识 (Phase Completion Indicator)

所有关键类的头文件（.h）已定义，编程规范文档（或注释）已创建，且开发者对整体设计框架表示了解。

**请确认以上项目概览与设计锚定无误。如果准备好了，请回复“确认”，我们将正式开始 Phase 1 的开发。**  
