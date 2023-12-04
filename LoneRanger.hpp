using namespace std;

struct LoneRangerArgs {
    array<vector<int>, 9>& options;
};

void reduceOptionsLoneRangerHelper(array<vector<int>, 9>& options) {
    // initialize hashmap
    unordered_map<int, vector<int>> found;
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
            options[index].clear();
            options[index].push_back(value);
        }
    }
}

void *reduceOptionsLoneRangerMultiHelper(void *arg) {
    array<vector<int>, 9> *options = (array<vector<int>, 9> *) arg;
    reduceOptionsLoneRangerHelper(*options);
    return NULL;
}

// multi threaded
void reduceOptionsLoneRangerMulti(array<array<vector<int>, 9>, 9>& options) {
    // all rows
    pthread_t row_threads[9];
    for(int row=0;row<9;row++) {
        pthread_create(&row_threads[row], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &options[row]);
    }
    for(int i=0;i<9;i++) {
        pthread_join(row_threads[i], nullptr);
    }
    // all cols
    pthread_t col_threads[9];
    array<array<vector<int>, 9>, 9> myArrCol;
    for(int col=0;col<9;col++) {
        for(int r=0;r<9;r++) {
            myArrCol[col][r] = options[r][col];
        }
        pthread_create(&col_threads[col], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &myArrCol[col]);
    }
    for(int i=0;i<9;i++) {
        pthread_join(col_threads[i], nullptr);
    }
    // all subsquares
    pthread_t sq_threads[9];
    array<array<vector<int>, 9>, 9> myArrSq;
    for(int sq=0;sq<9;sq++) {
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        int index = 0;
        for(int r=0;r<3;r++) {
            for(int c=0;c<3;c++) {
                myArrSq[sq][index] = options[startRow+r][startCol+c];
                index++;
            }
        }
        pthread_create(&sq_threads[sq], nullptr, reduceOptionsLoneRangerMultiHelper, (void*) &myArrSq[sq]);
    }
    for(int i=0;i<9;i++) {
        pthread_join(sq_threads[i], nullptr);
    }
}

// single threaded
void reduceOptionsLoneRangerSingle(array<array<vector<int>, 9>, 9>& options) {
    // all rows
    for(int row=0;row<9;row++) {
        reduceOptionsLoneRangerHelper(options[row]);
    }
    // all cols
    for(int col=0;col<9;col++) {
        array<vector<int>, 9> myArr;
        for(int r=0;r<9;r++) {
            myArr[r] = options[r][col];
        }
        reduceOptionsLoneRangerHelper(myArr);
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
        reduceOptionsLoneRangerHelper(myArr);
    }
}