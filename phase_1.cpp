#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

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

    //4. render the payoff matrix in a readable format
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
    return 0;
}
