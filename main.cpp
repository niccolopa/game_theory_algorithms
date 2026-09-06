#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <random>

//level 1, 1. Define memory compact payoff structure
// Each integer is 4 bytes, making this structure 8 bytes in size
//this compactness is crucial for keeping massive matrices in the CPU cache, which is essential for performance in large-scale computations.
struct Payoff {
    int player1; // Payoff for player 1 (row player, bar A)
    int player2; // Payoff for player 2 (column player, bar B)
};

//*LEVEL 2: iterated elimination function*
//1) declare a void function called eliminate_strictly_dominated_strategies
//  why: I need a reusable, isolated module so I can pass massive 1,000x1,000 random matrices to it during the Phase 2 stress-test.
//it must accept the following parameters: a reference to the game_matrix, a reference to the p1_strategies vector, a reference to the p2_strategies vector, and a reference to the log_file stream.
//I used the references because the function modify the original data directly in memory, rather than working on copies. This is crucial for performance and memory efficiency, especially with large matrices.
void eliminate_strictly_dominated_strategies(std::vector<std::vector<Payoff>>& game_matrix, std::vector<std::string>& p1_strategies, std::vector<std::string>& p2_strategies, std::ofstream& log_file) {
    bool eliminated_this_round = true;
    while (eliminated_this_round) {
        eliminated_this_round = false;

        // PLAYER 1 (ROW) CHECK
        for (size_t i = 0; i < game_matrix.size(); ++i) {
            for (size_t j = 0; j < game_matrix.size(); ++j) {
                if (i != j) { // Don't compare the same row
                    bool is_dominated = true;
                    for (size_t k = 0; k < game_matrix[i].size(); ++k) {
                        if (game_matrix[i][k].player1 >= game_matrix[j][k].player1) {
                            is_dominated = false;
                            break;
                        }
                    }
                    if (is_dominated) {
                        log_file << "Player 1 strategy " << p1_strategies[i] << " is strictly dominated by " << p1_strategies[j] << ". Eliminating " << p1_strategies[i] << ".\n";
                        game_matrix.erase(game_matrix.begin() + i);
                        p1_strategies.erase(p1_strategies.begin() + i);
                        eliminated_this_round = true;
                        break; // Break to resize the matrix safely
                    }
                }
            }
            if (eliminated_this_round) break; // Break outer loop to restart checks after resizing
        }

        // PLAYER 2 (COLUMN) CHECK
        for (size_t i = 0; i < game_matrix[0].size(); ++i) {
            for (size_t j = 0; j < game_matrix[0].size(); ++j) {
                if (i != j) { // Don't compare the same column
                    bool is_dominated = true;
                    for (size_t k = 0; k < game_matrix.size(); ++k) {
                        if (game_matrix[k][i].player2 >= game_matrix[k][j].player2) {
                            is_dominated = false;
                            break;
                        }
                    }
                    if (is_dominated) {
                        log_file << "Player 2 strategy " << p2_strategies[i] << " is strictly dominated by " << p2_strategies[j] << ". Eliminating " << p2_strategies[i] << ".\n";
                        for (auto& row : game_matrix) {
                            row.erase(row.begin() + i);
                        }
                        p2_strategies.erase(p2_strategies.begin() + i);
                        eliminated_this_round = true;
                        break; // Break to resize the matrix safely
                    }
                }
            }
            if (eliminated_this_round) break; // Break outer loop to restart checks after resizing
        }
    }
}

//2) open the function block {and CUT/PASTE the entire `while (eliminated_this_round)` from main() into this function. This isolates the elimination logic, making it reusable and easier to maintain. The function will now handle the elimination of strictly dominated strategies for both players, logging the process to the provided log file.}


int main() {
    std::ofstream log_file("phase_1.log", std::ios::app);
    if (!log_file) {
        std::cerr << "Could not open phase_1.log\n";
        return 1;
    }

    log_file << "Starting payoff matrix run\n";

    //level 1, 2. define the strategies for both players
    std::vector<std::string> strategies_p1= {"High", "Low"};
    std::vector<std::string> strategies_p2= {"High", "Low"};

    //level 1, 3. define the payoff matrix for the game
    //The weekly payoff matrix is a 2x2 matrix where each cell contains a Payoff structure representing the payoffs for both players.
    //Row player (Player 1) chooses between "High" and "Low" strategies, while the column player (Player 2) also chooses between "High" and "Low" strategies.
    std::vector<std::vector<Payoff>> game_matrix = {
        { {125, 125}, {80, 170} }, // Player 1 chooses "High", Player 2 chooses "High" or "Low"
        { {170, 80}, {93, 93} }  // Player 1 chooses "Low", Player 2 chooses "High" or "Low"
    };

    //LEVEL 2, 2) define a constant integer MATRIX_SIZE =1000;
    // why? Because I will use this constant to generate a 1000x1000 random payoff matrix for stress-testing the elimination function. This allows me to easily adjust the size of the test matrix in one place, making the code more maintainable and flexible for future tests.
    const int MATRIX_SIZE = 1000;

    //LEVEL 2, 3) initialize an empty vector for strategy_p1, strategy_p2 and game_matrix, then use a nested loop to fill them with random payoffs for a 1000x1000 matrix. This simulates a large game scenario for stress-testing the elimination function.
    std::vector<std::string> large_strategies_p1(MATRIX_SIZE);
    std::vector<std::string> large_strategies_p2(MATRIX_SIZE);
    std::vector<std::vector<Payoff>> large_game_matrix(MATRIX_SIZE, std::vector<Payoff>(MATRIX_SIZE));

    //Level 2, 4) set up a random number generator (std::random_device and std::mt19937) and a std::uniform_int_distribution between 1 and 1000;
    //to create unpredictable payoffs for the large game matrix. This randomness is crucial for stress-testing the elimination function under varied and realistic game scenarios.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    

    //level 2, 5) use the loop from 0 to matrix_size to populate p1 and p2 strategy vectors with unique names like "P1_Strategy_0", "P1_Strategy_1", ..., "P1_Strategy_999" and similarly for Player 2. This naming convention helps in easily identifying and logging the strategies during the elimination process.
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        large_strategies_p1[i] = "P1_Strategy_" + std::to_string(i);
        large_strategies_p2[i] = "P2_Strategy_" + std::to_string(i);
    }

    //level 2, 6) use a nested loop to fill the large_game_matrix with random payoffs for both players. Each cell [i][j] will have a Payoff structure with random integers for player1 and player2. This simulates a complex game scenario for stress-testing the elimination function.
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            large_game_matrix[i][j] = {dis(gen), dis(gen)};
        }
    }

    //level 2, 7) plant guaranteed-dominated rows so the stress test has real work to do.
    //why: with independent uniform payoffs, the odds a row loses in ALL 1000 columns are ~2^-1000, so a purely random matrix eliminates NOTHING and the benchmark proves nothing.
    //Row 0 is the benchmark. Each planted row is row 0 shifted strictly downward, so row i is dominated by row 0 (and by every planted row above it) in every single column.
    const int PLANTED_DOMINATED_ROWS = 50;
    for (int i = 1; i <= PLANTED_DOMINATED_ROWS; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            large_game_matrix[i][j].player1 = large_game_matrix[0][j].player1 - i;
        }
    }
 

    //level 1, 4 render the payoff matrix in a readable format
    

    constexpr int label_width = 24;
    constexpr int cell_width = 20;

    // Player 1 chooses rows; Player 2 chooses columns.
    std::cout << std::setw(label_width) << ""
              << std::setw(cell_width * 2) << "PLAYER 2 (columns)" << "\n";
    std::cout << std::setw(label_width) << "PLAYER 1 (rows)"
              << std::setw(cell_width) << strategies_p2[0]
              << std::setw(cell_width) << strategies_p2[1] << "\n";



    // Print each row of the matrix with Player 1's strategies and the corresponding payoffs
    for (size_t i = 0; i < strategies_p1.size(); ++i) {
        std::cout << std::setw(label_width) << strategies_p1[i];
        for (size_t j = 0; j < strategies_p2.size(); ++j) {
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

    //level 2, 3) Call `eliminate_strictly_dominated_strategies(game_matrix, strategies_p1, strategies_p2, log_file);` to perform the elimination of strictly dominated strategies for both players. This will modify the game_matrix and strategy vectors in place, and log the eliminations to the log file.


    // 4.5 Run the Iterated Elimination engine
    eliminate_strictly_dominated_strategies(game_matrix, strategies_p1, strategies_p2, log_file);

    // 4.5. Initialize a boolean flag `eliminated_this_round = true` to control a while loop.
    // The loop must continue running as long as a strategy was eliminated in the previous pass.
    /*bool eliminated_this_round = true;
    // 4.5a. Inside the while loop, immediately set `eliminated_this_round = false`.
    while (eliminated_this_round) {
        eliminated_this_round = false;
    
    // 4.5b. PLAYER 1 (ROW) CHECK:
    // Loop through all pairs of active rows (Row A and Row B).
    // Check if Row A is STRICTLY DOMINATED by Row B. 
    // This requires a nested loop across all columns: if Row A's payoff is strictly less (<) than Row B's payoff for EVERY column, it is dominated.
    // IF DOMINATED: Erase Row A from `game_matrix` and erase its name from `p1_strategies`.
    // Set `eliminated_this_round = true`, log the elimination to your file, and `break` out of the row check so the matrix can resize safely.
    for (size_t i = 0; i < game_matrix.size(); ++i) {
        for (size_t j = 0; j < game_matrix.size(); ++j) {
            if (i != j) { // Don't compare the same row
                bool is_dominated = true;
                for (size_t k = 0; k < game_matrix[i].size(); ++k) {
                    if (game_matrix[i][k].player1 >= game_matrix[j][k].player1) {
                        is_dominated = false;
                        break;
                    }
                }
                if (is_dominated) {
                    log_file << "Player 1 strategy " << strategies_p1[i] << " is strictly dominated by " << strategies_p1[j] << ". Eliminating " << strategies_p1[i] << ".\n";
                    game_matrix.erase(game_matrix.begin() + i);
                    strategies_p1.erase(strategies_p1.begin() + i);
                    eliminated_this_round = true;
                    break; // Break to resize the matrix safely
                }
            }
        }
        if (eliminated_this_round) break; // Break outer loop to restart checks after resizing
    }

    // 4.5c. PLAYER 2 (COLUMN) CHECK:
    // Loop through all pairs of active columns (Column A and Column B).
    // Check if Column A is STRICTLY DOMINATED by Column B.
    // This requires a nested loop across all rows: if Column A's payoff is strictly less (<) than Column B's payoff for EVERY row, it is dominated.
    // IF DOMINATED: Erase Column A from `game_matrix` and erase its name from `p2_strategies`.
    // Set `eliminated_this_round = true`, log the elimination to your file, and `break` out of the column check so the matrix can resize safely.
    for (size_t i = 0; i < game_matrix[0].size(); ++i) {
        for (size_t j = 0; j < game_matrix[0].size(); ++j) {
            if (i != j) { // Don't compare the same column
                bool is_dominated = true;
                for (size_t k = 0; k < game_matrix.size(); ++k) {
                    if (game_matrix[k][i].player2 >= game_matrix[k][j].player2) {
                        is_dominated = false;
                        break;
                    }
                }
                if (is_dominated) {
                    log_file << "Player 2 strategy " << strategies_p2[i] << " is strictly dominated by " << strategies_p2[j] << ". Eliminating " << strategies_p2[i] << ".\n";
                    for (auto& row : game_matrix) {
                        row.erase(row.begin() + i);
                    }
                    strategies_p2.erase(strategies_p2.begin() + i);
                    eliminated_this_round = true;
                    break; // Break to resize the matrix safely
                }
            }
        if (eliminated_this_round) break; // Break outer loop to restart checks after resizing
        
    }
    */

    //4.6 After the while loop ends, print the reduced game matrix to the console and log file in the same format as before. This will show the remaining strategies after eliminating strictly dominated strategies.
    //write a quick loop to print the reduced game matrix to the console and log file in the same format as before. This will show the remaining strategies after eliminating strictly dominated strategies.
    std::cout << "\n** reduced two-player game matrix after eliminating strictly dominated strategies **\n";
    std::cout << std::setw(label_width) << ""
              << std::setw(cell_width * strategies_p2.size()) << "PLAYER 2 (columns)" << "\n";
    std::cout << std::setw(label_width) << "PLAYER 1 (rows)";
    for (const auto& strategy : strategies_p2) {
        std::cout << std::setw(cell_width) << strategy;
    }
    std::cout << "\n";
    for (size_t i = 0; i < game_matrix.size(); ++i) {
        std::cout << std::setw(label_width) << strategies_p1[i];
        for (size_t j = 0; j < game_matrix[i].size(); ++j) {
            std::string payoff = "(" + std::to_string(game_matrix[i][j].player1)
                               + ", " + std::to_string(game_matrix[i][j].player2) + ")";
            std::cout << std::setw(cell_width) << payoff;
            log_file << "Cell [" << i << "][" << j << "]: ("
                     << game_matrix[i][j].player1 << ", "
                     << game_matrix[i][j].player2 << ")\n";
        }
        std::cout << "\n";
    }



    // 5. Setup a std::vector of std::pair<int, int> to store the (row, column) coordinates of any Nash Equilibria found, then start the std::chrono high-resolution timer.
    std::vector<std::pair<int, int>> nash_equilibria;
    auto start_time = std::chrono::high_resolution_clock::now();

    //6 Loop through every single cell [i][j] in the game_matrix to test if it is a Nash Equilibrium.
    // 6a. Assume the current cell [i][j] is the best response for both players (initialize two boolean flags to true).
    for (size_t i = 0; i < strategies_p1.size(); ++i) {
        for (size_t j = 0; j < strategies_p2.size(); ++j) {
            bool is_best_response_player1 = true;
            bool is_best_response_player2 = true;

            // 6b. Check if Player 1 has a better response in the same column (j).
            for (size_t k = 0; k < strategies_p1.size(); ++k) {
                if (game_matrix[k][j].player1 > game_matrix[i][j].player1) {
                    is_best_response_player1 = false;
                    break;
                }
            }

            // 6c. Check if Player 2 has a better response in the same row (i).
            for (size_t l = 0; l < strategies_p2.size(); ++l) {
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

        // 7 Stop the std::chrono timer and calculate the elapsed duration in nanoseconds.
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time).count();

    // 8 Check if the Nash Equilibria vector is empty. If it is, print a message saying no
    // pure-strategy equilibrium exists. If it has data, loop through it and print the winning
    // strategies and their payoffs to the console (and log file).
    if (nash_equilibria.empty()) {
        std::cout << "No pure-strategy Nash Equilibrium exists in this game.\n";
        log_file << "No pure-strategy Nash Equilibrium exists in this game.\n";
    } else {
        std::cout << "\n** Nash Equilibria found **\n\n";
        for (const auto& eq : nash_equilibria) {
            int row = eq.first;
            int col = eq.second;
            std::cout << "Nash Equilibrium at cell [" << row << "][" << col << "] with strategies: "
                      << strategies_p1[row] << " (Player 1), " << strategies_p2[col] << " (Player 2) "
                      << "and payoffs: (" << game_matrix[row][col].player1
                      << ", " << game_matrix[row][col].player2 << ")\n";
            log_file << "Nash Equilibrium at cell [" << row << "][" << col << "] with strategies: "
                     << strategies_p1[row] << " (Player 1), " << strategies_p2[col] << " (Player 2) "
                     << "and payoffs: (" << game_matrix[row][col].player1
                     << ", " << game_matrix[row][col].player2 << ")\n";
        }
    }

    //9 print the all execution time in nanoseconds to the console and log file.
    std::cout << "\nExecution time: " << duration << " nanoseconds\n";
    log_file << "Execution time: " << duration << " nanoseconds\n";

    //LEVEL 2, PHASE 2: 1000x1000 stress test
    //The chrono timer above measures the Nash scan on the 2x2 game. This one measures what
    //Phase 2 is actually about: iterated elimination on a million-cell matrix.
    std::cout << "\n** stress test: " << MATRIX_SIZE << "x" << MATRIX_SIZE << " matrix **\n";
    log_file << "\nStarting " << MATRIX_SIZE << "x" << MATRIX_SIZE << " stress test\n";

    auto stress_start = std::chrono::high_resolution_clock::now();
    eliminate_strictly_dominated_strategies(large_game_matrix, large_strategies_p1,
                                            large_strategies_p2, log_file);
    auto stress_end = std::chrono::high_resolution_clock::now();

    auto stress_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        stress_end - stress_start).count();

    std::cout << "Rows survived:    " << large_game_matrix.size()    << " / " << MATRIX_SIZE << "\n";
    std::cout << "Columns survived: " << large_game_matrix[0].size() << " / " << MATRIX_SIZE << "\n";
    std::cout << "Elimination time: " << stress_ns << " nanoseconds\n";
    log_file << "Stress test: " << large_game_matrix.size() << " rows / "
             << large_game_matrix[0].size() << " columns survived in " << stress_ns << " nanoseconds\n";

    log_file.close();
    return 0;
}
