# Consumable Use API (Backend -> Frontend Guide)

## Endpoint

`POST /use`

## Purpose

Consume an item in the player's inventory (e.g., HP potion). Applies the effect, removes one copy from inventory, recalculates player stats, persists state.

## Request

- **Query/JSON param**: `item_id` (int) — ID of the item in inventory to consume.

Example (form/query):

```http
POST /use?item_id=123
```

## Success Response

- **200 OK**
- Body: `GameResponse` (same shape as /move and /equip)
  - `player.hp` updated after effect (capped by `max_hp`)
  - `player.inventory` has the item removed
  - `logs` appended with a human-readable message
  - `status` remains `playing` unless already win/dead

## Error Responses

- **400** if item not found in inventory
- **400** if item type is not consumable

## Effects (current implementation)

- **Health Potion** (`ItemType.POTION`): restore 20 HP (capped at max_hp). You can tune per-item value later (see `description`/`value`).

## Persistence

- After a successful use, dungeon state and user save are automatically persisted, same as /move and /equip.

## Frontend Notes

- Call this endpoint when user clicks “Use” on a consumable in the inventory list.
- After response, refresh local state from returned `GameResponse` to reflect HP and inventory changes.
- If multiple potions share the same ID, they are treated as unique items; current backend removes the first match only (no stacking yet).
- To start a fresh run, call `/init` with `resume=false`.
