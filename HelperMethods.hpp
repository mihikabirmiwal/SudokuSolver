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