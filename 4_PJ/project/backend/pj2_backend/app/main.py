from fastapi import FastAPI, HTTPException

from app.core.game_engine import GameEngine
from app.core.pathfinder import PathFinder
from app.models import GameResponse, HealthResponse

app = FastAPI(title="PJ2 Backend", version="0.3.0")

engine = GameEngine()
pathfinder = PathFinder()


@app.get("/", response_model=HealthResponse)
def read_root() -> HealthResponse:
    """Phase 0 health check endpoint."""
    return HealthResponse()


@app.post("/init", response_model=GameResponse)
def init_game(difficulty: str = "easy") -> GameResponse:
    """Initialize a new dungeon map based on difficulty factor."""

    factor_map = {
        "easy": 1.0,
        "normal": 1.5,
        "hard": 2.0,
        "abyss": 3.0,
    }
    if difficulty not in factor_map:
        raise HTTPException(status_code=400, detail="Invalid difficulty. Use easy/normal/hard/abyss.")

    factor = factor_map[difficulty]
    return engine.new_game(factor)


@app.post("/move", response_model=GameResponse)
def move_player(target_room_id: int) -> GameResponse:
    """Process a turn: move to neighbor, combat, loot, boss movement, fog update."""
    try:
        return engine.process_turn(target_room_id)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))


@app.post("/equip", response_model=GameResponse)
def equip_item(item_id: int) -> GameResponse:
    try:
        return engine.player_equip(item_id)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))


@app.get("/guide/shortest")
def get_shortest_path():
    if not engine.state:
        raise HTTPException(status_code=400, detail="Game not initialized")
    rooms = engine.state.map_view.rooms
    boss_id = engine._find_boss_room_id()
    path = pathfinder.get_shortest_path(rooms, engine.state.player.current_room_id, boss_id)
    return {"path": path}


@app.get("/guide/best_loot")
def get_best_loot_path():
    if not engine.state:
        raise HTTPException(status_code=400, detail="Game not initialized")
    rooms = engine.state.map_view.rooms
    boss_id = engine._find_boss_room_id()
    paths = pathfinder.get_top_k_value_paths(rooms, engine.state.player.current_room_id, boss_id)
    return {"paths": paths}
