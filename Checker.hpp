#include <unordered_set>
using namespace std;

bool isValidSolution(const array<array<int, 9>, 9>& solved) {
    // check rows
    for(int r=0;r<9;r++) {
        unordered_set<int> rowSet;
        for(int c=0;c<9;c++) rowSet.insert(solved[r][c]);
        if(rowSet.size()!=9) return false;
    }
    // check cols 
    for(int c=0;c<9;c++){
        unordered_set<int> colSet;
        for(int r=0;r<9;r++) colSet.insert(solved[r][c]);
        if(colSet.size()!=9) return false;
    }
    // check subsquare
    for(int sq=0;sq<9;sq++) {
        unordered_set<int> sqSet;
        int startRow = sq/3;
        int startCol = sq%3;
        startRow *= 3;
        startCol *= 3;
        for(int r=0;r<3;r++) {
            for(int c=0;c<3;c++) {
                sqSet.insert(solved[startRow+r][startCol+c]);
            }
        }
        if(sqSet.size()!=9) return false;
    }
    // passed all checks
    return true;
}