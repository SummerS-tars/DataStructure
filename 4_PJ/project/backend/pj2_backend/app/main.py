from fastapi import FastAPI, HTTPException

from app.core.game_engine import GameEngine
from app.core.pathfinder import PathFinder
from app.models import GameResponse, HealthResponse
from app.utils.persistence import load_dungeon_state, load_user_save

app = FastAPI(title="PJ2 Backend", version="0.3.0")

engine = GameEngine()
pathfinder = PathFinder()


@app.get("/", response_model=HealthResponse)
def read_root() -> HealthResponse:
    """Phase 0 health check endpoint."""
    return HealthResponse()


@app.post("/init", response_model=GameResponse)
def init_game(difficulty: str = "easy", resume: bool = True) -> GameResponse:
    """Initialize a new dungeon map or resume previous run based on difficulty factor."""

    factor_map = {
        "easy": 1.0,
        "normal": 1.5,
        "hard": 2.0,
        "abyss": 3.0,
    }
    if difficulty not in factor_map:
        raise HTTPException(status_code=400, detail="Invalid difficulty. Use easy/normal/hard/abyss.")

    factor = factor_map[difficulty]
    return engine.start_game(factor, resume=resume)


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


@app.post("/use", response_model=GameResponse)
def use_consumable(item_id: int) -> GameResponse:
    try:
        return engine.use_consumable(item_id)
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


@app.get("/collection")
def get_collection_tree():
    """Expose persistent collection tree for lobby view."""
    return {
        "collection_tree": engine.loot_manager.to_dict(),
    }


@app.get("/save/dungeon")
def get_dungeon_save():
    data = load_dungeon_state()
    if not data:
        raise HTTPException(status_code=404, detail="Dungeon save not found")
    return data


@app.get("/save/user")
def get_user_save():
    data = load_user_save()
    if not data:
        raise HTTPException(status_code=404, detail="User save not found")
    return data
