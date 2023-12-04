#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>

using namespace std;

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

void reduceOptionsTwins(array<vector<int>, 9>& optionsForAll) {
    // initialize map
    unordered_map<int, vector<int>> optionsInSquares;
    for(int i = 1; i <= 9 ; i++) {
        optionsInSquares[i] = {};
    }

    // populate map
    for(int i = 0; i < 9 ; i++) {
        vector<int> currSqOptions = optionsForAll[i];
        for(int option: currSqOptions) {
            optionsInSquares[option].push_back(i);
        }
    }

    // printf("---->After populating:\n");
    // printOptionsInSquares(optionsInSquares);

    // only keep options that are present in exactly 2 squares
    auto pair = optionsInSquares.begin();
    while (pair != optionsInSquares.end()){
        if(pair->second.size() != 2) {
            pair = optionsInSquares.erase(pair);
        } else {
            ++pair;
        }
    }

    // printf("---->After reducing based on size:\n");
    // printOptionsInSquares(optionsInSquares);


    // check for twins
    auto currPair = optionsInSquares.begin();
    auto nextPair = optionsInSquares.begin();
    ++nextPair;
    while (currPair != optionsInSquares.end() && nextPair != optionsInSquares.end()){
        // cout << "\n";

        while (nextPair != optionsInSquares.end() && currPair != nextPair){
            // std::cout << "currPair = Key: " << currPair->first << ", ";
            // std::cout << "nextPair = Key: " << nextPair->first << "\n";
            if(currPair->second == nextPair->second){

                int squareIndex1 = currPair->second.front();
                int squareIndex2 = currPair->second.back();
                vector<int> newOptions = {currPair->first, nextPair->first};

                // printf("---->FOUND A MATCH:\n");
                // printf("indeces are %d and %d, numbers are %d and %d\n", squareIndex1, squareIndex2, currPair->first, nextPair->first);
                
                optionsForAll[squareIndex1] = newOptions;
                optionsForAll[squareIndex2] = newOptions;
                break;
            }
            ++nextPair;
        }
        // cout << "\n";
        // std::cout << "Moving on onto next currPair\n";
        ++currPair;
        nextPair = optionsInSquares.begin();
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