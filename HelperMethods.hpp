#include <vector>
#include <array>

using namespace std;

// PRINTING FUNCTIONS

void printBoard(const array<array<int, 9>, 9>& grid) {
    for (const auto& row : grid) {
        for (const auto& element : row) {
            printf("%d ", element);
        }
        printf("\n");
    }
    printf("-----------\n"); // Add a separator between 2D arrays
}

void printBoard(array<array<int, 9>, 9>& board, ostream& output) {
    for (const auto& row : board) {
        for (int value : row) {
            output << value << " ";
        }
        output << "\n";
    }
}

void printOptions(const array<array<vector<int>, 9>, 9>& options) {
    printf("PRINTING OPTIONS\n");
    for(const auto& row: options) {
        for(const auto& col: row) {
            for(int i=0;i<col.size();i++) {
                printf("%d ", col[i]);
            }
            printf("\n");
        }
    }
    printf("-----------\n");
}

// TESTING FUNCTIONS

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


void testingTwins(array<array<int, 9>, 9>& testCase, ostream& output, int numThreads) {
    printf("testingTwins: New grid\n");
    output << "\n\n~~~~~~~~~~~~~~~~~~\n";
    array<array<int, 9>, 9> firstRun = testCase;
    output << "TESTING TWINS:\n\n";
    output << "Original:\n";
    printBoard(firstRun, output);
    array<array<vector<int>, 9>, 9> allOptions = getOptions(firstRun);
    reduceOptionsElimination(allOptions, firstRun, numThreads);
    bool x = pureBacktracking(firstRun, allOptions);
    output << "After elimination + backtracking:\n";
    printBoard(firstRun, output);

    array<array<int, 9>, 9> secondRun = testCase;
    allOptions = getOptions(secondRun);
    reduceOptionsElimination(allOptions, secondRun, numThreads);
    reduceOptionsTwinsParallel(allOptions); // parallel
    // reduceOptionsTwins(allOptions); // sequential
    x = pureBacktracking(secondRun, allOptions);
    output << "After elimination + twins + backtracking:\n";
    printBoard(secondRun, output);
}

void testingTriplets(array<array<int, 9>, 9>& testCase, ostream& output, int numThreads) {
    printf("testingTriplets: New grid\n");
    output << "\n\n~~~~~~~~~~~~~~~~~~\n";
    array<array<int, 9>, 9> firstRun = testCase;
    output << "TESTING TRIPLETS:\n\n";
    output << "Original:\n";
    printBoard(firstRun, output);
    array<array<vector<int>, 9>, 9> allOptions = getOptions(firstRun);
    reduceOptionsElimination(allOptions, firstRun, numThreads);
    bool x = pureBacktracking(firstRun, allOptions);
    output << "After elimination + backtracking:\n";
    printBoard(firstRun, output);

    array<array<int, 9>, 9> secondRun = testCase;
    allOptions = getOptions(secondRun);
    reduceOptionsElimination(allOptions, secondRun, numThreads);
    reduceOptionsTripletsParallel(allOptions); // parallel
    // reduceOptionsTriplets(allOptions); // sequential
    x = pureBacktracking(secondRun, allOptions);
    output << "After elimination + triplets + backtracking:\n";
    printBoard(secondRun, output);
}

array<vector<int>, 9> twinsTester() {
    array<vector<int>, 9> test_case1;
    vector<int> square1 = {1, 2, 8};
    vector<int> square2 = {3};
    vector<int> square3 = {1, 4};
    vector<int> square4 = {5, 7, 8};
    vector<int> square5 = {6};
    vector<int> square6 = {4, 5, 7};
    vector<int> square7 = {1, 2, 8};
    vector<int> square8 = {9};
    vector<int> square9 = {1, 2};

    test_case1[0] = square1;
    test_case1[1] = square2;
    test_case1[2] = square3;
    test_case1[3] = square4;
    test_case1[4] = square5;
    test_case1[5] = square6;
    test_case1[6] = square7;
    test_case1[7] = square8;
    test_case1[8] = square9;

    return test_case1;
}

array<vector<int>, 9> tripletsTester() {
    array<vector<int>, 9> test_case1;
    vector<int> square1 = {3, 4};
    vector<int> square2 = {2};
    vector<int> square3 = {3, 4, 8};
    vector<int> square4 = {3, 6, 8};
    vector<int> square5 = {1, 3, 5, 7};
    vector<int> square6 = {3, 4, 8};
    vector<int> square7 = {1, 5, 7, 8};
    vector<int> square8 = {9};
    vector<int> square9 = {1, 5, 6, 7};

    test_case1[0] = square1;
    test_case1[1] = square2;
    test_case1[2] = square3;
    test_case1[3] = square4;
    test_case1[4] = square5;
    test_case1[5] = square6;
    test_case1[6] = square7;
    test_case1[7] = square8;
    test_case1[8] = square9;

    return test_case1;
}