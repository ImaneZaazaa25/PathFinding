const canvas = document.getElementById('gridCanvas');
const ctx = canvas.getContext('2d');
const CELL_SIZE = 25;

let ROWS = 20;
let COLS = 30;
let grid = [];
let start = { row: 2, col: 2 };
let end = { row: 15, col: 25 };
let pathfinderModule = null;
let algorithm = "astar";

let dragging = null;

// ============ Charger WASM ============
Pathfinder().then(Module => {
    pathfinderModule = Module;
    initGrid();
    drawGrid();
});

// ============ Initialisation ============
function initGrid() {
    grid = Array.from({ length: ROWS }, () => Array(COLS).fill(0));
}

// ============ Dessiner la grille ============
function drawGrid() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    for (let i = 0; i < ROWS; i++) {
        for (let j = 0; j < COLS; j++) {
            if (i === start.row && j === start.col) ctx.fillStyle = "green";
            else if (i === end.row && j === end.col) ctx.fillStyle = "blue";
            else ctx.fillStyle = grid[i][j] === 1 ? "#777" : "#fff";

            ctx.fillRect(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE);

            // ligne de séparation
            ctx.strokeStyle = "#ccc";
            ctx.strokeRect(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }
}

// ============ Dessiner une cellule ============
function drawCell(row, col, color) {
    ctx.fillStyle = color;
    ctx.fillRect(col*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
    ctx.strokeStyle = "#ccc";
    ctx.strokeRect(col*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
}

// ============ Animation des nœuds ============
function animateNodes(nodes, color, delay = 20) {
    return new Promise(resolve => {
        let i = 0;
        const interval = setInterval(() => {
            if (i >= nodes.length) { clearInterval(interval); resolve(); return; }
            if (!(nodes[i].x === start.row && nodes[i].y === start.col) &&
                !(nodes[i].x === end.row && nodes[i].y === end.col)) {
                drawCell(nodes[i].x, nodes[i].y, color);
            }
            i++;
        }, delay);
    });
}

// ============ Run Pathfinding ============
document.getElementById('runBtn').addEventListener('click', async () => {
    if (!pathfinderModule) return alert("WASM pas encore chargé !");

    const flatGrid = [];
    for (let i = 0; i < ROWS; i++)
        for (let j = 0; j < COLS; j++)
            flatGrid.push(grid[i][j]);

    let algorithmCode = algorithm === "bfs" ? 0 :
                        algorithm === "dijkstra" ? 1 : 2;

    const pathVector = pathfinderModule.findPath(
        flatGrid,
        ROWS, COLS,
        start.row, start.col,
        end.row, end.col,
        algorithmCode
    );

    drawGrid();

    // Animation
    await animateNodes(pathVector, "yellow", 15);
    await animateNodes(pathVector, "red", 30);
});

// ============ Reset ============
document.getElementById('resetBtn').addEventListener('click', () => {
    initGrid();
    drawGrid();
});

// ============ Select Algorithm ============
document.getElementById('algorithmSelect').addEventListener('change', (e) => {
    algorithm = e.target.value;
});

// ============ Canvas size ============
canvas.width = COLS * CELL_SIZE;
canvas.height = ROWS * CELL_SIZE;

// ============ Interaction souris ============
canvas.addEventListener('mousedown', e => {
    const rect = canvas.getBoundingClientRect();
    const col = Math.floor((e.clientX - rect.left) / CELL_SIZE);
    const row = Math.floor((e.clientY - rect.top) / CELL_SIZE);

    if (row === start.row && col === start.col) dragging = "start";
    else if (row === end.row && col === end.col) dragging = "end";
    else {
        grid[row][col] = grid[row][col] === 0 ? 1 : 0; // obstacle
        drawGrid();
    }
});

canvas.addEventListener('mousemove', e => {
    if (!dragging) return;
    const rect = canvas.getBoundingClientRect();
    const col = Math.floor((e.clientX - rect.left) / CELL_SIZE);
    const row = Math.floor((e.clientY - rect.top) / CELL_SIZE);
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;

    if (dragging === "start") start = { row, col };
    else if (dragging === "end") end = { row, col };

    drawGrid();
});

canvas.addEventListener('mouseup', e => { dragging = null; });
