#include <iostream>
#include <getopt.h>
#include <fstream>
#include <array>
#include <vector>
#include "Solver.hpp"
#include "HelperMethods.hpp"
#include "Elimination.hpp"
#include "LoneRanger.hpp"
#include "Twins.hpp"
#include "Triplets.hpp"
#include <chrono>

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
    int numThreads = 1;

    int opt = 0;
    while ((opt = getopt(argc, argv, "a:i:o:t:")) != -1) {
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

            case 't':
                numThreads = stoi(optarg);
                break;
            
            default:
                break;
        }
    }

    printf("\nArgs read are:\nalgorithm = %d\ninput_file = %s\noutput_file = %s\nnumThreads=%d\n\n", algorithm, inputFilePath, outputFilePath, numThreads);

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




    // Test, time, and write to output


    // - bt (sequential)    
    printf("doing seq bt\n");
    output_file << "---- Testing sequential backtracking ----\n";
    vector<array<array<int, 9>, 9>> testCasesSeqBt = testCases;

    auto start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesSeqBt) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        bool x = pureBacktracking(testCase, allOptions);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";


    // - bt+elimination (sequential)
    printf("doing seq bt + el\n");
    output_file << "---- Testing sequential backtracking + elimination ----\n";
    vector<array<array<int, 9>, 9>> testCasesSeqBtEl = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesSeqBtEl) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, 1);       
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";


    // - bt+elimination (multithread) → find fastest num of threads [1-81]
    printf("doing multi bt + el\n");
    output_file << "---- Testing multithreaded backtracking + elimination ----\n";
    array<chrono::duration<double>, 81> durations;

    for (int num_threads = 1; num_threads <= 81; num_threads++) {
        vector<array<array<int, 9>, 9>> testCasesMultiBtEl = testCases;
        start = chrono::high_resolution_clock::now();
        for (auto& testCase : testCasesMultiBtEl) {
            array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
            reduceOptionsElimination(allOptions, testCase, num_threads);       
            bool x = pureBacktracking(testCase, allOptions);
        }
        end = chrono::high_resolution_clock::now();
        durations[num_threads - 1] = end - start;
    }

    for (int i = 0; i < 81; i++){
        output_file << "Time to solve all sudokus with " << i + 1 << " threads: " << durations[i].count() << " s\n";
    }
    output_file << "\n";

    // Find shortest duration
    auto shortestDuration = min_element(durations.begin(), durations.end());

    // Calculate index
    int fastestNumThreads = distance(durations.begin(), shortestDuration);
    output_file << "The number of threads with the shortest duration is " << fastestNumThreads << " with " << shortestDuration->count() << " s\n\n";

        // ---


    // - bt+elimination+triplets (sequential)
    printf("doing seq bt + el + tri\n");
    output_file << "---- Testing sequential backtracking + elimination + triplets ----\n";
    vector<array<array<int, 9>, 9>> testCasesSeqBtElTri = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesSeqBtElTri) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, 1);  
        reduceOptionsTriplets(allOptions);
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";

    
    
    // - bt+elimination+triplets (multithread)
    printf("doing multi bt + el + tri\n");
    output_file << "---- Testing multithreaded backtracking + elimination + triplets ----\n";
    vector<array<array<int, 9>, 9>> testCasesMultiBtElTri = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesMultiBtElTri) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, fastestNumThreads);  
        // printf("what\n");
        reduceOptionsTripletsParallel(allOptions);
        // printf("getting through\n");
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";


    // - bt+elimination+triplets+twins (sequential)
    printf("doing seq bt + el + tri + twin\n");
    output_file << "---- Testing sequential backtracking + elimination + triplets + twins ----\n";
    vector<array<array<int, 9>, 9>> testCasesSeqBtElTriTwi = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesSeqBtElTriTwi) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, 1);  
        reduceOptionsTriplets(allOptions);
        reduceOptionsTwins(allOptions);
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";


    
    // - bt+elimination+triplets+twins (multithread)
    printf("doing multi bt + el + tri + twin\n");
    output_file << "---- Testing multithread backtracking + elimination + triplets + twins ----\n";
    vector<array<array<int, 9>, 9>> testCasesMultiBtElTriTwi = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesMultiBtElTriTwi) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, fastestNumThreads);  
        reduceOptionsTripletsParallel(allOptions);
        reduceOptionsTwinsParallel(allOptions);
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";



        // - bt+elimination+triplets+twins+lone rangers (sequential)
    printf("doing seq bt + el + tri + twin + lr\n");
    output_file << "---- Testing sequential backtracking + elimination + triplets + twins + lone rangers ----\n";
    vector<array<array<int, 9>, 9>> testCasesSeqBtElTriTwiLr = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesSeqBtElTriTwiLr) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, 1);  
        reduceOptionsTriplets(allOptions);
        reduceOptionsTwins(allOptions);
        reduceOptionsLoneRangerSingle(allOptions);
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n";



        // - bt+elimination+triplets+twins+lone rangers (multithread)
    printf("doing seq bt + el + tri + twin + lr\n");
    output_file << "---- Testing multithread backtracking + elimination + triplets + twins + lone rangers ----\n";
    vector<array<array<int, 9>, 9>> testCasesMultiBtElTriTwiLr = testCases;

    start = chrono::high_resolution_clock::now();
    for (auto& testCase : testCasesMultiBtElTriTwiLr) {
        array<array<vector<int>, 9>, 9> allOptions = getOptions(testCase);
        reduceOptionsElimination(allOptions, testCase, fastestNumThreads);  
        reduceOptionsTripletsParallel(allOptions);
        reduceOptionsTwinsParallel(allOptions);
        reduceOptionsLoneRangerMulti(allOptions);
        bool x = pureBacktracking(testCase, allOptions);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::duration<double>>(end - start);

    output_file << "Time to solve all sudokus: " << duration.count() << " s\n";
    output_file << "\n"; 
}
