// =========================
// CONFIGURATION
// =========================
const canvas = document.getElementById('gridCanvas');
const ctx = canvas.getContext('2d');
const CELL_SIZE = 40;

let ROWS = 0;
let COLS = 0;
let grid = [];
let start = { row: 8, col: 5 };
let end = { row: 8, col: 15 };
let pathfinderModule = null;
let mode = "obstacle";
let algorithm = "astar"; // "bfs", "dijkstra", "astar"

// =========================
// CHARGER LE MODULE WASM
// =========================
Pathfinder().then(Module => {
    pathfinderModule = Module;
    resizeCanvas();
}).catch(err => console.error("Erreur chargement WASM :", err));

// =========================
// REDIMENSIONNER CANVAS
// =========================
function resizeCanvas() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    ROWS = Math.floor(canvas.height / CELL_SIZE);
    COLS = Math.floor(canvas.width / CELL_SIZE);
    grid = Array.from({ length: ROWS }, () => Array(COLS).fill(0));
    drawGrid();
}

window.addEventListener('resize', resizeCanvas);

// =========================
// DESSINER LA GRILLE
// =========================
function drawGrid() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    for (let i = 0; i < ROWS; i++) {
        for (let j = 0; j < COLS; j++) {
            ctx.fillStyle = grid[i][j] === 1 ? "#777" : "#fff";
            ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1);

            if (i === start.row && j === start.col) {
                ctx.fillStyle = "green";
                ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }

            if (i === end.row && j === end.col) {
                ctx.fillStyle = "blue";
                ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }
    }
}

// =========================
// DESSINER LE CHEMIN
// =========================
function drawPath(path) {
    if (!path || path.length === 0) return;
    
    ctx.fillStyle = "red";
    path.forEach(point => {
        // point.x = ligne, point.y = colonne
        ctx.fillRect(
            point.y * CELL_SIZE + 6,  // colonne = y
            point.x * CELL_SIZE + 6,  // ligne = x
            CELL_SIZE - 12,
            CELL_SIZE - 12
        );
    });
}

// =========================
// GESTION CLAVIER
// =========================
document.addEventListener('keydown', e => {
    if (e.key === 's') mode = "start";
    if (e.key === 'e') mode = "end";
    if (e.key === 'o') mode = "obstacle";
});

// =========================
// GESTION CLICS SUR CANVAS
// =========================
canvas.addEventListener('click', e => {
    const rect = canvas.getBoundingClientRect();
    const col = Math.floor((e.clientX - rect.left) / CELL_SIZE);
    const row = Math.floor((e.clientY - rect.top) / CELL_SIZE);

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;

    if (mode === "obstacle") {
        grid[row][col] = 1;
    } else if (mode === "start") {
        start = { row, col };
    } else if (mode === "end") {
        end = { row, col };
    }

    drawGrid();
});

// =========================
// BOUTON RUN PATHFINDING
// =========================
document.getElementById('runBtn').addEventListener('click', () => {
    if (!pathfinderModule) {
        alert("WASM pas encore chargé !");
        return;
    }

    const flatGrid = [];
    for (let i = 0; i < ROWS; i++) {
        for (let j = 0; j < COLS; j++) {
            flatGrid.push(grid[i][j]);
        }
    }

    // Convertir l'algorithme en code numérique
    let algorithmCode;
    switch(algorithm) {
        case "bfs": algorithmCode = 0; break;
        case "dijkstra": algorithmCode = 1; break;
        case "astar": algorithmCode = 2; break;
        default: algorithmCode = 2;
    }

    try {
        const pathVector = pathfinderModule.findPath(
            flatGrid,
            ROWS,
            COLS,
            start.row,
            start.col,
            end.row,
            end.col,
            algorithmCode
        );

        drawGrid();
        drawPath(pathVector);

    } catch (err) {
        console.error("Erreur pathfinding :", err);
        alert("Impossible de calculer le chemin !");
    }
});

// =========================
// BOUTON RESET OBSTACLES
// =========================
document.getElementById('resetBtn').addEventListener('click', () => {
    grid = Array.from({ length: ROWS }, () => Array(COLS).fill(0));
    drawGrid();
});

// =========================
// SELECTION ALGORITHME
// =========================
document.getElementById('algorithmSelect').addEventListener('change', function(e) {
    algorithm = this.value;
    console.log("Algorithme sélectionné :", algorithm);
});

// =========================
// INITIALISATION
// =========================
resizeCanvas();