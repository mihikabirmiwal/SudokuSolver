#include <unordered_set>
#include <cmath>
using namespace std;

// const int NUM_GRID_SIZE = 25;

bool isValidSolution(const array<array<int, NUM_GRID_SIZE>, NUM_GRID_SIZE>& solved) {
    // check rows
    for(int r=0;r<NUM_GRID_SIZE;r++) {
        unordered_set<int> rowSet;
        for(int c=0;c<NUM_GRID_SIZE;c++) rowSet.insert(solved[r][c]);
        if(rowSet.size()!=NUM_GRID_SIZE) return false;
    }
    // check cols 
    for(int c=0;c<NUM_GRID_SIZE;c++){
        unordered_set<int> colSet;
        for(int r=0;r<NUM_GRID_SIZE;r++) colSet.insert(solved[r][c]);
        if(colSet.size()!=NUM_GRID_SIZE) return false;
    }
    // check subsquare
    for(int sq=0;sq<NUM_GRID_SIZE;sq++) {
        unordered_set<int> sqSet;
        double num = NUM_GRID_SIZE;
        int subsquare = static_cast<int>(sqrt(num));
        int startRow = sq/subsquare;
        int startCol = sq%subsquare;
        startRow *= subsquare;
        startCol *= subsquare;
        for(int r=0;r<subsquare;r++) {
            for(int c=0;c<subsquare;c++) {
                sqSet.insert(solved[startRow+r][startCol+c]);
            }
        }
        if(sqSet.size()!=NUM_GRID_SIZE) return false;
    }
    // passed all checks
    return true;
}