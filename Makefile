all:
	clang++ -o mcts -fsanitize=undefined src/main.cc src/arena.cc src/node.cc
