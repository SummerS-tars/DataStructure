from fastapi import FastAPI
from .models import HealthResponse

app = FastAPI(title="PJ2 Backend", version="0.1.0")


@app.get("/", response_model=HealthResponse)
def read_root() -> HealthResponse:
    """Phase 0 health check endpoint."""
    return HealthResponse()
