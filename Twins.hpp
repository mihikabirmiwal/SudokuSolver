using namespace std;

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