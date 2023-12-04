using namespace std;

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