#include <cmath>

using namespace std;

// const int NUM_GRID_SIZE = 25;

struct EliminationArgs {
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options;
    const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid;
    int start;
    int end;
};

void* reduceOptionsEliminationHelper(void* arg) {
    EliminationArgs* threadArgs = static_cast<EliminationArgs*>(arg);
    array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options = threadArgs->options;
    const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid = threadArgs->grid;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for(int j=start;j<end;j++) {
        int r = j/NUM_GRID_SIZE;
        int c = j%NUM_GRID_SIZE;
        vector<int>& option = options[r][c];
        if(grid[r][c]==0) {
            // check row
            for(int i=0;i<NUM_GRID_SIZE;i++) {
                // number to remove
                int num = grid[r][i];
                // index of number to remove
                auto numInd = find(option.begin(), option.end(), num);
                // remove if value was found
                if(numInd != option.end()) option.erase(numInd);
                
            }

            // check col
            for(int i=0;i<NUM_GRID_SIZE;i++) {
                // number to remove
                int num = grid[i][c];
                // index of number to remove
                auto numInd = find(option.begin(), option.end(), num);
                // remove if value was found
                if(numInd != option.end()) option.erase(numInd);
            }

            // check small square
            double num = NUM_GRID_SIZE;
            int subsquare = static_cast<int>(sqrt(num));
            int startRow = r - r%subsquare;
            int startCol = c - c%subsquare;
            for(int i=0;i<subsquare;i++) {
                for(int j=0;j<subsquare;j++) {
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

void reduceOptionsElimination(array<array<vector<int>, NUM_GRID_SIZE>, NUM_GRID_SIZE>& options, const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& grid, int numThreads) {
    pthread_t threads[numThreads];
    if(numThreads>(NUM_GRID_SIZE*NUM_GRID_SIZE)) numThreads = NUM_GRID_SIZE*NUM_GRID_SIZE;
    int expectedLoad = NUM_GRID_SIZE*NUM_GRID_SIZE/numThreads;
    int extraLoad = NUM_GRID_SIZE*NUM_GRID_SIZE%numThreads;
    int startCalc = 0;
    for(int i=0;i<numThreads;i++) {
        int endCalc = startCalc+expectedLoad;
        if(extraLoad>0) {
            endCalc++;
            extraLoad--;
        }
        EliminationArgs* args = new EliminationArgs{options, grid, startCalc, endCalc};
        pthread_create(&threads[i], nullptr, reduceOptionsEliminationHelper, static_cast<void*>(args));
        startCalc = endCalc;
    }
    // wait on all threads to finish
    for(int i=0;i<numThreads;i++) {
        pthread_join(threads[i], nullptr);
    }
}