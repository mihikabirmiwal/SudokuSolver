#include <iostream>
#include <getopt.h>
#include <fstream>
#include <array>
#include <vector>
#include "Solver.hpp"

using namespace std;

enum Algorithms {
    BACKTRACKING,
    CROOKS,
    PARALLELIZED,
    ALL
};

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

    printf("Args read are:\nalgorithm = %d\ninput_file = %s\noutput_file = %s\n", algorithm, inputFilePath, outputFilePath);

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
    
    // TODO read the file, create test cases
    vector<array<array<int, 9>, 9>> testCases;

    char* line;
    chat c;
    int index = 0;

    while (getline(input_file, line)) {
        if (strncmp(line, "Grid", 4) == 0) {
            input_file.get(c); // newline character
            for (int row = 0; row < 9; row++){
                for (int col = 0; col < 9; col++){
                    input_file.get(c);
                    int i = stoi(c);
                    testCases[index][row][col] = i;
                }
                input_file.get(c); // newline character
            }
        }
    }


    if(algorithm==Algorithms.BACKTRACKING) {
        
    }
    
}
