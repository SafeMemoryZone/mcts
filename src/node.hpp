#ifndef NODE_HPP
#define NODE_HPP
#include <cstddef>
#include <cstdint>
#include <deque>

struct Node {
  int64_t eval;
  size_t visit_count;
  Node *parent;
  uint16_t ai_board;
  uint16_t enemy_board;
  uint8_t child_count;
  Node *children[9];
  std::deque<Node>* arena;
  bool is_ai_turn;

  Node(std::deque<Node> *arena, bool is_ai_turn);
  Node *CalculateBestMove(size_t iter_count);
  Node *SearchEnemyMove(int move);
  int GetWinner();

private:
  Node *GetBestChild(double c);
  Node *FindBestLeafNode();
  void CreateChildren();
  void SimulateAndBackpropagate();
  double GetUcbScore(double c);
};
#endif // NODE_HPP 
