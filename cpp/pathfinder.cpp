#include "pathfinder.h"
#include <queue>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

// Voisins 4 directions
vector<pair<int,int>> directions = {{0,1},{1,0},{0,-1},{-1,0}};

// Heuristique Manhattan pour A*
int heuristic(pair<int,int> a, pair<int,int> b){
    return abs(a.first - b.first) + abs(a.second - b.second);
}

// Dijkstra
vector<pair<int,int>> dijkstra(const vector<vector<Node>>& grid, pair<int,int> start, pair<int,int> end, vector<pair<int,int>>& visited){
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, 1e9));
    vector<vector<Node*>> nodes(rows, vector<Node*>(cols, nullptr));
    auto cmp = [&](pair<int,int> a, pair<int,int> b){ return dist[a.first][a.second] > dist[b.first][b.second]; };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);

    dist[start.first][start.second] = 0;
    pq.push(start);

    while(!pq.empty()){
        auto cur = pq.top(); pq.pop();
        visited.push_back(cur);
        if(cur == end) break;
        for(auto d : directions){
            int nx = cur.first + d.first;
            int ny = cur.second + d.second;
            if(nx>=0 && nx<rows && ny>=0 && ny<cols && !grid[nx][ny].obstacle){
                if(dist[nx][ny] > dist[cur.first][cur.second]+1){
                    dist[nx][ny] = dist[cur.first][cur.second]+1;
                    nodes[nx][ny] = new Node(grid[nx][ny]);
                    nodes[nx][ny]->parent = nodes[cur.first][cur.second];
                    pq.push({nx,ny});
                }
            }
        }
    }

    // reconstruire chemin
    vector<pair<int,int>> path;
    Node* curNode = nodes[end.first][end.second];
    while(curNode){
        path.push_back({curNode->x, curNode->y});
        curNode = curNode->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

// A* (très similaire à Dijkstra)
vector<pair<int,int>> astar(const vector<vector<Node>>& grid, pair<int,int> start, pair<int,int> end, vector<pair<int,int>>& visited){
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<int>> gCost(rows, vector<int>(cols, 1e9));
    vector<vector<Node*>> nodes(rows, vector<Node*>(cols, nullptr));
    auto cmp = [&](pair<int,int> a, pair<int,int> b){
        int f1 = gCost[a.first][a.second] + heuristic(a,end);
        int f2 = gCost[b.first][b.second] + heuristic(b,end);
        return f1 > f2;
    };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);

    gCost[start.first][start.second] = 0;
    pq.push(start);

    while(!pq.empty()){
        auto cur = pq.top(); pq.pop();
        visited.push_back(cur);
        if(cur == end) break;

        for(auto d : directions){
            int nx = cur.first + d.first;
            int ny = cur.second + d.second;
            if(nx>=0 && nx<rows && ny>=0 && ny<cols && !grid[nx][ny].obstacle){
                int tentative = gCost[cur.first][cur.second]+1;
                if(tentative < gCost[nx][ny]){
                    gCost[nx][ny] = tentative;
                    nodes[nx][ny] = new Node(grid[nx][ny]);
                    nodes[nx][ny]->parent = nodes[cur.first][cur.second];
                    pq.push({nx,ny});
                }
            }
        }
    }

    vector<pair<int,int>> path;
    Node* curNode = nodes[end.first][end.second];
    while(curNode){
        path.push_back({curNode->x, curNode->y});
        curNode = curNode->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}
