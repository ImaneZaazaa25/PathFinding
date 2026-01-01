const canvas = document.getElementById('gridCanvas');
const ctx = canvas.getContext('2d');
const ROWS = 10, COLS = 10;
const CELL_SIZE = 40;

let grid = Array.from({length:ROWS}, ()=>Array(COLS).fill(0)); // 0 libre, 1 obstacle
let path = [], visited = [];

function drawGrid(){
    for(let i=0;i<ROWS;i++){
        for(let j=0;j<COLS;j++){
            if(grid[i][j]===1) ctx.fillStyle='black';
            else ctx.fillStyle='white';
            ctx.fillRect(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE-1, CELL_SIZE-1);
        }
    }
    visited.forEach(([x,y])=>{
        ctx.fillStyle='yellow';
        ctx.fillRect(y*CELL_SIZE,x*CELL_SIZE,CELL_SIZE-1,CELL_SIZE-1);
    });
    path.forEach(([x,y])=>{
        ctx.fillStyle='blue';
        ctx.fillRect(y*CELL_SIZE,x*CELL_SIZE,CELL_SIZE-1,CELL_SIZE-1);
    });
}

// simulation: lire output.json
document.getElementById('runBtn').addEventListener('click', ()=>{
    fetch('../data/output.json')
    .then(resp=>resp.json())
    .then(data=>{
        visited = data.visited;
        path = data.path;
        drawGrid();
    });
});

drawGrid();
