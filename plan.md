**C++ Game Theory Engineering Roadmap**

**Level 1: Normal-Form Pure Strategy Engine**

* **Phase 1: The Matrix Architecture:** Define `Payoff` structs and 2D `std::vector` grids to represent simultaneous normal-form games. *(Completed)*


* **Phase 2: The Best Response Algorithm:** Implement a scanner that iterates through columns and rows to find each player's highest payoff, automatically isolating pure-strategy Nash Equilibria.


* **Phase 3: Iterated Elimination:** Write a function that algorithmically compares rows and columns to identify and delete strictly dominated strategies, reducing the matrix until only rationalizable outcomes remain.



**Level 2: Mixed Strategy & Expected Utility Solver**

* **Phase 1: Continuous Probability Variables:** Expand the engine to accept probability weights ($p$ and $1-p$) for each strategy.
* **Phase 2: The Indifference Calculator:** Program the algebraic logic to solve for the exact mixed-strategy probabilities that make the opposing player indifferent between their available choices.
* **Phase 3: Expected Payoff Generation:** Calculate and output the overall expected utility for both players under mixed conditions.

**Level 3: Extensive-Form Sequential Games (Memory & Pointers)**

* **Phase 1: Tree Data Structures:** Move away from 2D grids and build a Node-based Game Tree using C++ memory pointers to represent sequential moves.


* **Phase 2: Backward Induction Engine:** Implement a recursive algorithm that starts at the terminal leaves (final payoffs) and works backward up the tree to find the Subgame Perfect Nash Equilibrium.



**Level 4: Dynamic Ingestion & Scale (Data Science Integration)**

* **Phase 1: CSV Matrix Parser:** Use the `<fstream>` library to load massively scaled $N \times M$ payoff matrices dynamically from text files, rather than hardcoding 2x2 examples.
* **Phase 2: Evolutionary Strategy Simulator:** Build an iteration loop that pits different algorithmic behavioral strategies (e.g., Tit-for-Tat, Always Defect) against each other over 10,000 continuous games to test evolutionary stability.

