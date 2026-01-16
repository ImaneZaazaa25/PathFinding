#include <emscripten/bind.h>
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>

using namespace std;
using namespace emscripten;

// Structure pour un point
struct Point {
    int x;
    int y;
};

// Structure pour un nœud dans l'algorithme A*
struct AStarNode {
    Point pos;
    double f;
    
    bool operator>(const AStarNode& other) const {
        return f > other.f;
    }
};

// Structure pour Dijkstra/BFS
struct DijkstraNode {
    Point pos;
    int cost;
    
    bool operator>(const DijkstraNode& other) const {
        return cost > other.cost;
    }
};

// Heuristique Manhattan
int heuristic(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// =========================
// ALGORITHME BFS
// =========================
vector<Point> bfs(const vector<vector<int>>& grid, const Point& start, const Point& end) {
    vector<Point> emptyResult;
    
    if(grid.empty()) return emptyResult;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    
    if(start.x < 0 || start.x >= rows || start.y < 0 || start.y >= cols ||
       end.x < 0 || end.x >= rows || end.y < 0 || end.y >= cols) return emptyResult;
    
    if(grid[start.x][start.y] != 0 || grid[end.x][end.y] != 0) return emptyResult;
    
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1,-1}));
    
    queue<Point> q;
    q.push(start);
    visited[start.x][start.y] = true;
    
    vector<Point> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while(!q.empty()) {
        Point cur = q.front();
        q.pop();
        
        if(cur.x == end.x && cur.y == end.y) break;
        
        for(const auto& dir : directions) {
            int nx = cur.x + dir.x;
            int ny = cur.y + dir.y;
            
            if(nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
               !visited[nx][ny] && grid[nx][ny] == 0) {
                visited[nx][ny] = true;
                parent[nx][ny] = cur;
                q.push({nx, ny});
            }
        }
    }
    
    if(parent[end.x][end.y].x == -1) return emptyResult;
    
    vector<Point> path;
    Point cur = end;
    while(cur.x != -1) {
        path.push_back(cur);
        cur = parent[cur.x][cur.y];
    }
    
    reverse(path.begin(), path.end());
    return path;
}

// =========================
// ALGORITHME DIJKSTRA
// =========================
vector<Point> dijkstra(const vector<vector<int>>& grid, const Point& start, const Point& end) {
    vector<Point> emptyResult;
    
    if(grid.empty()) return emptyResult;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    
    if(start.x < 0 || start.x >= rows || start.y < 0 || start.y >= cols ||
       end.x < 0 || end.x >= rows || end.y < 0 || end.y >= cols) return emptyResult;
    
    if(grid[start.x][start.y] != 0 || grid[end.x][end.y] != 0) return emptyResult;
    
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1,-1}));
    
    priority_queue<DijkstraNode, vector<DijkstraNode>, greater<DijkstraNode>> pq;
    
    dist[start.x][start.y] = 0;
    pq.push({start, 0});
    
    vector<Point> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while(!pq.empty()) {
        DijkstraNode current = pq.top();
        pq.pop();
        Point cur = current.pos;
        
        if(visited[cur.x][cur.y]) continue;
        visited[cur.x][cur.y] = true;
        
        if(cur.x == end.x && cur.y == end.y) break;
        
        for(const auto& dir : directions) {
            int nx = cur.x + dir.x;
            int ny = cur.y + dir.y;
            
            if(nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
               grid[nx][ny] == 0 && !visited[nx][ny]) {
                
                int newDist = dist[cur.x][cur.y] + 1;
                
                if(newDist < dist[nx][ny]) {
                    dist[nx][ny] = newDist;
                    parent[nx][ny] = cur;
                    pq.push({{nx, ny}, newDist});
                }
            }
        }
    }
    
    if(parent[end.x][end.y].x == -1) return emptyResult;
    
    vector<Point> path;
    Point cur = end;
    while(cur.x != -1) {
        path.push_back(cur);
        cur = parent[cur.x][cur.y];
    }
    
    reverse(path.begin(), path.end());
    return path;
}

// =========================
// ALGORITHME A*
// =========================
vector<Point> astar(const vector<vector<int>>& grid, const Point& start, const Point& end) {
    vector<Point> emptyResult;
    
    if(grid.empty()) return emptyResult;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    
    if(start.x < 0 || start.x >= rows || start.y < 0 || start.y >= cols ||
       end.x < 0 || end.x >= rows || end.y < 0 || end.y >= cols) return emptyResult;
    
    if(grid[start.x][start.y] != 0 || grid[end.x][end.y] != 0) return emptyResult;
    
    vector<vector<int>> g(rows, vector<int>(cols, INT_MAX));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1,-1}));
    
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;
    
    g[start.x][start.y] = 0;
    int h_start = heuristic(start, end);
    pq.push({start, static_cast<double>(h_start)});
    
    vector<Point> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while(!pq.empty()) {
        AStarNode current = pq.top();
        pq.pop();
        Point cur = current.pos;
        
        if(visited[cur.x][cur.y]) continue;
        visited[cur.x][cur.y] = true;
        
        if(cur.x == end.x && cur.y == end.y) break;
        
        for(const auto& dir : directions) {
            int nx = cur.x + dir.x;
            int ny = cur.y + dir.y;
            
            if(nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
               grid[nx][ny] == 0 && !visited[nx][ny]) {
                
                int new_g = g[cur.x][cur.y] + 1;
                
                if(new_g < g[nx][ny]) {
                    g[nx][ny] = new_g;
                    parent[nx][ny] = cur;
                    Point next = {nx, ny};
                    int h = heuristic(next, end);
                    pq.push({next, static_cast<double>(new_g + h)});
                }
            }
        }
    }
    
    if(parent[end.x][end.y].x == -1) return emptyResult;
    
    vector<Point> path;
    Point cur = end;
    while(cur.x != -1) {
        path.push_back(cur);
        cur = parent[cur.x][cur.y];
    }
    
    reverse(path.begin(), path.end());
    return path;
}

// =========================
// FONCTION WRAPPER PRINCIPALE
// =========================
emscripten::val findPath(const emscripten::val& flatGrid, int rows, int cols, 
                        int startX, int startY, int endX, int endY,
                        int algorithmType) {
    
    // Convertir le tableau JavaScript en vector 2D
    vector<vector<int>> grid(rows, vector<int>(cols));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = flatGrid[i * cols + j].as<int>();
        }
    }
    
    Point start = {startX, startY};
    Point end = {endX, endY};
    
    vector<Point> path;
    
    switch(algorithmType) {
        case 0: // BFS
            path = bfs(grid, start, end);
            break;
        case 1: // Dijkstra
            path = dijkstra(grid, start, end);
            break;
        case 2: // A*
            path = astar(grid, start, end);
            break;
        default:
            path = astar(grid, start, end); // Par défaut A*
    }
    
    // Convertir le résultat en tableau JavaScript
    emscripten::val result = emscripten::val::array();
    
    for (size_t i = 0; i < path.size(); i++) {
        emscripten::val pointObj = emscripten::val::object();
        pointObj.set("x", path[i].x);
        pointObj.set("y", path[i].y);
        result.set(i, pointObj);
    }
    
    return result;
}

// =========================
// BINDINGS EMSCRIPTEN
// =========================
EMSCRIPTEN_BINDINGS(pathfinder_module) {
    value_object<Point>("Point")
        .field("x", &Point::x)
        .field("y", &Point::y);
    
    emscripten::function("findPath", &findPath);
}