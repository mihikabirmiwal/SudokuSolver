#include <vector>
#include <array>
#include <algorithm>

using namespace std;

void print(const array<array<int, 9>, 9> grid) {
    for (const auto& row : grid) {
        for (const auto& element : row) {
            printf("%d ", element);
        }
        printf("\n");
    }
    printf("-----------\n"); // Add a separator between 2D arrays
}


// intitialize options, if grid value empty then all options. if already filled in, then 1 option (the value itself)
array<array<vector<int>, 9>, 9> getOptions(const array<array<int, 9>, 9>& grid) {
    array<array<vector<int>, 9>, 9> options;
    for(int r=0;r<9;r++) {
        for(int c=0;c<9;c++) {
            if(grid[r][c]==0) {
                for(int i=1;i<=9;i++) options[r][c].push_back(i);
            } else {
                options[r][c].push_back(grid[r][c]);
            }
        }
    }
    return options;
}

void reduceOptions(array<array<vector<int>, 9>, 9> options, const array<array<int, 9>, 9>& grid) {
    for(int r=0;r<9;r++) {
        for(int c=0;c<9;c++) {
            // TODO can add multithreading at this level
            // only need to reduce options for empty spots
            if(grid[r][c]==0) {
                vector<int> option = options[r][c];
                // check row
                for(int i=0;i<9;i++) {
                    // number to remove
                    int num = grid[r][i];
                    // index of number to remove
                    auto numInd = find(option.begin(), option.end(), num);
                    // remove if value was found
                    if(numInd != option.end()) option.erase(numInd);
                    
                }

                // check col
                for(int i=0;i<9;i++) {
                    // number to remove
                    int num = grid[i][c];
                    // index of number to remove
                    auto numInd = find(option.begin(), option.end(), num);
                    // remove if value was found
                    if(numInd != option.end()) option.erase(numInd);
                }

                // check small square
                int startRow = r - r%3;
                int startCol = c - c%3;
                for(int i=0;i<3;i++) {
                    for(int j=0;j<3;j++) {
                        // number to remove
                        int num = grid[startRow+i][startCol+j];
                        // index of number to remove
                        auto numInd = find(option.begin(), option.end(), num);
                        // remove if value was found
                        if(numInd != option.end()) option.erase(numInd);
                    }
                }
            }
        }
    }
}


// check to see if the number can be placed at a specific spot, will be called prior to placing the value on the board
bool isSafe(const array<array<int, 9>, 9>& grid, int row, int col, int num) {
    // check if not present in current row and column
    for(int i=0;i<9;i++) {
        if(grid[row][i]==num || grid[i][col]==num) return false;
    }
    
    // check if not present in current 3x3 subgrid
    int startRow = row - row%3;
    int startCol = col - col%3;
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            if(grid[startRow+i][startCol+j]==num) return false;
        }
    }

    // safe placement
    return true;
}

bool pureBacktracking(array<array<int, 9>, 9>& grid, const array<array<vector<int>, 9>, 9> options) {
    for(int r=0;r<9;r++) {
        for(int c=0;c<9;c++) {
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