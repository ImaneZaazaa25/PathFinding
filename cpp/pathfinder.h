#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <utility>

struct Node {
    int x, y;
    bool obstacle = false;
    int gCost = 0;
    int hCost = 0;
    int fCost() const { return gCost + hCost; }
    Node* parent = nullptr;
};

// A* et Dijkstra
std::vector<std::pair<int,int>> dijkstra(const std::vector<std::vector<Node>>& grid, std::pair<int,int> start, std::pair<int,int> end, std::vector<std::pair<int,int>>& visited);
std::vector<std::pair<int,int>> astar(const std::vector<std::vector<Node>>& grid, std::pair<int,int> start, std::pair<int,int> end, std::vector<std::pair<int,int>>& visited);

#endif
