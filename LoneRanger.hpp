#include <cmath>
using namespace std;

// const int NUM_GRID_SIZE = 25;

struct LoneRangerArgs {
    array<vector<int>, NUM_GRID_SIZE>& options;
};

void reduceOptionsLoneRangerHelper(array<vector<int>, NUM_GRID_SIZE>& options) {
    // initialize hashmap
    unordered_map<int, vector<int>> found;
    for(int i=1;i<=NUM_GRID_SIZE;i++) {
        found[i] = {};
    }
    // populate hashmap
    for(int index=0;index<NUM_GRID_SIZE;index++) {
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
            options[index].clear();
            options[index].push_back(value);
        }
    }
}

void *reduceOptionsLoneRangerMultiHelper(void *arg) {
    array<vector<int>, NUM_GRID_SIZE> *options = (array<vector<int>, NUM_GRID_SIZE> *) arg;
    reduceOptionsLoneRangerHelper(*options);
    return NULL;
}

// multi threaded
void reduceOptionsLoneRangerMulti(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    // all rows
    pthread_t row_threads[NUM_GRID_SIZE];
    for(int row=0;row<NUM_GRID_SIZE;row++) {
        pthread_create(&row_threads[row], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &options[row]);
    }
    for(int i=0;i<NUM_GRID_SIZE;i++) {
        pthread_join(row_threads[i], nullptr);
    }
    // all cols
    pthread_t col_threads[NUM_GRID_SIZE];
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> myArrCol;
    for(int col=0;col<NUM_GRID_SIZE;col++) {
        for(int r=0;r<NUM_GRID_SIZE;r++) {
            myArrCol[col][r] = options[r][col];
        }
        pthread_create(&col_threads[col], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &myArrCol[col]);
    }
    for(int i=0;i<NUM_GRID_SIZE;i++) {
        pthread_join(col_threads[i], nullptr);
    }
    // all subsquares
    pthread_t sq_threads[NUM_GRID_SIZE];
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE> myArrSq;

    double num = NUM_GRID_SIZE;
    int subsquare = static_cast<int>(sqrt(num));
    
    for(int sq=0;sq<NUM_GRID_SIZE;sq++) {
        int startRow = sq/subsquare;
        int startCol = sq%subsquare;
        startRow *= subsquare;
        startCol *= subsquare;
        int index = 0;
        for(int r=0;r<subsquare;r++) {
            for(int c=0;c<subsquare;c++) {
                myArrSq[sq][index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &myArrSq[sq]);
    }
    for(int i=0;i<NUM_GRID_SIZE;i++) {
        pthread_join(sq_threads[i], nullptr);
    }
}

// single threaded
void reduceOptionsLoneRangerSingle(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options) {
    // all rows
    for(int row=0;row<NUM_GRID_SIZE;row++) {
        reduceOptionsLoneRangerHelper(options[row]);
    }
    // all cols
    for(int col=0;col<NUM_GRID_SIZE;col++) {
        array<vector<int>, NUM_GRID_SIZE> myArr;
        for(int r=0;r<NUM_GRID_SIZE;r++) {
            myArr[r] = options[r][col];
        }
        reduceOptionsLoneRangerHelper(myArr);
    }
    // all subsquares
    double num = NUM_GRID_SIZE;
    int subsquare = static_cast<int>(sqrt(num));
    for(int sq=0;sq<NUM_GRID_SIZE;sq++) {
        array<vector<int>, NUM_GRID_SIZE> myArr;
        int startRow = sq/subsquare;
        int startCol = sq%subsquare;
        startRow *= subsquare;
        startCol *= subsquare;
        int index = 0;
        for(int r=0;r<subsquare;r++) {
            for(int c=0;c<subsquare;c++) {
                myArr[index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        reduceOptionsLoneRangerHelper(myArr);
    }
}