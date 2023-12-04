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
	./solver -a 0 -i test_problems.txt -o output.txt -t 0