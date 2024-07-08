#include "node.hpp"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <random>
#include <vector>

Node::Node(std::deque<Node> *arena, bool is_ai_turn) : eval(0), visit_count(0), parent(nullptr), ai_board(0), 
  enemy_board(0), child_count(0), children{0}, arena(arena), is_ai_turn(is_ai_turn) {}

Node *Node::GetBestChild(double c) {
  double best_score = -DBL_MAX;
  Node* best_child = nullptr;
  assert(this->child_count > 0);

  for (auto i = 0; i < this->child_count; i++) {
    auto score = this->children[i]->GetUcbScore(c);
    if (score > best_score) {
      best_score = score;
      best_child = this->children[i];
    }
  }

  assert(best_child != nullptr);
  return best_child;
}

Node *Node::FindBestLeafNode() {
  if (this->child_count == 0)
    return this;

  return this->GetBestChild(1.44)->FindBestLeafNode();
}

Node *Node::SearchEnemyMove(int move) {
  this->CreateChildren();

  for(int i = 0; i < this->child_count; i++) {
    if(this->children[i]->enemy_board & (1 << move))
      return this->children[i];
  }
  
  assert(false);
  return nullptr;
}

void Node::CreateChildren() {
  if(this->GetWinner() || this->child_count > 0)
    return;

  for(int pos = 0; pos < 9; pos++) {
    bool p1 = this->ai_board & (1 << pos);
    bool p2 = this->enemy_board & (1 << pos);

    if (!p1 && !p2) {
      Node n(this->arena, !this->is_ai_turn);
      n.parent = this;
      n.ai_board = this->ai_board;
      n.enemy_board = this->enemy_board;

      if(this->is_ai_turn) 
        n.ai_board |= 1 << pos;
      else
        n.enemy_board |= 1 << pos;

      this->arena->push_back(n);

      assert(this->child_count < 9);
      this->children[this->child_count++] = &this->arena->back();
    }
  }
}

void Node::SimulateAndBackpropagate() {
  int result = this->GetWinner();

  uint16_t saved_ai_board = this->ai_board;
  uint16_t saved_enemy_board = this->enemy_board;
  bool saved_turn = this->is_ai_turn;

  std::random_device rd;
  std::mt19937 gen(rd());

  assert((saved_ai_board & saved_enemy_board) == 0);

  while (!result) {
    std::vector<int> available_moves;

    for (int pos = 0; pos < 9; pos++) {
      bool p1 = this->ai_board & (1 << pos);
      bool p2 = this->enemy_board & (1 << pos);
      if (!p1 && !p2)
        available_moves.push_back(pos);
    }

    if (available_moves.empty())
      break;

    std::uniform_int_distribution<> dis(0, available_moves.size() - 1);
    int move = available_moves[dis(gen)];

    if (this->is_ai_turn) {
      this->ai_board |= 1 << move;
    } else {
      this->enemy_board |= 1 << move;
    }

    this->is_ai_turn = !this->is_ai_turn;
    result = this->GetWinner();
  }

  this->ai_board = saved_ai_board;
  this->enemy_board = saved_enemy_board;
  this->is_ai_turn = saved_turn;

  auto curr_node = this;
  while (curr_node != nullptr) {
    curr_node->visit_count++;
    curr_node->eval += curr_node->is_ai_turn ? -result : result;
    curr_node = curr_node->parent;
  }
}

Node *Node::CalculateBestMove(size_t iter_count) {
  std::random_device rd;
  std::mt19937 gen(rd());

  for (size_t i = 0; i < iter_count; i++) {
    Node *leaf = this->FindBestLeafNode();
    leaf->CreateChildren();

    if (leaf->child_count > 0) {
      std::uniform_int_distribution<> dis(0, leaf->child_count - 1);
      leaf = leaf->children[dis(gen)];
    }
    leaf->SimulateAndBackpropagate();
  }

  return this->GetBestChild(0);
}

int Node::GetWinner() {
  for (const uint16_t mask :
      {0b000000111, 0b000111000, 0b111000000, 0b001001001, 0b010010010,
      0b100100100, 0b100010001, 0b001010100}) {
    if ((this->ai_board & mask) == mask) {
      return 1;
    } else if ((this->enemy_board & mask) == mask) {
      return -1;
    }
  }
  return 0;
}

double Node::GetUcbScore(double c) {
  const Node* p = this->parent != nullptr ? this->parent : this;

  if (this->visit_count == 0)
    return DBL_MAX;

  double exploitation = (double)this->eval / this->visit_count;
  double exploration = c * sqrt(log(p->visit_count) / this->visit_count);

  return exploration + exploitation;
}
