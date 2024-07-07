#include "node.hpp"
#include <cassert>
#include <cfloat>
#include <iostream>
#include <string>
#include <cctype>
#include <deque>

void print_board(uint16_t ai_board, uint16_t enemy_board, char ai, char player) {
  char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

  for (int i = 0; i < 9; i++) {
    if (ai_board & (1 << i))
      board[i] = ai;
    else if (enemy_board & (1 << i))
      board[i] = player;
  }

  std::cout << " A   B   C\n\n";
  std::cout << " " << board[0] << " | " << board[1] << " | " << board[2] << "   1\n";
  std::cout << "---|---|---\n";
  std::cout << " " << board[3] << " | " << board[4] << " | " << board[5] << "   2\n";
  std::cout << "---|---|---\n";
  std::cout << " " << board[6] << " | " << board[7] << " | " << board[8] << "   3\n";
}

bool is_valid_coordinate(char col, char row) {
  return (col >= 'A' && col <= 'C') && (row >= '1' && row <= '3');
}

int coordinate_to_idx(char col, char row) {
  return (col - 'A') + 3 * (row - '1');
}

int main(int argc, char **argv) {
  char player;

  for(;;) {
    std::string selection;
    std::cout << "Select your symbol (X/O): ";
    std::cin >> selection;

    if(selection.length() != 1) {
      std::cout << "Invalid input format. Try again.\n";
      continue;
    }
    player = std::toupper(selection[0]);

    if(player != 'X' && player != 'O') {
      std::cout << "Invalid selection. Try again.\n";
      continue;
    }

    break;
  }

  char ai = player == 'O' ? 'X' : 'O';
  bool is_ai_turn = ai == 'X';

  std::deque<Node> arena;
  Node root(&arena, is_ai_turn);
  Node *curr_node = &root;

  if(!is_ai_turn)
    print_board(0, 0, ai, player);

  while (true) {
    if (curr_node->GetWinner()) {
      std::cout << "Game Over! ";
      if (curr_node->GetWinner() == 1)
        std::cout << "AI wins!\n";
      else
        std::cout << "You win!\n";
      break;
    }
    else if((curr_node->ai_board | curr_node->enemy_board) == 0b111111111) {
      std::cout << "Game Over! Draw!\n";
      break;
    }

    if (curr_node->is_ai_turn) {
      curr_node = curr_node->CalculateBestMove(1000);
      std::cout << "AI's move:\n";
      print_board(curr_node->ai_board, curr_node->enemy_board, ai, player);
    } 
    else {
      std::cout << "Enter your move (e.g., A1, B2): ";
      std::string move;
      std::cin >> move;

      if (move.size() != 2) {
        std::cout << "Invalid input format. Try again.\n";
        continue;
      }

      char col = std::toupper(move[0]);
      char row = move[1];

      if (!std::isalpha(col) || !std::isdigit(row) || !is_valid_coordinate(col, row)) {
        std::cout << "Invalid input format. Try again.\n";
        continue;
      }

      int idx = coordinate_to_idx(col, row);

      if (curr_node->ai_board & (1 << idx) || curr_node->enemy_board & (1 << idx)) {
        std::cout << "The position is already taken. Try again.\n";
        continue;
      }

      curr_node = curr_node->SearchEnemyMove(idx);

      assert(curr_node != nullptr);
      std::cout << "Your move:\n";
      print_board(curr_node->ai_board, curr_node->enemy_board, ai, player);
    }
  }

  return 0;
}
