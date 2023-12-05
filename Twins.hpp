#include <cmath>

using namespace std;

// const int NUM_GRID_SIZE = 25;

void reduceOptionsTwins(array<vector<int>, NUM_GRID_SIZE>& optionsForAll) {
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
                // printf("testingTwins: --->found twins!\n");
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
    array<vector<int>, NUM_GRID_SIZE> *options = (array<vector<int>, NUM_GRID_SIZE> *) arg;
    reduceOptionsTwins(*options);
    return NULL;
}

void reduceOptionsTwins(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    // all rows
    for(int r = 0; r < NUM_GRID_SIZE; r++) {
        reduceOptionsTwins(options[r]);
    }

    // all cols
    for(int c = 0; c < NUM_GRID_SIZE; c++) {
        array<vector<int>, NUM_GRID_SIZE> myArr;
        for(int r = 0; r < NUM_GRID_SIZE; r++) {
            myArr[r] = options[r][c];
        }
        reduceOptionsTwins(myArr);
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
        reduceOptionsTwins(myArr);
    }
}

void reduceOptionsTwinsParallel(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    // all rows
    pthread_t row_threads[NUM_GRID_SIZE];
    for(int r = 0; r < NUM_GRID_SIZE; r++){
        pthread_create(&row_threads[r], nullptr, reduceOptionsTwins, (void*) &options[r]);
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
        pthread_create(&col_threads[c], nullptr, reduceOptionsTwins, (void*) &myArrCol[c]);
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
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsTwins, (void*) &myArrSq[sq]);
    }
    for (int i = 0; i < NUM_GRID_SIZE; i++) {
        pthread_join(sq_threads[i], nullptr);
    }
}