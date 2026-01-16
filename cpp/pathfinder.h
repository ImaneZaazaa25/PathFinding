#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <utility>

// 🔹 Structure Node pour chaque cellule de la grille
struct Node {
    int x, y;             // coordonnées
    bool obstacle = false; // est-ce un obstacle
    int gCost = 0;         // coût pour A*
    int hCost = 0;         // heuristique pour A*
    Node* parent = nullptr; // parent dans le chemin

    // fonction pour calculer fCost (A* = g + h)
    int fCost() const { return gCost + hCost; }
};

// 🔹 Prototypes des fonctions de pathfinding
// visited est rempli au fur et à mesure des cellules explorées
std::vector<std::pair<int,int>> dijkstra(
    const std::vector<std::vector<Node>>& grid,
    std::pair<int,int> start,
    std::pair<int,int> end,
    std::vector<std::pair<int,int>>& visited
);

std::vector<std::pair<int,int>> astar(
    const std::vector<std::vector<Node>>& grid,
    std::pair<int,int> start,
    std::pair<int,int> end,
    std::vector<std::pair<int,int>>& visited
);

#endif // PATHFINDER_H
