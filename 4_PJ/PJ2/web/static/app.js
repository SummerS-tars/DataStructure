async function genMap() {
  const seed = +document.getElementById('seed').value;
  const room_num = +document.getElementById('room_num').value;
  const extra_edges = +document.getElementById('extra_edges').value;
  const r = await fetch('/generate', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ seed, room_num, extra_edges }) });
  const j = await r.json();
  document.getElementById('gen_status').textContent = `OK -> ${j.map_path}`;
}

async function loadMapText() {
  const r = await fetch('/map_text');
  const j = await r.json();
  document.getElementById('map_text').textContent = j.text || '';
}

async function shortest() {
  const map_path = document.getElementById('map_path_short').value.trim();
  const start = +document.getElementById('start').value;
  const goal = +document.getElementById('goal').value;
  const r = await fetch('/shortest', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path, start, goal }) });
  const j = await r.json();
  document.getElementById('shortest_out').textContent = `PATH: ${j.path?.join('->') || ''}`;
}

async function topPaths() {
  const map_path = document.getElementById('map_path_top').value.trim();
  const k = +document.getElementById('k').value;
  const initial_power = +document.getElementById('initp').value;
  const r = await fetch('/top', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path, start: 0, goal: 5, k, initial_power }) });
  const j = await r.json();
  const lines = (j.paths || []).map(p => `#${p.rank} val=${p.val} pow+=${p.pow} ${p.path.join('->')}`);
  document.getElementById('top_out').textContent = lines.join('\n');
}

async function exploreShortest() {
  const map_path = document.getElementById('map_path_ex').value.trim();
  const start = +document.getElementById('start_ex').value;
  const goal = +document.getElementById('goal_ex').value;
  const initial_power = +document.getElementById('initp_ex').value;
  const r = await fetch('/explore', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path, start, goal, initial_power }) });
  const j = await r.json();
  document.getElementById('ex_out').textContent = `success=${j.success} power_end=${j.power_end} total_value=${j.total_value}\nprogressed=${(j.progressed||[]).join('->')}`;
}

async function bossMove() {
  const map_path = document.getElementById('map_path_boss').value.trim();
  const r = await fetch('/bossmove', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path }) });
  const j = await r.json();
  document.getElementById('boss_out').textContent = `boss=${j.boss_id} new_map=${j.map_path}`;
}
