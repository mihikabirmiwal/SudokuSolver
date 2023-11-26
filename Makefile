all:
	g++ -O3 -o solver SolverRun.cpp -pthread
basic:
	make all
	./solver -a 0 -i test_problems.txt -o output.txt
