#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <iomanip>

using namespace std;

// ===================== CONSTANTS =====================
const int MIN_SIZE = 5;
const int MAX_SIZE = 40;
const char ALIVE = '#';
const char DEAD  = '.';

// ===================== GRID FUNCTIONS =====================

typedef vector<vector<int>> Grid;

Grid createGrid(int rows, int cols, int val = 0) {
    return Grid(rows, vector<int>(cols, val));
}

void printGrid(const Grid& grid, int generation, int population) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Top border
    cout << "+";
    for (int j = 0; j < cols; j++) cout << "-";
    cout << "+" << endl;

    for (int i = 0; i < rows; i++) {
        cout << "|";
        for (int j = 0; j < cols; j++) {
            cout << (grid[i][j] ? ALIVE : DEAD);
        }
        cout << "|";

        // Print stats on the right
        if (i == 1) cout << "  Generation : " << generation;
        if (i == 2) cout << "  Population : " << population;
        if (i == 3) cout << "  Alive (" << ALIVE << ") Dead (" << DEAD << ")";
        cout << endl;
    }

    // Bottom border
    cout << "+";
    for (int j = 0; j < cols; j++) cout << "-";
    cout << "+" << endl;
}

int countNeighbors(const Grid& grid, int r, int c) {
    int rows = grid.size();
    int cols = grid[0].size();
    int count = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            // Wrap around edges (toroidal)
            nr = (nr + rows) % rows;
            nc = (nc + cols) % cols;
            count += grid[nr][nc];
        }
    }
    return count;
}

Grid nextGeneration(const Grid& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    Grid next = createGrid(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int neighbors = countNeighbors(grid, i, j);
            if (grid[i][j]) {
                // Live cell survives with 2 or 3 neighbors
                next[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                // Dead cell becomes alive with exactly 3 neighbors
                next[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
    return next;
}

int countPopulation(const Grid& grid) {
    int count = 0;
    for (auto& row : grid)
        for (int cell : row)
            count += cell;
    return count;
}

bool gridsEqual(const Grid& a, const Grid& b) {
    return a == b;
}

// ===================== UTILITY =====================

void printLine(char ch = '-', int len = 55) {
    cout << string(len, ch) << endl;
}

void printHeader(const string& title) {
    printLine('=');
    int padding = (55 - (int)title.length()) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << title << endl;
    printLine('=');
}

int getValidInt(int minVal, int maxVal) {
    int val;
    while (true) {
        cin >> val;
        if (cin.fail() || val < minVal || val > maxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid! Enter between " << minVal << " and " << maxVal << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

void pauseScreen() {
    cout << "\n  Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ===================== PRESET PATTERNS =====================

void applyPreset(Grid& grid, int choice) {
    int rows = grid.size();
    int cols = grid[0].size();
    int cr = rows / 2;
    int cc = cols / 2;

    // Clear grid first
    for (auto& row : grid)
        fill(row.begin(), row.end(), 0);

    if (choice == 1) {
        // Glider
        if (cr-1 >= 0 && cc+1 < cols) grid[cr-1][cc+1] = 1;
        if (cr   >= 0 && cc-1 >= 0)   grid[cr][cc-1]   = 1;
        if (cr   >= 0 && cc+1 < cols) grid[cr][cc+1]   = 1;
        if (cr+1 >= 0 && cc   < cols) grid[cr+1][cc]   = 1;
        if (cr+1 >= 0 && cc+1 < cols) grid[cr+1][cc+1] = 1;
        cout << "  Glider pattern applied!\n";
    }
    else if (choice == 2) {
        // Blinker (period 2 oscillator)
        if (cc-1 >= 0 && cc+1 < cols) {
            grid[cr][cc-1] = 1;
            grid[cr][cc]   = 1;
            grid[cr][cc+1] = 1;
        }
        cout << "  Blinker pattern applied!\n";
    }
    else if (choice == 3) {
        // Block (still life)
        if (cr+1 < rows && cc+1 < cols) {
            grid[cr][cc]     = 1;
            grid[cr][cc+1]   = 1;
            grid[cr+1][cc]   = 1;
            grid[cr+1][cc+1] = 1;
        }
        cout << "  Block pattern applied!\n";
    }
    else if (choice == 4) {
        // Toad (period 2)
        if (cr+1 < rows && cc+2 < cols && cc-1 >= 0) {
            grid[cr][cc]     = 1;
            grid[cr][cc+1]   = 1;
            grid[cr][cc+2]   = 1;
            grid[cr+1][cc-1] = 1;
            grid[cr+1][cc]   = 1;
            grid[cr+1][cc+1] = 1;
        }
        cout << "  Toad pattern applied!\n";
    }
    else if (choice == 5) {
        // Random
        srand((unsigned)time(0));
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                grid[i][j] = rand() % 2;
        cout << "  Random pattern applied!\n";
    }
}

// ===================== MANUAL SETUP =====================

void manualSetup(Grid& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Clear
    for (auto& row : grid) fill(row.begin(), row.end(), 0);

    printHeader("MANUAL CELL PLACEMENT");
    cout << "\n  Grid size: " << rows << " x " << cols << "\n";
    cout << "  Enter row and column to toggle a cell alive.\n";
    cout << "  Rows: 1 to " << rows << ", Cols: 1 to " << cols << "\n";
    cout << "  Enter 0 0 when done.\n\n";

    // Show empty grid first
    printGrid(grid, 0, 0);

    while (true) {
        cout << "\n  Enter row col (0 0 to finish): ";
        int r, c;
        cin >> r >> c;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (r == 0 && c == 0) break;
        if (r < 1 || r > rows || c < 1 || c > cols) {
            cout << "  Out of bounds! Try again.\n";
            continue;
        }
        grid[r-1][c-1] ^= 1; // toggle
        cout << "  Cell (" << r << "," << c << ") is now "
             << (grid[r-1][c-1] ? "ALIVE" : "DEAD") << "\n";
        printGrid(grid, 0, countPopulation(grid));
    }
}

// ===================== RUN SIMULATION =====================

void runSimulation(Grid grid, int maxGenerations, int delayMs) {
    printHeader("SIMULATION RUNNING");

    int generation = 0;
    int population = countPopulation(grid);

    cout << "\n  Initial State:\n\n";
    printGrid(grid, generation, population);
    pauseScreen();

    Grid prev = grid;
    bool stable = false;

    while (generation < maxGenerations) {
        Grid next = nextGeneration(grid);
        generation++;
        population = countPopulation(next);

        // Clear screen effect
        for (int i = 0; i < 3; i++) cout << "\n";

        printLine('=');
        cout << "  GENERATION: " << generation
             << "  |  POPULATION: " << population << "\n";
        printLine('=');
        cout << "\n";
        printGrid(next, generation, population);

        // Check if stable (no change)
        if (gridsEqual(next, grid)) {
            cout << "\n  *** Grid is STABLE - No more changes! ***\n";
            stable = true;
            grid = next;
            break;
        }

        // Check extinction
        if (population == 0) {
            cout << "\n  *** All cells are DEAD - Extinction! ***\n";
            grid = next;
            break;
        }

        grid = next;

        // Simple delay
        cout << "\n  [Generation " << generation << "/" << maxGenerations
             << "]  Press ENTER for next gen (or wait)...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Final summary
    printLine('=');
    cout << "  SIMULATION COMPLETE\n";
    printLine('=');
    cout << "\n  Total Generations  : " << generation << "\n";
    cout << "  Final Population   : " << population << "\n";
    cout << "  Status             : "
         << (stable ? "Stable" : (population == 0 ? "Extinct" : "Max generations reached"))
         << "\n\n";
}

// ===================== MAIN MENU =====================

void showMainMenu() {
    printHeader("GAME OF LIFE SIMULATION");
    cout << "\n";
    cout << "  1. Start Simulation\n";
    cout << "  2. About Conway's Game of Life\n";
    cout << "  3. Rules\n";
    cout << "  4. Exit\n";
    cout << "\n  Choose an option: ";
}

void showAbout() {
    printHeader("ABOUT");
    cout << "\n";
    cout << "  Conway's Game of Life\n";
    cout << "  Created by mathematician John Conway in 1970.\n\n";
    cout << "  It is a cellular automaton simulation where cells\n";
    cout << "  on a grid live, die, or reproduce based on simple rules.\n\n";
    cout << "  Despite simple rules, it produces incredibly\n";
    cout << "  complex and beautiful patterns.\n\n";
    cout << "  Developed for: Rhombix Technologies Internship\n";
    cout << "  Domain       : C++ Programming - Month 2 - Task 2\n\n";
}

void showRules() {
    printHeader("GAME RULES");
    cout << "\n";
    cout << "  Each cell can be ALIVE (#) or DEAD (.)\n\n";
    cout << "  Rules applied each generation:\n\n";
    cout << "  1. UNDERPOPULATION:\n";
    cout << "     A live cell with < 2 live neighbors DIES.\n\n";
    cout << "  2. SURVIVAL:\n";
    cout << "     A live cell with 2 or 3 neighbors SURVIVES.\n\n";
    cout << "  3. OVERPOPULATION:\n";
    cout << "     A live cell with > 3 live neighbors DIES.\n\n";
    cout << "  4. REPRODUCTION:\n";
    cout << "     A dead cell with exactly 3 neighbors BECOMES ALIVE.\n\n";
    cout << "  * Grid wraps around edges (toroidal topology)\n\n";
}

void setupAndRun() {
    printHeader("SIMULATION SETUP");

    // Grid size
    cout << "\n  Enter grid ROWS (" << MIN_SIZE << "-" << MAX_SIZE << "): ";
    int rows = getValidInt(MIN_SIZE, MAX_SIZE);
    cout << "  Enter grid COLS (" << MIN_SIZE << "-" << MAX_SIZE << "): ";
    int cols = getValidInt(MIN_SIZE, MAX_SIZE);

    // Max generations
    cout << "  Enter max generations (1-500): ";
    int maxGen = getValidInt(1, 500);

    Grid grid = createGrid(rows, cols);

    // Initial state
    printHeader("SET INITIAL STATE");
    cout << "\n";
    cout << "  1. Glider\n";
    cout << "  2. Blinker (oscillator)\n";
    cout << "  3. Block (still life)\n";
    cout << "  4. Toad (oscillator)\n";
    cout << "  5. Random\n";
    cout << "  6. Manual (place cells yourself)\n";
    cout << "\n  Choose pattern: ";
    int patChoice = getValidInt(1, 6);

    if (patChoice == 6) {
        manualSetup(grid);
    } else {
        applyPreset(grid, patChoice);
    }

    if (countPopulation(grid) == 0) {
        cout << "\n  No alive cells! Please add some cells first.\n";
        return;
    }

    cout << "\n  Starting simulation...\n\n";
    runSimulation(grid, maxGen, 0);
}

// ===================== MAIN =====================

int main() {
    cout << "\n";
    printLine('*');
    cout << "  Conway's Game of Life - C++ Simulation\n";
    cout << "  Rhombix Technologies Internship\n";
    printLine('*');
    cout << "\n";

    int choice;
    while (true) {
        showMainMenu();
        choice = getValidInt(1, 4);

        switch (choice) {
            case 1:
                setupAndRun();
                break;
            case 2:
                showAbout();
                break;
            case 3:
                showRules();
                break;
            case 4:
                printLine('=');
                cout << "  Thank you! Goodbye!\n";
                printLine('=');
                return 0;
        }
    }

    return 0;
}
