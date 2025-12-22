"""Pydantic models placeholder for Phase 0.
Add game entities (PlayerState, Room, etc.) in later phases per backend_ref.md.
"""
from pydantic import BaseModel


class HealthResponse(BaseModel):
    status: str = "ok"
    message: str = "backend scaffold ready"
