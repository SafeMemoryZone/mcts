all:
	clang++ -o mcts -g -std=c++11 -fsanitize=undefined src/main.cc src/node.cc
