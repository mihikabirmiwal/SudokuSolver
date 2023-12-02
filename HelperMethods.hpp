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

// void printOptionsInSquares(const std::unordered_map<int, std::vector<int>>& optionsInSquares) {
//     for (const auto& pair : optionsInSquares) {
//         std::cout << pair.first << "=";

//         // Print vector elements separated by commas
//         for (size_t i = 0; i < pair.second.size(); ++i) {
//             std::cout << pair.second[i];
//             if (i < pair.second.size() - 1) {
//                 std::cout << ",";
//             }
//         }

//         std::cout << "\n";
//     }
// }

void printOptionsPerGrouping(const array<vector<int>, 9>& optionsForAll) {
    for (size_t i = 0; i < optionsForAll.size(); ++i) {
        std::cout << "Square " << i + 1 << ": ";
        for (const int& value : optionsForAll[i]) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
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


void testingTwins(array<array<int, 9>, 9>& testCase) {
    printf("\n\n~~~~~~~~~~~~~~~~~~\n");
    array<std::array<int, 9>, 9> firstRun = testCase;
    printf("TESTING TWINS:\n\n");
    printf("Original:\n");
    printBoard(firstRun);
    array<array<vector<int>, 9>, 9> allOptions = getOptions(firstRun);
    reduceOptionsElimination(allOptions, firstRun);
    bool x = pureBacktracking(firstRun, allOptions);
    printf("After elimination + backtracking:\n");
    printBoard(firstRun);

    array<std::array<int, 9>, 9> secondRun = testCase;
    allOptions = getOptions(secondRun);
    reduceOptionsElimination(allOptions, secondRun);
    reduceOptionsTwins(allOptions);
    x = pureBacktracking(secondRun, allOptions);
    printf("After elimination + twins + backtracking:\n");
    printBoard(secondRun);
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