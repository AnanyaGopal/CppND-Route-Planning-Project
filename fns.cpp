#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;



enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;

    vector<State> row;
    while (sline >> n >> c && c == ',') {
        // TODO: Modify the line below to push_back
        // a State::kEmpty if n is 0, and push_back
        // a State::kObstacle otherwise.
        if(n)
            row.push_back(State::kObstacle);
        else
            row.push_back(State::kEmpty);
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile (path);
    vector<vector<State>> board{};
    if (myfile) {
        
        string line;
        while (getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    } else {
        std::cout << "File does not exist\n";
        
    }
    return board;
}

/* Our creation. */
string CellString(State type)
{
    switch(type) {
        case State::kObstacle: return "⛰️   ";
        default: return "0   ";
    }
    
}

void PrintBoard(const vector<vector<State>> board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}


bool Compare(vector<int> Node1,vector<int> Node2){
    int node1f = Node1[2]+Node1[3];
    int node2f = Node2[2]+Node2[3];
    return (node1f > node2f ? true : false);
}

void CellSort(vector<vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

bool CheckValidCell(int x, int y, vector<vector<State>> & grid){
    
    if((x>=grid.size()) || (y>=grid[0].size()))
        return false;
    
    if(grid[x][y]==State::kEmpty)
        return true;

    return false;
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Add node to open vector, and mark grid cell as closed.
    openlist.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];
    
    // Loop through current node's potential neighbors.
    for (int i = 0; i < 4; i++) {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];
        
        // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
        if (CheckValidCell(x2, y2, grid)) {
            // Increment g value and add neighbor to open list.
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
    // Create the vector of open nodes.
    vector<vector<int>> open {};
    
    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0],goal[1]);
    AddToOpen(x, y, g, h, open, grid);
    
    while (open.size() > 0) {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;
        
        // Check if we're done.
        if (x == goal[0] && y == goal[1]) {
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }
        
        // If we're not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open, grid);
    }
    
    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return std::vector<vector<State>>{};
}
/*
    

#include "test.h"
int main() {
    string filename = "/Users/ananygop/C++/Udacity Nanodegree/RouteMapPlanner/RouteMapPlanner/1.board";
    int init[2]{0, 0};
    int goal[2]{4, 5};
    auto board = ReadBoardFile(filename);
    auto solution = Search(board, init, goal);
    
    PrintBoard(solution);
    // Tests
    
    TestHeuristic();
    TestAddToOpen();
    TestCompare();
    TestSearch(filename);
    TestCheckValidCell();
    TestExpandNeighbors();
    
}
*/
