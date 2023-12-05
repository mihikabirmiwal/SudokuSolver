#include <cmath>

using namespace std;

// const int NUM_GRID_SIZE = 25;

void reduceOptionsTriplets(array<vector<int>, NUM_GRID_SIZE>& optionsForAll){
    // initialize map
    unordered_map<int, vector<int>> optionsInSquares;
    for(int i = 1; i <= NUM_GRID_SIZE ; i++) {
        optionsInSquares[i] = {};
    }

    // populate map
    for(int i = 0; i < NUM_GRID_SIZE ; i++) {
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
                    // printf("--->found triplets!\n");
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
    array<vector<int>, NUM_GRID_SIZE> *options = (array<vector<int>, NUM_GRID_SIZE> *) arg;
    reduceOptionsTriplets(*options);
    return NULL;
}

void reduceOptionsTriplets(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    // all rows
    for(int r = 0; r < NUM_GRID_SIZE; r++) {
        reduceOptionsTriplets(options[r]);
    }

    // all cols
    for(int c = 0; c < NUM_GRID_SIZE; c++) {
        array<vector<int>, NUM_GRID_SIZE> myArr;
        for(int r = 0; r < NUM_GRID_SIZE; r++) {
            myArr[r] = options[r][c];
        }
        reduceOptionsTriplets(myArr);
    }

    // all subsquares
    double num = NUM_GRID_SIZE;
    int subsquare = static_cast<int>(sqrt(num));
    for(int sq = 0; sq < NUM_GRID_SIZE; sq++) {
        array<vector<int>, NUM_GRID_SIZE> myArr;
        int startRow = sq/subsquare;
        int startCol = sq%subsquare;
        startRow *= subsquare;
        startCol *= subsquare;
        int index = 0;
        for(int r = 0; r < subsquare; r++) {
            for(int c = 0; c < subsquare; c++) {
                myArr[index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        reduceOptionsTriplets(myArr);
    }
}

void reduceOptionsTripletsParallel(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    pthread_t row_threads[NUM_GRID_SIZE];
    // all rows
    for(int r = 0; r < NUM_GRID_SIZE; r++) {
        pthread_create(&row_threads[r], nullptr, reduceOptionsTriplets, (void*) &options[r]);
    }

    for (int i = 0; i < NUM_GRID_SIZE; i++) {
        pthread_join(row_threads[i], nullptr);
    }

    // all cols
    pthread_t col_threads[NUM_GRID_SIZE];
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> myArrCol;
    for(int c = 0; c < NUM_GRID_SIZE; c++){
        for(int r = 0; r < NUM_GRID_SIZE; r++) {
            myArrCol[c][r] = options[r][c];
        }
        pthread_create(&col_threads[c], nullptr, reduceOptionsTriplets, (void*) &myArrCol[c]);
    }

    for (int i = 0; i < NUM_GRID_SIZE; i++) {
        pthread_join(col_threads[i], nullptr);
    }

    // all subsquares
    double num = NUM_GRID_SIZE;
    int subsquare = static_cast<int>(sqrt(num));
    pthread_t sq_threads[NUM_GRID_SIZE];    
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> myArrSq;
    for(int sq = 0; sq < NUM_GRID_SIZE; sq++) {
        int startRow = sq/subsquare;
        int startCol = sq%subsquare;
        startRow *= subsquare;
        startCol *= subsquare;
        int index = 0;
        for(int r = 0; r < subsquare; r++) {
            for(int c = 0; c < subsquare; c++) {
                myArrSq[sq][index] = options[startRow + r][startCol + c];
                index++;
            }
        }
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsTriplets, (void*) &myArrSq[sq]);
    }

    for (int i = 0; i < NUM_GRID_SIZE; i++) {
        pthread_join(sq_threads[i], nullptr);
    }
}