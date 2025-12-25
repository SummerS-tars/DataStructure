# Phase 0: 项目脚手架与基础架构搭建

## 1. 目标概述
完成前后端开发环境的初始化，建立符合文档要求的目录结构，并验证前后端连通性。这是开发的基石。

## 2. 详细任务清单

### 2.1. 后端环境搭建 (Backend)
* [ ] **初始化环境**: 创建 `pj2_backend` 文件夹，配置 Python 虚拟环境 (venv)。
* [ ] **安装依赖**: 安装 `fastapi`, `uvicorn`, `pydantic`.
* [ ] **构建目录结构**: 按照 `backend_ref.md` 中的 "项目目录结构建议" 创建文件夹 (`app/core`, `app/models`, `data` 等).
* [ ] **Hello World**: 编写 `app/main.py`，创建一个基础 GET 路由 `/` 返回 `{"status": "ok"}`，确保服务可启动。

### 2.2. 前端环境搭建 (Frontend)
* [ ] **初始化项目**: 使用 Vite 创建 Vue 3 项目 `pj2_frontend`.
* [ ] **安装依赖**: 安装 `pinia`, `axios`, `vis-network` (用于地图), `vue-router` (可选).
* [ ] **构建目录结构**: 按照 `frontend_ref.md` 创建 `src/api`, `src/stores`, `src/components/game` 等文件夹.
* [ ] **配置代理**: 在 `vite.config.js` 中配置 Proxy，解决开发环境跨域问题，指向后端 `http://localhost:8000`。

## 3. 成功验收标准 (Definition of Done)
1.  **后端**: 运行 `python run.py` (或 uvicorn) 无报错，浏览器访问 `http://localhost:8000/docs` 能看到自动生成的 Swagger 文档。
2.  **前端**: 运行 `npm run dev` 能看到 Vue 欢迎页面。
3.  **连通性**: 前端代码中能通过 Axios 请求到后端的测试接口并在控制台打印结果。