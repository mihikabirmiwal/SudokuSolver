#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <cmath>

using namespace std;

const int NUM_GRID_SIZE = 16;

// intitialize options, if grid value empty then all options. if already filled in, then 1 option (the value itself)
array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> getOptions(const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid) {
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> options;
    for(int r=0;r<NUM_GRID_SIZE;r++) {
        for(int c=0;c<NUM_GRID_SIZE;c++) {
            if(grid[r][c]==0) {
                for(int i=1;i<=NUM_GRID_SIZE;i++) options[r][c].push_back(i);
            } else {
                options[r][c].push_back(grid[r][c]);
            }
        }
    }
    return options;
}

void printOptionsInSquares(const std::unordered_map<int, std::vector<int>>& optionsInSquares) {
    for (const auto& pair : optionsInSquares) {
        std::cout << pair.first << "=";

        // Print vector elements separated by commas
        for (size_t i = 0; i < pair.second.size(); ++i) {
            std::cout << pair.second[i];
            if (i < pair.second.size() - 1) {
                std::cout << ",";
            }
        }

        std::cout << "\n";
    }
}

// check to see if the number can be placed at a specific spot, will be called prior to placing the value on the board
bool isSafe(const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid, int row, int col, int num) {
    // check if not present in current row and column
    for(int i=0;i<NUM_GRID_SIZE;i++) {
        if(grid[row][i]==num || grid[i][col]==num) return false;
    }
    
    // check if not present in current subgrid
    double num_grid = NUM_GRID_SIZE;
    int subsquare = static_cast<int>(sqrt(num_grid));
    int startRow = row - row%subsquare;
    int startCol = col - col%subsquare;
    for(int i=0;i<subsquare;i++) {
        for(int j=0;j<subsquare;j++) {
            if(grid[startRow+i][startCol+j]==num) return false;
        }
    }

    // safe placement
    return true;
}

bool pureBacktracking(array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid, const array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> options) {
    for(int r=0;r<NUM_GRID_SIZE;r++) {
        for(int c=0;c<NUM_GRID_SIZE;c++) {
            if(grid[r][c]==0) {
                vector<int> allOptions = options[r][c];
                for(int option: allOptions) {
                    if(isSafe(grid, r, c, option)) {
                        grid[r][c] = option;
                        if(pureBacktracking(grid, options)) return true;
                        else grid[r][c] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}