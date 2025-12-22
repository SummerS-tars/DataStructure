from fastapi import FastAPI, HTTPException

from app.core.map_gen import MapGenerator
from app.models import (
    EquipmentSlots,
    GameResponse,
    HealthResponse,
    MapStructure,
    PlayerState,
)

app = FastAPI(title="PJ2 Backend", version="0.2.0")

map_generator = MapGenerator()


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
    map_view: MapStructure = map_generator.generate(factor)

    player = PlayerState(
        current_room_id=0,
        hp=100,
        max_hp=100,
        base_power=10,
        total_power=10,
        equipment=EquipmentSlots(),
        inventory=[],
    )

    logs = [f"Game initialized with difficulty '{difficulty}' (factor={factor})."]

    return GameResponse(player=player, map_view=map_view, logs=logs, status="playing")
