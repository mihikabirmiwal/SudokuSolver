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

array<vector<int>, 9> loneRangerTester() {
    array<vector<int>, 9> ret;
    ret[0].push_back(1);
    ret[0].push_back(2);

    ret[1].push_back(3);

    ret[2].push_back(3);
    ret[2].push_back(4);

    ret[3].push_back(1);
    ret[3].push_back(2);
    ret[3].push_back(5);

    ret[4].push_back(6);

    ret[5].push_back(5);
    ret[5].push_back(6);
    ret[5].push_back(7);

    ret[6].push_back(8);
    ret[6].push_back(9);

    ret[7].push_back(8);
    ret[7].push_back(9);
    ret[7].push_back(1);

    ret[8].push_back(2);
    return ret;
}

void reduceOptionsLoneRanger(array<vector<int>, 9>& options) {
    // initialize hashmap
    std::unordered_map<int, vector<int>> found;
    for(int i=1;i<=9;i++) {
        found[i] = {};
    }
    // populate hashmap
    for(int index=0;index<9;index++) {
        vector<int> currSqOptions = options[index];
        for(int option: currSqOptions) {
            found[option].push_back(index);
        }
    }
    // check if any of the values has a vector of length 1
    for (auto& pair : found) {
        if(pair.second.size()==1) {
            // is a lone ranger!
            int value = pair.first;
            int index = pair.second[0];
            cout << "found a lone ranger! value = " << value << " index = " << index << "\n";
            options[index].clear();
            options[index].push_back(value);
            cout << "after clearing\n";
            for(auto option: options[index]) {
                cout << option << " ";
            }
            cout << "\n";
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

void reduceOptionsLoneRanger(array<array<vector<int>, 9>, 9> options) {
    // all rows
    for(int row=0;row<9;row++) {
        reduceOptionsLoneRanger(options[row]);
    }
    // all cols
    for(int col=0;col<9;col++) {
        array<vector<int>, 9> myArr;
        for(int r=0;r<9;r++) {
            myArr[r] = options[r][col];
        }
        reduceOptionsLoneRanger(myArr);
    }
    // all subsquares
    for(int sq=0;sq<9;sq++) {
        array<vector<int>, 9> myArr;
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r=0;r<3;r++) {
            for(int c=0;c<3;c++) {
                myArr[index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        reduceOptionsLoneRanger(myArr);
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