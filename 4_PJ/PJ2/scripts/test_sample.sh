#!/usr/bin/env bash
set -Eeuo pipefail
cd "$(dirname "$0")/.."

# Build debug
make -s debug > /dev/null

# Run with default (20 rooms) unless ARGS provided
ARGS=${ARGS:-}
OUT=$(mktemp)
trap 'rm -f "$OUT"' EXIT

if [[ -n "$ARGS" ]]; then
  bin/pj2 $ARGS | tee "$OUT"
else
  bin/pj2 | tee "$OUT"
fi

# Simple checks: must contain BossRoom and Connected
if ! grep -q "BossRoom:" "$OUT"; then
  echo "[FAIL] Output missing BossRoom line" >&2
  exit 1
fi
if ! grep -q "Connected" "$OUT"; then
  echo "[FAIL] Output missing Connected line" >&2
  exit 1
fi

echo "[PASS] Map generated with Boss and is connected."

# Run minimal C++ tests
if bin/test_phase1; then
  echo "[PASS] test_phase1 binary passed."
else
  echo "[FAIL] test_phase1 binary failed." >&2
  exit 1
fi

if bin/test_phase2; then
  echo "[PASS] test_phase2 binary passed."
else
  echo "[FAIL] test_phase2 binary failed." >&2
  exit 1
fi

if bin/test_phase3; then
  echo "[PASS] test_phase3 binary passed."
else
  echo "[FAIL] test_phase3 binary failed." >&2
  exit 1
fi

if bin/test_phase4; then
  echo "[PASS] test_phase4 binary passed."
else
  echo "[FAIL] test_phase4 binary failed." >&2
  exit 1
fi
