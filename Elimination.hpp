using namespace std;

struct EliminationArgs {
    array<array<vector<int>, 9>, 9>& options;
    const array<array<int, 9>, 9>& grid;
    int start;
    int end;
};

void* reduceOptionsEliminationHelper(void* arg) {
    EliminationArgs* threadArgs = static_cast<EliminationArgs*>(arg);
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
        EliminationArgs* args = new EliminationArgs{options, grid, startCalc, endCalc};
        pthread_create(&threads[i], nullptr, reduceOptionsEliminationHelper, static_cast<void*>(args));
        startCalc = endCalc;
    }
    // wait on all threads to finish
    for(int i=0;i<numThreads;i++) {
        pthread_join(threads[i], nullptr);
    }
}