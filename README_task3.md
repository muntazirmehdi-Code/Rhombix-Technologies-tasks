# Month 3 Tasks - C++ Programming

**Rhombix Technologies Internship | Month 3**

---

## Task 1 - Number Guessing Game

### Description
Computer randomly picks a secret number. Player guesses it with limited attempts. Hints like "Too High", "Too Low", and warmth indicators help guide the player.

### Features
- 4 Difficulty levels: Easy, Medium, Hard, Expert
- "Too High / Too Low" hints after every guess
- Warmth hints: Very Hot, Hot, Warm, Cold, Very Cold
- Guess history shown after each attempt
- Star rating based on performance
- Leaderboard with best scores
- Multiple players supported

### How to Compile & Run
```bash
g++ -o number_game number_guessing_game.cpp
./number_game
```
Windows:
```bash
g++ -o number_game.exe number_guessing_game.cpp
number_game.exe
```

---

## Task 2 - Sudoku Solver

### Description
Solves Sudoku puzzles of varying difficulty using a Backtracking algorithm. Supports preset puzzles and manual input.

### Features
- 3 Preset puzzles: Easy, Medium, Hard
- Manual puzzle input (enter your own)
- Backtracking algorithm to solve any valid puzzle
- Detects invalid/unsolvable puzzles
- Color-coded output (solved cells shown in green)
- Input validation

### How to Compile & Run
```bash
g++ -o sudoku sudoku_solver.cpp
./sudoku
```
Windows:
```bash
g++ -o sudoku.exe sudoku_solver.cpp
sudoku.exe
```

### Algorithm
Uses **Backtracking**:
1. Find an empty cell
2. Try digits 1-9
3. If valid → place it and recurse
4. If no digit works → backtrack and try next

---

## Author
Developed as part of **Rhombix Technologies C++ Programming Internship**

*Repository: RhombixTechnologies_Tasks*
