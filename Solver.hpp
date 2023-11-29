#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>

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

void reduceOptionsElimination(array<array<vector<int>, 9>, 9> options, const array<array<int, 9>, 9>& grid) {
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

void reduceOptionsTwins(array<vector<int>, 9> optionsForGroup) {
    array<unordered_map<int, vector<int>>, 9> optionsAvailable;
    for (int i = 0; i < 9; i++) { // iterate through each square
        if (optionsForGroup[i].size() > 1) {
            for (int j = 0; j < optionsForGroup[i].size(); j++) { // iterate through each option per square
                for (int k = i + 1; k < 9; j++) { // iterate over the rest of squares
                    int curr_option = optionsForGroup[i][j];
                    auto found = find(optionsForGroup[k].begin(), optionsForGroup[k].end(), curr_option);
                    if (found != optionsForGroup[k].end()) {
                        vector<int> listOfSquaresCurr;
                        vector<int> listOfSquaresMatch;
                        try {
                            listOfSquaresCurr = optionsAvailable[i].at(curr_option);
                        } catch (const std::out_of_range& e) {}   
                        try {
                            listOfSquaresMatch = optionsAvailable[k].at(curr_option);
                        } catch (const std::out_of_range& e) {}   
                        listOfSquaresCurr.push_back(k);
                        listOfSquaresMatch.push_back(i);
                        optionsAvailable[i].at(curr_option) = listOfSquaresCurr;
                        optionsAvailable[k].at(curr_option) = listOfSquaresMatch;
                    }
                }
            }
            // NOTES: ya esta la iteration para crear todos los hashmaps con las opciones
            // de distintos cuadros donde se repite la opcion. Lo que falta (TODO:) es hacer
            // el check y descartar los cuadrados que no cumplen los requisitos para twins
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