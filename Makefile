all:
	g++ -O3 -o solver SolverRun.cpp -pthread
basic:
	make all
<<<<<<< HEAD
	./solver -a 0 -i test_problems.txt -o output.txt
=======
	./solver -a 0 -i few_problems.txt -o output.txt -t 10
>>>>>>> 6867feb552fa6a355e8eaf750efefbd7869833ac
