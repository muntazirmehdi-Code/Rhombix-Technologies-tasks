# Game of Life Simulation - C++

**Rhombix Technologies Internship | Month 2 | Task 2**

## Description
Conway's Game of Life implemented in C++. A cellular automaton simulation where cells on a grid live, die, or reproduce based on simple rules — producing complex and fascinating patterns.

## Features
- **5 Preset Patterns** — Glider, Blinker, Block, Toad, Random
- **Manual Cell Placement** — Place cells yourself on the grid
- **Custom Grid Size** — 5x5 up to 40x40
- **Generation Control** — Set max generations (1–500)
- **Live Stats** — Generation count & population displayed
- **Stable Detection** — Stops if grid stops changing
- **Extinction Detection** — Stops if all cells die
- **Toroidal Grid** — Edges wrap around

## Game Rules
1. **Underpopulation** — Live cell with < 2 neighbors dies
2. **Survival** — Live cell with 2 or 3 neighbors survives
3. **Overpopulation** — Live cell with > 3 neighbors dies
4. **Reproduction** — Dead cell with exactly 3 neighbors becomes alive

## How to Compile & Run

### Linux / Mac:
```bash
g++ -o game_of_life game_of_life.cpp
./game_of_life
```

### Windows (MinGW):
```bash
g++ -o game_of_life.exe game_of_life.cpp
game_of_life.exe
```

### Windows (Dev C++ / Code::Blocks):
Open `game_of_life.cpp` and press **F9** or **Ctrl+F5** to compile and run.

## How to Use
1. Run the program
2. Select **Start Simulation**
3. Enter grid size (rows and columns)
4. Enter max generations
5. Choose a starting pattern (or place cells manually)
6. Press **ENTER** to step through each generation
7. Watch the cells evolve!

## Patterns Included
| Pattern | Type | Description |
|---------|------|-------------|
| Glider | Spaceship | Moves diagonally across grid |
| Blinker | Oscillator | Alternates between 2 states |
| Block | Still Life | Never changes |
| Toad | Oscillator | Period-2 oscillator |
| Random | Random | Random initial state |

## Author
Developed as part of **Rhombix Technologies C++ Programming Internship**

---
*Repository: RhombixTechnologies_Tasks*
