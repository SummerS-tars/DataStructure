async function genMap() {
  const seed = +document.getElementById('seed').value;
  const room_num = +document.getElementById('room_num').value;
  const extra_edges = +document.getElementById('extra_edges').value;
  const r = await fetch('/generate', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ seed, room_num, extra_edges }) });
  const j = await r.json();
  document.getElementById('gen_status').textContent = `OK -> ${j.map_path}`;
  tryRenderFromRaw(j.raw, j.state);
}

async function genGrid() {
  const width = +document.getElementById('g_width').value;
  const height = +document.getElementById('g_height').value;
  const seed = +document.getElementById('g_seed').value;
  const r = await fetch('/generate_grid', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ width, height, seed }) });
  const j = await r.json();
  document.getElementById('grid_status').textContent = `OK -> ${j.map_path}`;
  tryRenderFromRaw(j.raw, j.state);
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

async function neighbors() {
  const map_path = document.getElementById('map_path_nbr').value.trim();
  const room_id = +document.getElementById('room_nbr').value;
  const r = await fetch('/neighbors', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path, room_id }) });
  const j = await r.json();
  document.getElementById('nbr_out').textContent = `NEIGH: ${(j.neighbors||[]).join(', ')}`;
}

async function stepMove() {
  const map_path = document.getElementById('map_path_step').value.trim();
  const out_map_path = document.getElementById('out_map_step').value.trim();
  const player_pos = +document.getElementById('pos_step').value;
  const next_room = +document.getElementById('next_step').value;
  const power = +document.getElementById('power_step').value;
  const r = await fetch('/step_move', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ map_path, out_map_path, player_pos, next_room, power }) });
  const j = await r.json();
  document.getElementById('step_out').textContent = `pos=${j.pos} power_end=${j.power_end} total_value=${j.total_value} moved=${j.moved} over=${j.over}\nmap=${j.map_path}`;
}

// --- Vis.js map rendering & interactions ---

let mapData = null;
let network = null;
let visited = new Set();
let adj = {};
let playerStatus = { pos: 0, power: 10, total_value: 0 };
let moveCount = 0;
let highlightedEdges = [];
let gameOver = false;

function openModal(title, msg) {
  const m = document.getElementById('modal');
  document.getElementById('modal_title').textContent = title;
  document.getElementById('modal_msg').textContent = msg;
  m.style.display = 'flex';
  gameOver = true;
}

function closeModalAndReset() {
  const m = document.getElementById('modal');
  m.style.display = 'none';
  gameOver = false;
  genMap();
}

function buildAdj(edges) {
  const res = {};
  for (const e of edges || []) {
    const u = e.source, v = e.target;
    if (!res[u]) res[u] = new Set();
    if (!res[v]) res[v] = new Set();
    res[u].add(v); res[v].add(u);
  }
  return res;
}

function tierToLevel(tier) {
  return (typeof tier === 'number' && tier >= 0) ? tier : undefined;
}

function typeToColor(type, isVisited) {
  if (isVisited) return '#bdc3c7';
  switch ((type || 'NORMAL').toUpperCase()) {
    case 'START': return '#2ecc71';
    case 'BOSS': return '#e74c3c';
    case 'TREASURE': return '#f1c40f';
    default: return '#3498db';
  }
}

function renderMap(json) {
  if (!window.vis) {
    setStatus('vis-network 未加载，检查 CDN 或网络连接', true);
    return;
  }
  if (!json || !json.rooms) {
    setStatus('地图数据为空或格式不正确', true);
    return;
  }
  mapData = json;
  adj = buildAdj(json.edges || []);
  const nodes = json.rooms.map(r => {
    const hasItems = (r.items || []).length > 0;
    const labelParts = [`${r.id}`];
    if (typeof r.power === 'number') labelParts.push(`p=${r.power}`);
    if (hasItems) labelParts.push('📦');
    return {
      id: r.id,
      label: labelParts.join(' '),
      level: tierToLevel(r.tier),
      color: typeToColor(r.type, visited.has(r.id)),
      shape: 'dot',
      size: 16,
    };
  });
  const edges = (json.edges || []).map(e => ({ from: e.source, to: e.target, id: `${Math.min(e.source, e.target)}-${Math.max(e.source, e.target)}` }));
  const container = document.getElementById('map_canvas');
  network = new vis.Network(container, { nodes, edges }, {
    layout: { hierarchical: { enabled: true, direction: 'UD', sortMethod: 'directed' } },
    physics: false,
    interaction: { hover: true },
  });

  network.on('click', async (params) => {
    if (!params.nodes || params.nodes.length === 0) return;
    const target = params.nodes[0];
    const cur = playerStatus.pos;
    if (adj[cur] && adj[cur].has(target)) {
      await actionMove(target);
    } else {
      setStatus(`请选择与当前位置(${cur})邻接的节点`, true);
    }
  });

  setStatus(`地图加载完成，房间数 ${json.rooms.length}`, false);

  applyHighlight();
}

function setStatus(msg, warn=false) {
  const el = document.getElementById('map_status');
  el.textContent = msg || '';
  el.className = warn ? 'warn' : 'ok';
}

function updatePlayerStatus(status) {
  if (!status) return;
  playerStatus = { ...playerStatus, ...status };
  document.getElementById('player_pos').textContent = playerStatus.pos;
  document.getElementById('player_power').textContent = playerStatus.power;
  document.getElementById('player_value').textContent = playerStatus.total_value;
  visited.add(playerStatus.pos);
  if (mapData) renderMap(mapData); // refresh colors
}

async function actionMove(target) {
  if (gameOver) return;
  const body = { target_room_id: target };
  // let backend use persisted map_path
  const r = await fetch('/action/move', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body) });
  const j = await r.json();
  updatePlayerStatus(j.player_status);
  if (j.status === 'VICTORY') {
    setStatus('击败 Boss，胜利！', false);
    openModal('VICTORY', '你已击败 Boss！');
  } else {
    setStatus(j.map_updates?.moved ? `移动到 ${target}，胜利=${!j.map_updates.over}` : '移动失败（可能未邻接或战斗失败）', !j.map_updates?.moved);
  }
  moveCount += j.map_updates?.moved ? 1 : 0;
  if (moveCount >= 3 && !gameOver) {
    moveCount = 0;
    await bossTick();
  } else {
    await refreshShortest();
  }
}

function tryRenderFromRaw(raw, state) {
  try {
    const json = typeof raw === 'string' ? JSON.parse(raw) : raw;
    renderMap(json);
    if (state) updatePlayerStatus({ pos: state.player_pos, power: state.power, total_value: state.total_value });
    refreshShortest();
  } catch (e) {
    console.error('parse map json failed', e);
    setStatus('地图 JSON 解析失败，请检查后端输出', true);
  }
}

// --- Shortest path highlighting ---

async function refreshShortest() {
  if (!mapData || gameOver) return;
  const bossId = mapData.bossId;
  if (bossId === undefined || bossId === null) return;
  try {
    const body = { map_path: mapData.map_path || undefined, start: playerStatus.pos, goal: bossId };
    const r = await fetch('/shortest', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body) });
    const j = await r.json();
    highlightPath(j.path || []); // clear highlight on failure
  } catch (e) {
    console.warn('shortest fetch failed', e);
    highlightPath([]); // clear highlight on failure
  }
}

function highlightPath(path) {
  highlightedEdges = [];
  for (let i = 1; i < path.length; i++) {
    const u = path[i-1], v = path[i];
    const id = u < v ? `${u}-${v}` : `${v}-${u}`;
    highlightedEdges.push(id);
  }
  applyHighlight();
}

function applyHighlight() {
  if (!network || !mapData) return;
  const updates = [];
  const defaultColor = '#cccccc';
  const defaultWidth = 1;
  for (const e of (mapData.edges || [])) {
    const id = `${Math.min(e.source, e.target)}-${Math.max(e.source, e.target)}`;
    const isHL = highlightedEdges.includes(id);
    updates.push({ id, color: isHL ? { color: '#e74c3c' } : { color: defaultColor }, width: isHL ? 3 : defaultWidth });
  }
  if (updates.length) network.body.data.edges.update(updates);
}

// --- Boss tick and wait ---

async function bossTick() {
  if (gameOver) return;
  const r = await fetch('/boss/tick', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({}) });
  const j = await r.json();
  if (mapData) {
    // 重新标记 Boss 类型，避免颜色不更新
    const prevBoss = mapData.bossId;
    mapData.bossId = j.boss_id;
    mapData.map_path = j.map_path;
    if (Array.isArray(mapData.rooms)) {
      mapData.rooms = mapData.rooms.map(r => {
        if (r.id === mapData.bossId) return { ...r, type: 'BOSS', is_boss: true };
        if (prevBoss !== undefined && r.id === prevBoss) return { ...r, type: 'NORMAL', is_boss: false };
        return r;
      });
    }
    // 直接重渲染即可，避免 moveNode 位置变 NaN 导致节点缺失
    renderMap(mapData);
  }
  await refreshShortest();
}

async function waitBoss() {
  await bossTick();
}

// --- Loot tree rendering ---

function createLootNode(node) {
  if (!node) return document.createTextNode('');
  const hasChildren = Array.isArray(node.children) && node.children.length > 0;
  const label = node.label || 'Item';
  if (!hasChildren) {
    const div = document.createElement('div');
    div.textContent = label + (node.favorite ? ' ★' : '');
    return div;
  }
  const details = document.createElement('details');
  details.open = true;
  const summary = document.createElement('summary');
  summary.textContent = label;
  details.appendChild(summary);
  for (const child of node.children) {
    details.appendChild(createLootNode(child));
  }
  return details;
}

function renderLootTree(json) {
  const container = document.getElementById('loot_tree');
  container.innerHTML = '';
  container.appendChild(createLootNode(json));
}

function renderLootFromInput() {
  const txt = document.getElementById('loot_json').value.trim();
  if (!txt) return;
  try {
    const obj = JSON.parse(txt);
    renderLootTree(obj);
  } catch (e) {
    alert('JSON 解析失败');
  }
}
