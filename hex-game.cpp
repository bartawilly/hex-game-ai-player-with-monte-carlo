/*
    Program: Implement Monte Carlo Hex move evaluation using a graph data structure
    Author: Ahmed Nasser
    Date: 30-Mar-2024
*/


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <algorithm>

using namespace std;


// representing the color of the cell
enum class Color { Black, White, Empty };

class Cell {
public:
   int index;
   Color state;
   vector<int> connections; // cell Connections
   Cell(int index, Color state = Color::Empty) : index(index), state(state) {}
};

// Class graph
class Board {
   public:

      int size;
      vector<Cell> cells; // vector of cells in the board

      Board(int size) : size(size){
         // reserve the size of the cells vector
         cells.reserve(size * size);

         for (int i = 0; i < size * size; ++i) {
            
            // construct the cells with empty color adding them to the cells vector
            cells.emplace_back(i, Color::Empty);
            
            int r = i / size;
            int c = i % size;

            // Right cell 
            if (c < size - 1) {
                  cells[i].connections.push_back(i + 1);
            }

            // Bottom cell
            if (r < size - 1) {
                  cells[i].connections.push_back(i + size);
            }

            // Bottom Left cell
            if (c > 0 && r < size - 1) {
                  cells[i].connections.push_back(i + (size - 1));
            }

            // Left cell
            if (c > 0) {
                  cells[i].connections.push_back(i - 1);
            }

            // Top cell
            if (r > 0) {
                  cells[i].connections.push_back(i - size);
            }

            // Top Right cell
            if (c < size - 1 && r > 0) {
                  cells[i].connections.push_back(i - (size - 1));
            }


         }
      }

      int monteCarlo(Color player, int iterations) {
         // make a vector to store the number of wins
         vector<int> wins(size * size, 0); 
         for (int i = 0; i < iterations; ++i) {
            // make a copy of the current board state
            Board simBoard = *this; 
            // make a vector to store the empty cells
            vector<int> emptyCells;
            for (int j = 0; j < size * size; ++j) {
               if (simBoard.cells[j].state == Color::Empty) {
                  emptyCells.push_back(j);
               }
            }
            // Randomly play moves for both players until the game ends
            Color currentPlayer = player;
            while (!emptyCells.empty()) {
               // Random move for the current player
               int moveIndex = emptyCells[rand() % emptyCells.size()]; 
               // Make the move
               simBoard.makeMove(moveIndex, currentPlayer);
               if (simBoard.checkWin(currentPlayer)) { 
                  // If the current player is the AI player increase the wins of the win index
                  if (currentPlayer == player) wins[moveIndex]++;
                  break;
               }
               // Remove the chosen move from emptyCells
               emptyCells.erase(remove(emptyCells.begin(), emptyCells.end(), moveIndex));
               // Switch player
               currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            }
         }
         // loop over the wins vector and get the max wins index and that's the best move for AI
         int bestMove = -1;
         int maxInWins = 0;
         for (int i = 0; i < size * size; ++i) {
               if (cells[i].state == Color::Empty && wins[i] > maxInWins) {
                  bestMove = i;
                  maxInWins = wins[i];
               }
         }
         return bestMove;
      }

      void makeMove(int index, Color player) {
         // check if the entered index is valid
         if (index >= 0 && index < size * size && cells[index].state == Color::Empty) {
               cells[index].state = player;
         } else {
               cout << "Invalid move. Try again." << endl;
         }
      }

      bool checkWin(Color player) {
         // size * size number of cells in the board
         vector<bool> visited(size * size, false);
         for (int i = 0; i < size; ++i) {
               // check if the cell is black and the player who play is the black player
               if (player == Color::Black && cells[i].state == Color::Black) {
                  // check the connected cells recursively for the black player
                  if (dfs(i, visited, player)) return true;
               }
               // do the same for the white player
               if (player == Color::White && cells[i * size].state == Color::White) {
                  if (dfs(i * size, visited, player)) return true;
               }
         }
         return false;
      }

      bool dfs(int index, vector<bool>& visited, Color player) {
         // check if the cell is not has the same color of the player return false
         if (cells[index].state != player) return false;
         // Mark the cell as visited
         visited[index] = true;
         // check if black player wins by reach the last row
         if (player == Color::Black && index / size == size - 1) return true;
         // check if white player wins by reach the last column
         if (player == Color::White && index % size == size - 1) return true;

         // check the connected cells
         for (int i : cells[index].connections) {
               // if the cell is not visited and has the same color of the player
               if (!visited[i] && cells[i].state == player) {
                  // check the connected cells recursively
                  if (dfs(i, visited, player)) return true;
               }
         }
         return false;
      }

   void print() {
      // print word Black in the first row

      cout <<string(size, ' ') <<"Black" << endl;
      for (int r = 0; r < size; ++r) {
            if( r == 0 ) {
               for (int i = 0; i < size; i++){
                  if (i == 0) cout<<"  "<<i;
                  else cout << "   " << i;
               }
               cout << endl;
            }
            cout << r << " ";
            cout << string(r * 2, ' ');
            for (int c = 0; c < size; c++) {
                int index = r * size + c;
                char cellChar = (cells[index].state == Color::Empty) ? 'E' :
                                (cells[index].state == Color::Black) ? 'B' : 'W';
                cout << cellChar;
                if (c < size - 1) cout << " - ";
            }
            // print word White at the end of the middle row
            if (r == size / 2) cout << "  White";
            cout << endl;
            if (r < size - 1) {
                cout << string(r * 2 + 3, ' ');
                for (int c = 0; c < size - 1; c++) {
                    cout << "\\ / ";
                }
                cout << "\\" << endl;
            }
        }
   }

      void printNodes() {
         for (int i = 0; i < size * size; ++i) {
               cout << i << ": ";
               for (int j : cells[i].connections) {
                  cout << j << " ";
               }
               cout << endl;
         }
      }
};

int main() {
   srand(time(NULL));

   int size;   
   cout<<"Enter the size of the board: ";
   
   cin >> size;
   if (cin.fail() || size <= 2) {
      cout << "Invalid input. Please enter a number greater than 2." << endl;
      return 1;
   }

   Board board(size);
   cout << "Welcome to the Hex Game!" << endl;
   cout << "You are the player playing with the black color." << endl;
   cout << "The AI is playing with the white color." << endl;

   int index;
   Color currentPlayer = Color::Black; // Start with Black player
   while (true) {
      board.print();
      if (currentPlayer == Color::White) {
            // AI's turn
            cout << "AI (White) is thinking..." << endl;
            index = board.monteCarlo(currentPlayer, 1000); 
            cout << "AI (White) chooses cell " << index << endl;
        } else {
            // Human player's turn
            cout << "Player with Black color, enter cell index" << " From 0 to " << board.size * board.size - 1 << ": ";
            cin >> index;
        }
      // Make a move
      board.makeMove(index, currentPlayer);
      if (board.checkWin(currentPlayer)) {
         board.print(); // Re-print the board to show the winning state
         cout << "Player with " << (currentPlayer == Color::Black ? "Black" : "White") << " wins!" << endl;
         break; // Exit the game loop
      }
      // Switch player
      currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
   }
   return 0;
}