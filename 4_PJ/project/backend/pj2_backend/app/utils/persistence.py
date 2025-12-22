"""Persistence helpers for dungeon and user save data.

Files:
- data/dungeon_state.json: current dungeon run (map, player state, logs, turn counter, difficulty).
- data/user_save.json: persistent collection across runs.

All helpers are small and synchronous for simplicity.
"""
from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, Optional

BASE_DIR = Path(__file__).resolve().parents[2]
DATA_DIR = BASE_DIR / "data"
DUNGEON_FILE = DATA_DIR / "dungeon_state.json"
USER_FILE = DATA_DIR / "user_save.json"


def _ensure_data_dir() -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)


def save_dungeon_state(payload: Dict[str, Any]) -> None:
    """Persist current dungeon run to disk."""
    _ensure_data_dir()
    DUNGEON_FILE.write_text(json.dumps(payload, indent=2, default=str), encoding="utf-8")


def load_dungeon_state() -> Optional[Dict[str, Any]]:
    if not DUNGEON_FILE.exists():
        return None
    try:
        return json.loads(DUNGEON_FILE.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return None


def clear_dungeon_state() -> None:
    if DUNGEON_FILE.exists():
        DUNGEON_FILE.unlink()


def save_user_save(payload: Dict[str, Any]) -> None:
    """Persist global user save (collection tree, optional stats)."""
    _ensure_data_dir()
    USER_FILE.write_text(json.dumps(payload, indent=2, default=str), encoding="utf-8")


def load_user_save() -> Optional[Dict[str, Any]]:
    if not USER_FILE.exists():
        return None
    try:
        return json.loads(USER_FILE.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return None


def clear_user_save() -> None:
    if USER_FILE.exists():
        USER_FILE.unlink()
