#include "pathfinder.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

int main() {
    ifstream file("../data/input.json");
    json j; file >> j;

    int rows = j["rows"], cols = j["cols"];
    vector<vector<Node>> grid(rows, vector<Node>(cols));
    for(int i=0;i<rows;i++)
        for(int k=0;k<cols;k++)
            grid[i][k] = {i,k,false};

    auto start = make_pair(j["start"][0], j["start"][1]);
    auto end = make_pair(j["end"][0], j["end"][1]);
    for(auto obs : j["obstacles"]){
        grid[obs[0]][obs[1]].obstacle = true;
    }

    vector<pair<int,int>> visited;
    auto path = astar(grid, start, end, visited); // ou dijkstra(grid,...)

    json output;
    output["path"] = path;
    output["visited"] = visited;

    ofstream outFile("../data/output.json");
    outFile << output.dump(4);
    cout << "Path saved in ../data/output.json\n";
    return 0;
}
