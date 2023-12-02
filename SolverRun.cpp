#include <iostream>
#include <getopt.h>
#include <fstream>
#include <array>
#include <vector>
#include "Solver.hpp"
#include "HelperMethods.hpp"

using namespace std;

enum Algorithms {
    BACKTRACKING,
    CROOKS,
    PARALLELIZED,
    ALL
};

void printTestCase(const std::array<std::vector<int>, 9>& testCase) {
    for (const auto& vec : testCase) {
        for (int value : vec) {
            std::cout << value << " ";
        }
        std::cout << "\t\t";  // Two tabs between vectors
    }
    std::cout << "\n";
}

int main(int argc, char **argv) {
    
    // ARGUMENT PARSING
    /* args:
        a - algorithm to run, options:
            0 - single-threaded backtracking
            1 - single-threaded crook's
            2 - parallelized algorithm
            3 - run all
    
        i - input file name with sudokus to solve

        o - output file name
    */
    
    int algorithm = 0;
    char* inputFilePath;
    char* outputFilePath;

    int opt = 0;
    while ((opt = getopt(argc, argv, "a:i:o:")) != -1) {
        switch (opt) {
            case 'a':
                algorithm = stoi(optarg);
                break;

            case 'i':
                inputFilePath = optarg;
                break;

            case 'o':
                outputFilePath = optarg;
                break;
            
            default:
                break;
        }
    }

    printf("\nArgs read are:\nalgorithm = %d\ninput_file = %s\noutput_file = %s\n\n", algorithm, inputFilePath, outputFilePath);

    // open input file
    ifstream input_file(inputFilePath);
    if (!input_file.is_open()) {
        cerr << "Error in opening input file" << endl;
        return 1;
    }
    
    // open output file
    ofstream output_file(outputFilePath);
    if (!output_file.is_open()) {
        cerr << "Error in opening output file" << endl;
        return 1;
    }
    
    // read input file and store sudokus
    vector<array<array<int, 9>, 9>> testCases;

    char next_char = input_file.peek();
    char c;

    while (next_char != EOF) {
        if (next_char == 'G') {
            string line;
            getline(input_file, line);
        } else {
            array<array<int, 9>, 9> currSudoku;
            for (int row = 0; row < 9; row++){
                for (int col = 0; col < 9; col++){
                    input_file.get(c);
                    int i = int(c - '0');
                    currSudoku[row][col] = i;
                }
                input_file.get(c); // newline character
            }
            testCases.push_back(currSudoku);
        }
        next_char = input_file.peek();
    }
    
    for (auto& testCase : testCases) {
        // printf("BEFORE SOLVING\n");
        // printBoard(testCase);
        // array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        // // printOptions(allOptions);
        // reduceOptionsElimination(allOptions, testCase);
        // // printOptions(allOptions);
        // reduceOptionsLoneRanger(allOptions);
        // // printOptions(allOptions);
        // bool x = pureBacktracking(testCase, allOptions);
        // printf("AFTER SOLVING\n");
        // printBoard(testCase);


        testingTwins(testCase);
        // just to do the first one
        // break;
    }    
}
