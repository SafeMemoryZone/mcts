#ifndef NODE_HPP
#define NODE_HPP
#include "arena.hpp"
#include <cstddef>
#include <cstdint>

struct Node {
  int64_t eval;
  size_t visit_count;
  size_t win_count;
  Node *parent;
  uint16_t ai_board;
  uint16_t enemy_board;
  uint8_t child_count;
  Node* children[9];
  Arena* arena;

  Node(Arena *arena);
  Node *CalculateBestMove(size_t iter_count);
  bool IsTerminal();

private:
  Node *GetBestChild();
  Node *FindBestLeafNode();
  void CreateChildren();
  void SimulateAndPropagate();
  double GetUcbScore();
};
#endif // NODE_HPP 
