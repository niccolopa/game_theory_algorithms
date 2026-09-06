#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>

// 1. Define memory compact payoff structure
// Each integer is 4 bytes, making this structure 8 bytes in size
//this compactness is crucial for keeping massive matrices in the CPU cache, which is essential for performance in large-scale computations.
struct Payoff {
    int player1; // Payoff for player 1 (row player, bar A)
    int player2; // Payoff for player 2 (column player, bar B)
};

int main() {
    std::ofstream log_file("phase_1.log", std::ios::app);
    if (!log_file) {
        std::cerr << "Could not open phase_1.log\n";
        return 1;
    }

    log_file << "Starting payoff matrix run\n";

    //2. define the strategies for both players
    std::vector<std::string> strategies= {"High", "Low"};

    //3. define the payoff matrix for the game
    //The weekly payoff matrix is a 2x2 matrix where each cell contains a Payoff structure representing the payoffs for both players.
    //Row player (Player 1) chooses between "High" and "Low" strategies, while the column player (Player 2) also chooses between "High" and "Low" strategies.
    std::vector<std::vector<Payoff>> game_matrix = {
        { {125, 125}, {80, 170} }, // Player 1 chooses "High", Player 2 chooses "High" or "Low"
        { {170, 80}, {93, 93} }  // Player 1 chooses "Low", Player 2 chooses "High" or "Low"
    };

    //4 render the payoff matrix in a readable format
    std::cout << "\n** two-player game matrix **\n\n";

    constexpr int label_width = 24;
    constexpr int cell_width = 20;

    // Player 1 chooses rows; Player 2 chooses columns.
    std::cout << std::setw(label_width) << ""
              << std::setw(cell_width * 2) << "PLAYER 2 (columns)" << "\n";
    std::cout << std::setw(label_width) << "PLAYER 1 (rows)"
              << std::setw(cell_width) << strategies[0]
              << std::setw(cell_width) << strategies[1] << "\n";

    // Print each row of the matrix with Player 1's strategies and the corresponding payoffs
    for (size_t i = 0; i < strategies.size(); ++i) {
        std::cout << std::setw(label_width) << strategies[i];
        for (size_t j = 0; j < strategies.size(); ++j) {
            std::string payoff = "(" + std::to_string(game_matrix[i][j].player1)
                               + ", " + std::to_string(game_matrix[i][j].player2) + ")";
            std::cout << std::setw(cell_width) << payoff;
            log_file << "Cell [" << i << "][" << j << "]: ("
                     << game_matrix[i][j].player1 << ", "
                     << game_matrix[i][j].player2 << ")\n";
        }
        std::cout << "\n";
    } 
    log_file << "Finished payoff matrix run\n\n";

    // 5. Setup a std::vector of std::pair<int, int> to store the (row, column) coordinates of any Nash Equilibria found, then start the std::chrono high-resolution timer.
    std::vector<std::pair<int, int>> nash_equilibria;
    auto start_time = std::chrono::high_resolution_clock::now();

    //6 Loop through every single cell [i][j] in the game_matrix to test if it is a Nash Equilibrium.
    // 6a. Assume the current cell [i][j] is the best response for both players (initialize two boolean flags to true).
    for (size_t i = 0; i < strategies.size(); ++i) {
        for (size_t j = 0; j < strategies.size(); ++j) {
            bool is_best_response_player1 = true;
            bool is_best_response_player2 = true;

            // 6b. Check if Player 1 has a better response in the same column (j).
            for (size_t k = 0; k < strategies.size(); ++k) {
                if (game_matrix[k][j].player1 > game_matrix[i][j].player1) {
                    is_best_response_player1 = false;
                    break;
                }
            }

            // 6c. Check if Player 2 has a better response in the same row (i).
            for (size_t l = 0; l < strategies.size(); ++l) {
                if (game_matrix[i][l].player2 > game_matrix[i][j].player2) {
                    is_best_response_player2 = false;
                    break;
                }
            }

            // 6d. If both players are best responding, record the cell as a Nash Equilibrium.
            if (is_best_response_player1 && is_best_response_player2) {
                nash_equilibria.emplace_back(i, j);
                log_file << "Nash Equilibrium found at cell [" << i << "][" << j << "]\n";
            }
        }
    }

    // 7 Stop the std::chrono timer and calculate the elapsed duration in microseconds.
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // 8 Check if the Nash Equilibria vector is empty. If it is, print a message saying no pure-strategy equilibrium exists. 
    // If it has data, loop through it and print the winning strategies and their payoffs to the console (and log file).
    if (nash_equilibria.empty()) {
        std::cout << "No pure-strategy Nash Equilibrium exists in this game.\n";
        log_file << "No pure-strategy Nash Equilibrium exists in this game.\n";
    } else {
        std::cout << "\n** Nash Equilibria found **\n\n";
        for (const auto& eq : nash_equilibria) {
            int row = eq.first;
            int col = eq.second;
            std::cout << "Nash Equilibrium at cell [" << row << "][" << col << "] with strategies: "
                      << strategies[row] << " (Player 1), " << strategies[col] << " (Player 2) "
                      << "and payoffs: (" << game_matrix[row][col].player1
                      << ", " << game_matrix[row][col].player2 << ")\n";
            log_file << "Nash Equilibrium at cell [" << row << "][" << col << "] with strategies: "
                     << strategies[row] << " (Player 1), " << strategies[col] << " (Player 2) "
                     << "and payoffs: (" << game_matrix[row][col].player1
                     << ", " << game_matrix[row][col].player2 << ")\n";
        }
    }

    //9 print the all execution time in microseconds to the console and log file.
    std::cout << "\nExecution time: " << duration << " microseconds\n";
    log_file << "Execution time: " << duration << " microseconds\n";
    log_file.close();

    return 0;
}
