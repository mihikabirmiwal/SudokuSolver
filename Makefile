all:
	g++ -O3 -o solver SolverRun.cpp -pthread
basic:
	make all
	./solver -a 0 -i few_problems.txt -o output.txt
