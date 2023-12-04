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

struct ThreadArgs {
    array<array<vector<int>, 9>, 9>& options;
    const array<array<int, 9>, 9>& grid;
    int start;
    int end;
};

void* reduceOptionsEliminationHelper(void* arg) {
    ThreadArgs* threadArgs = static_cast<ThreadArgs*>(arg);
    array<array<vector<int>, 9>, 9>& options = threadArgs->options;
    const array<array<int, 9>, 9>& grid = threadArgs->grid;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for(int j=start;j<end;j++) {
        int r = j/9;
        int c = j%9;
        vector<int>& option = options[r][c];
        if(grid[r][c]==0) {
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
    delete threadArgs;
    pthread_exit(nullptr);
}


void reduceOptionsElimination(array<array<vector<int>, 9>, 9>& options, const array<array<int, 9>, 9>& grid, int numThreads) {
    pthread_t threads[numThreads];
    if(numThreads>81) numThreads = 81;
    int expectedLoad = 81/numThreads;
    int extraLoad = 81%numThreads;
    int startCalc = 0;
    for(int i=0;i<numThreads;i++) {
        int endCalc = startCalc+expectedLoad;
        if(extraLoad>0) {
            endCalc++;
            extraLoad--;
        }
        ThreadArgs* args = new ThreadArgs{options, grid, startCalc, endCalc};
        pthread_create(&threads[i], nullptr, reduceOptionsEliminationHelper, static_cast<void*>(args));
        // reduceOptionsEliminationHelper(options, grid, startCalc, endCalc);
        startCalc = endCalc;
    }
    // wait on all threads to finish
    for(int i=0;i<numThreads;i++) {
        pthread_join(threads[i], nullptr);
    }
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
            // cout << "found a lone ranger! value = " << value << " index = " << index << "\n";
            options[index].clear();
            options[index].push_back(value);
        }
    }
}

void reduceOptionsLoneRanger(array<array<vector<int>, 9>, 9>& options) {
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

    // only keep options that are present in exactly 2 squares
    auto pair = optionsInSquares.begin();
    while (pair != optionsInSquares.end()){
        if(pair->second.size() != 2) {
            pair = optionsInSquares.erase(pair);
        } else {
            ++pair;
        }
    }

    // check for twins
    auto firstPair = optionsInSquares.begin();
    bool found = false;
    while (firstPair != optionsInSquares.end()){
        auto secondPair = firstPair;
        ++secondPair;
        while (secondPair != optionsInSquares.end()){
            if (firstPair->second == secondPair->second){
                printf("testingTwins: --->found twins!\n");
                // printf("testingTwins: crashing1!\n");
                int squareIndex1 = firstPair->second[0];
                int squareIndex2 = firstPair->second[1];
                vector<int> newOptions = {secondPair->first, firstPair->first};

                // printf("testingTwins: crashing2!\n");

                optionsForAll[squareIndex1] = newOptions;
                optionsForAll[squareIndex2] = newOptions;

                // printf("testingTwins: crashing3!\n");
                found = true;
                break;
            }
            if (found) {
                break;
            } else {
                ++secondPair;
            }
        }
        ++firstPair;
    }
}

void *reduceOptionsTwins(void *arg){
    array<vector<int>, 9> *options = (array<vector<int>, 9> *) arg;
    reduceOptionsTwins(*options);
    return NULL;
}

void reduceOptionsTwins(array<array<vector<int>, 9>, 9>& options) {
    // all rows
    for(int r = 0; r < 9; r++) {
        reduceOptionsTwins(options[r]);
    }

    // all cols
    for(int c = 0; c < 9; c++) {
        array<vector<int>, 9> myArr;
        for(int r = 0; r < 9; r++) {
            myArr[r] = options[r][c];
        }
        reduceOptionsTwins(myArr);
    }

    // all subsquares
    for(int sq = 0; sq < 9; sq++) {
        array<vector<int>, 9> myArr;
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r = 0; r < 3; r++) {
            for(int c = 0; c < 3; c++) {
                myArr[index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        reduceOptionsTwins(myArr);
    }
}

void reduceOptionsTwinsParallel(array<array<vector<int>, 9>, 9>& options) {
    // all rows
    pthread_t row_threads[9];
    for(int r = 0; r < 9; r++){
        pthread_create(&row_threads[r], nullptr, reduceOptionsTwins, (void*) &options[r]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(row_threads[i], nullptr);
    }

    // all cols
    pthread_t col_threads[9];
    array<array<vector<int>, 9>, 9> myArrCol;
    for(int c = 0; c < 9; c++){
        for(int r = 0; r < 9; r++) {
            myArrCol[c][r] = options[r][c];
        }
        pthread_create(&col_threads[c], nullptr, reduceOptionsTwins, (void*) &myArrCol[c]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(col_threads[i], nullptr);
    }

    // all subsquares
    pthread_t sq_threads[9];    
    array<array<vector<int>, 9>, 9> myArrSq;
    for(int sq = 0; sq < 9; sq++) {
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r = 0; r < 3; r++) {
            for(int c = 0; c < 3; c++) {
                myArrSq[sq][index] = options[startRow + r][startCol + c];
                index++;
            }
        }
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsTwins, (void*) &myArrSq[sq]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(sq_threads[i], nullptr);
    }
}

void reduceOptionsTriplets(array<vector<int>, 9>& optionsForAll){
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

    // only keep options that are present in exactly 3 squares
    auto pair = optionsInSquares.begin();
    while (pair != optionsInSquares.end()){
        if(pair->second.size() != 3) {
            pair = optionsInSquares.erase(pair);
        } else {
            ++pair;
        }
    }

    // check for triplets
    auto firstPair = optionsInSquares.begin();
    bool found = false;

    while (firstPair != optionsInSquares.end()){
        auto secondPair = firstPair;
        ++secondPair;
        while (secondPair != optionsInSquares.end()){
            auto thirdPair = secondPair;
            ++thirdPair;
            while (thirdPair != optionsInSquares.end()){
                if (firstPair->second == secondPair->second && secondPair->second == thirdPair->second){
                    printf("--->found triplets!\n");
                    int squareIndex1 = firstPair->second[0];
                    int squareIndex2 = firstPair->second[1];
                    int squareIndex3 = firstPair->second[2];
                    vector<int> newOptions = {thirdPair->first, secondPair->first, firstPair->first};

                    optionsForAll[squareIndex1] = newOptions;
                    optionsForAll[squareIndex2] = newOptions;
                    optionsForAll[squareIndex3] = newOptions;
                    found = true;
                    break;
                }
                if (found) {
                    break;
                } else {
                    ++thirdPair;
                }
            }
            if (found){
                break;
            } else {
                ++secondPair;
            }
        }
        ++firstPair;
    }
}

void *reduceOptionsTriplets(void *arg){
    array<vector<int>, 9> *options = (array<vector<int>, 9> *) arg;
    reduceOptionsTriplets(*options);
    return NULL;
}

void reduceOptionsTriplets(array<array<vector<int>, 9>, 9>& options) {
    // all rows
    for(int r = 0; r < 9; r++) {
        reduceOptionsTriplets(options[r]);
    }

    // all cols
    for(int c = 0; c < 9; c++) {
        array<vector<int>, 9> myArr;
        for(int r = 0; r < 9; r++) {
            myArr[r] = options[r][c];
        }
        reduceOptionsTriplets(myArr);
    }

    // all subsquares
    for(int sq = 0; sq < 9; sq++) {
        array<vector<int>, 9> myArr;
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r = 0; r < 3; r++) {
            for(int c = 0; c < 3; c++) {
                myArr[index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        reduceOptionsTriplets(myArr);
    }
}

void reduceOptionsTripletsParallel(array<array<vector<int>, 9>, 9>& options) {
    pthread_t row_threads[9];
    // all rows
    for(int r = 0; r < 9; r++) {
        pthread_create(&row_threads[r], nullptr, reduceOptionsTriplets, (void*) &options[r]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(row_threads[i], nullptr);
    }

    // all cols
    pthread_t col_threads[9];
    array<array<vector<int>, 9>, 9> myArrCol;
    for(int c = 0; c < 9; c++){
        for(int r = 0; r < 9; r++) {
            myArrCol[c][r] = options[r][c];
        }
        pthread_create(&col_threads[c], nullptr, reduceOptionsTriplets, (void*) &myArrCol[c]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(col_threads[i], nullptr);
    }

    // all subsquares
    pthread_t sq_threads[9];    
    array<array<vector<int>, 9>, 9> myArrSq;
    for(int sq = 0; sq < 9; sq++) {
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r = 0; r < 3; r++) {
            for(int c = 0; c < 3; c++) {
                myArrSq[sq][index] = options[startRow + r][startCol + c];
                index++;
            }
        }
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsTriplets, (void*) &myArrSq[sq]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(sq_threads[i], nullptr);
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