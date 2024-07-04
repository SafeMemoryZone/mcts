#include "arena.hpp"
#include "node.hpp"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <random>

Node::Node(Arena *arena) {
  this->arena = arena;
  this->child_count = 0;
  this->visit_count = 0;
  this->win_count = 0;
  this->eval = 0;
  this->parent = nullptr;
}

Node *Node::GetBestChild() {
  double best_score = -1;
  Node* best_child;

  for (auto i = 0; i < this->child_count; i++) {
    auto score = this->children[i]->GetUcbScore();
    if (score > best_score) {
      best_score = score;
      best_child = this->children[i];
    }
  }

  return best_child;
}

Node *Node::FindBestLeafNode() {
  if (this->child_count == 0)
    return this;

  return this->GetBestChild()->FindBestLeafNode();
}

void Node::CreateChildren() {
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      int position = row * 3 + col;
      bool p1 = this->ai_board & (1 << position);
      bool p2 = this->enemy_board & (1 << position);
      if (!p1 && !p2) {
        Node *child = (Node *)this->arena->Alloc(sizeof(Node));
        new (child) Node(this->arena);  // Placement new to initialize
        child->ai_board = this->ai_board;
        child->enemy_board = this->enemy_board;
        child->parent = this;
        // TODO: make move
        this->children[this->child_count++] = child;
      }
    }
  }
}

void Node::Simulate() {
  // TODO: implement
}

Node *Node::CalculateBestMove(size_t iter_count) {
  for (size_t i = 0; i < iter_count; i++) {
    Node *leaf = this->FindBestLeafNode();

    if (!IsTerminal(leaf)) {
      leaf->CreateChildren();
    }

    if (leaf->child_count > 0) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, leaf->child_count - 1);

      Node *chosen_leaf = leaf->children[dis(gen)];

      chosen_leaf->Simulate();
    }
  }

  int64_t best_eval = -1;
  Node *best_node;

  for (int i = 0; i < this->child_count; i++) {
    if (this->children[i]->eval > best_eval) {
      best_eval = this->children[i]->eval;
      best_node = this->children[i];
    }
  }

  return best_node;
}

bool Node::IsTerminal(Node *n) {
  // TODO: implement
  return false;
}

double Node::GetUcbScore() {
  auto parent = this->parent ? this->parent : this;

  if (this->visit_count == 0)
    return DBL_MAX;

  constexpr double c = 1.4;

  double exploitation = (double)this->win_count / this->visit_count;
  double exploration = c * sqrt(log(parent->visit_count) / this->visit_count);

  return exploration + exploitation;
}
