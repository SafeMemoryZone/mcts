#include <cstdint>
#include <iostream>
#include <cctype>

void print_board(uint16_t ai_board, uint16_t enemy_board) {
  std::cout << "  A B C\n";
  for (int row = 0; row < 3; ++row) {
    std::cout << row + 1 << " ";
    for (int col = 0; col < 3; ++col) {
      int position = row * 3 + col;
      char mark = '.';
      if (ai_board & (1 << position)) {
        mark = 'X';
      } else if (enemy_board & (1 << position)) {
        mark = 'O';
      }
      std::cout << mark << " ";
    }
    std::cout << "\n";
  }
}

int coordinate_to_idx(char col, char row) {
  return 3 * (row - '1') + (col - 'A');
}

bool is_valid_coordinate(char col, char row) {
  return (col >= 'A' && col <= 'C') && (row >= '1' && row <= '3');
}

int main(int argc, char **argv) {
  while (true) {
    // print_board(ai_board, enemy_board);

    std::cout << "Enter your move (e.x., A1, B2): ";
    std::string move;
    std::cin >> move;

    if (move.size() != 2) {
      std::cout << "Invalid input format. Try again.\n";
      continue;
    }

    char col = std::toupper(move[0]);
    char row = move[1];

    if (!std::isalpha(col) || !std::isdigit(row)) {
      std::cout << "Invalid input format. Try again.\n";
      continue;
    }

    if (!is_valid_coordinate(col, row)) {
      std::cout << "Invalid coordinates. Try again.\n";
      continue;
    }

    int idx = coordinate_to_idx(col, row);

    /* if (ai_board & (1 << idx) || enemy_board & (1 << idx)) { */
    /*   std::cout << "The position is already taken. Try again.\n"; */
    /*   continue; */
    /* } */
    /**/
    /* enemy_board |= (1 << idx); */
    // TODO: MCTS makes move
  }

  return 0;
}
