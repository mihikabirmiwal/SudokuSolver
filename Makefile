all:
	g++ -O3 -o solver SolverRun.cpp -pthread
single:
	make all
	./solver -a 0 -i few_problems.txt -o output.txt -t 1
multi:
	make all
	./solver -a 0 -i few_problems.txt -o output.txt -t 10
tests:
	make all
	./solver -a 0 -i test_problems.txt -o output_all_9x9_tests.txt -t 0
big:
	make all
	./solver -a 0 -i 25x25_test.txt -o output_25x25.txt -t 0
medium:
	make all
	./solver -a 0 -i 16x16_test.txt -o output16x16.txt -t 0