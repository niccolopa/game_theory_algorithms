# Game Theory Algorithms in C++

This is a personal project at the intersection of three things I am learning:

- C++ programming, from the fundamentals upward
- Microeconomics, after completing the first year of a BSc in Economics and Finance with Data Science
- Game theory, especially Nash equilibrium and the logic of strategic decision-making

I became fascinated by the idea that a simple payoff table can describe a surprisingly rich situation: each player's best choice depends on what the other player might do. This project is my way of exploring that idea by building the tools myself, starting with a small matrix and gradually moving toward larger, more demanding problems.

The goal is not to produce a polished library immediately. The goal is to understand what is happening at every level: how the data is represented, how the algorithms work, how memory is used, and where performance begins to matter.

## The Project Roadmap

The project is divided into four levels. Each level introduces a new game-theory problem and a new C++ concept.

### Level 1: Core Matrices and Benchmarking

The starting point is a normal-form game represented as a payoff matrix. Each cell stores the payoff for both players.

This level focuses on:

- Designing a compact `Payoff` structure
- Storing a two-dimensional game matrix with `std::vector`
- Printing the matrix clearly, with row and column ownership labelled
- Finding pure-strategy Nash equilibria by scanning possible outcomes
- Recording results in a log file
- Measuring execution time with `<chrono>`

The first example is inspired by a two-bar pricing game. It is small enough to understand by hand, but concrete enough to make the algorithms meaningful.

#### First Performance Result

The first timing experiment measured the pure-strategy Nash-equilibrium scan after the matrix had been reduced by eliminating strictly dominated strategies. On the current machine, one run took:

```text
Execution time: 100 nanoseconds
```

This result is useful as an initial baseline, but it should be interpreted carefully. The example is a very small game, and the measurement covers only the equilibrium-search section, not matrix construction, printing, logging, or strategy elimination. At this scale, operating-system scheduling, compiler optimisations, and the clock's resolution can have a noticeable effect on the result.

The next benchmarking step will repeat the same calculation many times and measure the total duration. That should produce a more stable average and make comparisons meaningful when the game matrix becomes larger. The aim is not to claim that a tiny example proves high performance; it is to create a measured reference point for the later levels.

### Level 2: Efficiency and Dominance

The next step is to reduce the search space using iterated elimination of strictly dominated strategies. A strategy is strictly dominated when another strategy gives a player a higher payoff in every relevant situation.

This level will explore:

- Comparing rows and columns of a payoff matrix
- Removing dominated strategies safely while containers are changing
- Keeping strategy labels synchronized with the matrix
- Generating large random matrices for stress-testing
- Comparing a direct exhaustive approach with a reduced search space

The aim is to connect an economic idea, rationalizability, with practical algorithmic efficiency.

### Level 3: Extensive-Form Games and Memory

Normal-form matrices describe simultaneous choices. Level 3 moves to sequential games, where the order of decisions matters.

Planned topics include:

- Representing game trees with a custom `Node` type
- Using pointers to connect decisions and outcomes
- Traversing a tree recursively
- Applying backward induction
- Calculating a subgame-perfect Nash equilibrium
- Understanding ownership and lifetime when using dynamic memory

This level will be a chance to study not only game theory, but also one of the areas where C++ becomes especially distinctive: explicit control over memory and object relationships.

### Level 4: Scale and Parallelisation

The final level focuses on larger data and concurrent computation.

Planned topics include:

- Reading simulated tournament data from CSV files
- Parsing and storing larger datasets
- Solving many independent games
- Dividing work across threads
- Coordinating shared output with `std::mutex`
- Measuring whether parallelism actually improves performance

The important question will not simply be whether the program can use more CPU cores. It will be whether the cost of coordination is justified by the work being performed.

## Current Status

The project is currently at Level 1 and beginning the transition toward Level 2. The program can represent and display a two-player payoff matrix, log its output, search for pure-strategy equilibria, and measure the solver's runtime.

The implementation is intentionally being developed in small steps. Some experiments may be incomplete or revised as my understanding improves. That is part of the project: the code is also a record of the learning process.

## Building and Running

With a C++ compiler such as `g++` installed, run:

```powershell
g++ main.cpp -o main.exe
.\main.exe
```

To save the program's console output to a log file while still displaying it, use PowerShell's `Tee-Object`:

```powershell
g++ main.cpp -o main.exe; if ($LASTEXITCODE -eq 0) { .\main.exe 2>&1 | Tee-Object -FilePath output.log }
```

The program also writes diagnostic information to `phase_1.log`. Log and executable files are ignored by Git in this project.

## Why I Am Building It This Way

I want to learn C++ by making decisions that have a reason behind them, rather than by collecting disconnected examples. Game theory gives the project a subject I genuinely care about, while the increasing scale gives each new language feature a practical purpose.

The project is deliberately being built from scratch. I am making only modest use of AI: it has helped me organise the project ideas and write this `README.md`. The C++ implementation, experiments, debugging, and learning process remain my own work. The important part is being able to explain the algorithm after it runs, not merely getting a result on the screen.

## Learning Goals

By the end of the project, I hope to be able to:

- Explain how pure-strategy Nash equilibria are found algorithmically
- Translate economic definitions into reliable C++ code
- Choose suitable data structures for different game representations
- Understand the practical effects of memory layout and container operations
- Benchmark algorithms without drawing conclusions from misleading measurements
- Use recursion, pointers, file handling, and threads with greater confidence
- Recognise the difference between a program that works for one example and an algorithm that works robustly in general

This repository is a work in progress, and that is intentional. The game may be theoretical, but the learning is very practical.
