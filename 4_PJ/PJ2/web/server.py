#!/usr/bin/env python3
import tempfile
import subprocess
import json
import random
from pathlib import Path
from typing import List, Optional, Dict, Any
from datetime import datetime

from fastapi import FastAPI, HTTPException
from fastapi.responses import RedirectResponse, FileResponse
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

ROOT = Path(__file__).resolve().parents[1]
ENGINE = ROOT / "bin" / "engine"
BUILD = ROOT / "build"
BUILD.mkdir(parents=True, exist_ok=True)
STATE_PATH = BUILD / "state.json"

app = FastAPI(title="PJ2 Engine Wrapper")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
static_dir = ROOT / "web" / "static"
if static_dir.exists():
    # html=True 允许 /static/ 直接返回 index.html
    app.mount("/static", StaticFiles(directory=str(static_dir), html=True), name="static")

# Serve index.html at root for convenience; fallback to redirect if missing
@app.get("/")
def root():
    idx = static_dir / "index.html"
    if idx.exists():
        return FileResponse(str(idx))
    return RedirectResponse(url="/static/")

class GenReq(BaseModel):
    seed: int
    room_num: int
    extra_edges: int

class GridReq(BaseModel):
    width: int
    height: int
    seed: int

class ShortestReq(BaseModel):
    map_path: str
    start: int
    goal: int

class TopReq(BaseModel):
    map_path: str
    start: int
    goal: int
    k: int
    initial_power: int | None = None

class ExploreReq(BaseModel):
    map_path: str
    start: int
    goal: int
    initial_power: int

class BossReq(BaseModel):
    map_path: str


class MoveReq(BaseModel):
    target_room_id: int
    map_path: Optional[str] = None


class BossTickReq(BaseModel):
    map_path: Optional[str] = None


# --- Simple state management ---
def load_map_state() -> Dict[str, Any]:
    if STATE_PATH.exists():
        try:
            return json.loads(STATE_PATH.read_text())
        except Exception:
            pass
    # default state
    return {
        "map_path": str(BUILD / "map.txt"),
        "player_pos": 0,
        "power": 10,
        "total_value": 0,
        "logs": [],
    }


def save_map_state(state: Dict[str, Any]) -> None:
    STATE_PATH.write_text(json.dumps(state, ensure_ascii=False, indent=2))


def run_engine(args: List[str]) -> str:
    try:
        cp = subprocess.run([str(ENGINE), *args], check=True, capture_output=True, text=True)
        return cp.stdout.strip()
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=400, detail=e.stderr.strip() or "engine error")

@app.post("/generate")
def generate(req: GenReq):
    out_path = str(BUILD / "map.txt")
    out = run_engine(["generate", str(req.seed), str(req.room_num), str(req.extra_edges), out_path])
    state = load_map_state()
    state.update({
        "map_path": out_path,
        "player_pos": 0,
        "power": 10,
        "total_value": 0,
        "logs": [],
    })
    save_map_state(state)
    return {"map_path": out_path, "raw": out, "state": state}

@app.post("/generate_grid")
def generate_grid(req: GridReq):
    out_path = str(BUILD / "map_grid.txt")
    out = run_engine(["generate_grid", str(req.width), str(req.height), str(req.seed), out_path])
    state = load_map_state()
    state.update({
        "map_path": out_path,
        "player_pos": 0,
        "power": 10,
        "total_value": 0,
        "logs": [],
    })
    save_map_state(state)
    return {"map_path": out_path, "raw": out, "state": state}

@app.get("/map_text")
def map_text(path: str | None = None):
    """Return textual map file content for quick viewing."""
    p = Path(path) if path else (BUILD / "map.txt")
    if not p.exists():
        raise HTTPException(status_code=404, detail="map file not found")
    return {"path": str(p), "text": p.read_text(encoding="utf-8", errors="ignore")}

@app.post("/shortest")
def shortest(req: ShortestReq):
    out = run_engine(["shortest", req.map_path, str(req.start), str(req.goal)])
    # PATH <a> <b> ...
    parts = out.split()
    if not parts or parts[0] != "PATH":
        raise HTTPException(status_code=400, detail="bad engine output")
    path = [int(x) for x in parts[1:]]
    return {"path": path}

@app.post("/top")
def top(req: TopReq):
    args = ["top", req.map_path, str(req.start), str(req.goal), str(req.k)]
    if req.initial_power is not None:
        args.append(str(req.initial_power))
    out = run_engine(args)
    # multiple lines: PATH <rank> val=<v> pow=<p> : <ids...>
    results = []
    for line in out.splitlines():
        line = line.strip()
        if not line:
            continue
        # naive parse
        if not line.startswith("PATH "):
            continue
        # PATH 1 val=10 pow=2 : 0 3 5
        try:
            head, ids = line.split(":", 1)
            head_parts = head.split()
            rank = int(head_parts[1])
            val = int(head_parts[2].split("=")[1])
            pw = int(head_parts[3].split("=")[1])
            path = [int(x) for x in ids.strip().split()] if ids.strip() else []
            results.append({"rank": rank, "val": val, "pow": pw, "path": path})
        except Exception:
            continue
    return {"paths": results}

@app.post("/explore")
def explore(req: ExploreReq):
    out = run_engine(["explore_shortest", req.map_path, str(req.start), str(req.goal), str(req.initial_power)])
    # EXPLORE success=1 power_end=XX total_value=YY progressed=a,b,c
    parts = {kv.split("=")[0]: kv.split("=")[1] for kv in out.split() if "=" in kv}
    success = parts.get("success", "0") == "1"
    power_end = int(parts.get("power_end", "0"))
    total_value = int(parts.get("total_value", "0"))
    progressed = parts.get("progressed", "")
    path = [int(x) for x in progressed.split(",") if x]
    return {"success": success, "power_end": power_end, "total_value": total_value, "progressed": path}

class NeighReq(BaseModel):
    map_path: str
    room_id: int

@app.post("/neighbors")
def neighbors(req: NeighReq):
    out = run_engine(["neighbors", req.map_path, str(req.room_id)])
    parts = out.split()
    if not parts or parts[0] != "NEIGH":
        raise HTTPException(status_code=400, detail="bad engine output")
    ids = [int(x) for x in parts[1:]]
    return {"neighbors": ids}

class StepReq(BaseModel):
    map_path: str
    player_pos: int
    next_room: int
    power: int
    out_map_path: str | None = None

@app.post("/step_move")
def step_move(req: StepReq):
    out_map = req.out_map_path or str(BUILD / "map_step.txt")
    out = run_engine(["step_move", req.map_path, out_map, str(req.player_pos), str(req.next_room), str(req.power)])
    # STATE pos=.. power_end=.. total_value=.. moved=.. over=.. map=..
    parts = {kv.split("=")[0]: kv.split("=")[1] for kv in out.split() if "=" in kv}
    return {
        "pos": int(parts.get("pos", "-1")),
        "power_end": int(parts.get("power_end", "0")),
        "total_value": int(parts.get("total_value", "0")),
        "moved": parts.get("moved", "0") == "1",
        "over": parts.get("over", "0") == "1",
        "map_path": parts.get("map", out_map),
    }

@app.post("/bossmove")
def bossmove(req: BossReq):
    out_path = str(BUILD / "map_after_boss.txt")
    out = run_engine(["bossmove", req.map_path, out_path])
    # BOSS <id> <path>
    parts = out.split()
    if not parts or parts[0] != "BOSS":
        raise HTTPException(status_code=400, detail="bad engine output")
    return {"map_path": out_path, "boss_id": int(parts[1])}


# --- Stateful endpoints ---

def _append_log(state: Dict[str, Any], event: Dict[str, Any]):
    logs = state.get("logs", [])
    event["ts"] = datetime.utcnow().isoformat() + "Z"
    logs.append(event)
    state["logs"] = logs


@app.post("/action/move")
def action_move(req: MoveReq):
    state = load_map_state()
    map_path = req.map_path or state.get("map_path")
    if not map_path:
        raise HTTPException(status_code=400, detail="no map loaded")
    player_pos = int(state.get("player_pos", 0))
    power = int(state.get("power", 10))

    # use engine step_move to validate adjacency + battle + loot + boss check
    out_map = str(BUILD / "map_step.txt")
    out = run_engine(["step_move", map_path, out_map, str(player_pos), str(req.target_room_id), str(power)])
    parts = {kv.split("=")[0]: kv.split("=")[1] for kv in out.split() if "=" in kv}
    moved = parts.get("moved", "0") == "1"
    over = parts.get("over", "0") == "1"
    new_pos = int(parts.get("pos", player_pos))
    new_power = int(parts.get("power_end", power))
    total_value = int(parts.get("total_value", state.get("total_value", 0)))
    new_map_path = parts.get("map", out_map)

    if moved:
        state.update({
            "player_pos": new_pos,
            "power": new_power,
            "total_value": total_value,
            "map_path": new_map_path,
        })
        _append_log(state, {"action": "move", "from": player_pos, "to": new_pos, "power": new_power, "value": total_value, "over": over})
        save_map_state(state)
    status = "VICTORY" if over else ("OK" if moved else "MOVE_FAIL")
    return {
        "player_status": {
            "pos": state.get("player_pos", new_pos),
            "power": state.get("power", new_power),
            "total_value": state.get("total_value", total_value),
        },
        "map_updates": {
            "map_path": new_map_path,
            "moved": moved,
            "over": over,
        },
        "logs": state.get("logs", []),
        "status": status,
    }


@app.post("/boss/tick")
def boss_tick(req: BossTickReq):
    state = load_map_state()
    map_path = req.map_path or state.get("map_path")
    if not map_path:
        raise HTTPException(status_code=400, detail="no map loaded")
    out_path = str(BUILD / "map_after_boss.txt")
    out = run_engine(["bossmove", map_path, out_path])
    parts = out.split()
    if not parts or parts[0] != "BOSS":
        raise HTTPException(status_code=400, detail="bad engine output")
    boss_id = int(parts[1])

    # recompute shortest path from current player pos to boss
    player_pos = int(state.get("player_pos", 0))
    sp_raw = run_engine(["shortest", out_path, str(player_pos), str(boss_id)])
    sp_parts = sp_raw.split()
    path = [int(x) for x in sp_parts[1:]] if sp_parts and sp_parts[0] == "PATH" else []

    state["map_path"] = out_path
    state["boss_id"] = boss_id
    save_map_state(state)

    _append_log(state, {"action": "boss_move", "boss_id": boss_id, "path_len": len(path)})

    return {
        "boss_id": boss_id,
        "path": path,
        "map_path": out_path,
        "player_pos": player_pos,
        "status": "OK",
    }

if __name__ == "__main__":
    import uvicorn
    print("[INFO] Server starting on http://0.0.0.0:8090")
    print("[INFO] Static at /static if web/static exists")
    uvicorn.run(app, host="0.0.0.0", port=8090)
