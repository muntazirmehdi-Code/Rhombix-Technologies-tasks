#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>

using namespace std;

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

// ===================== SUDOKU BOARD =====================

typedef vector<vector<int>> Board;

void printBoard(const Board& board, const Board& original) {
    cout << "\n  +-------+-------+-------+\n";
    for (int i = 0; i < 9; i++) {
        cout << "  |";
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 && j != 0) cout << " |";
            if (board[i][j] == 0)
                cout << " .";
            else if (original[i][j] == 0)
                cout << " \033[32m" << board[i][j] << "\033[0m"; // green for solved
            else
                cout << " " << board[i][j];
        }
        cout << " |\n";
        if (i % 3 == 2) cout << "  +-------+-------+-------+\n";
    }
    cout << "\n  (Green numbers = solved by computer)\n\n";
}

// ===================== SOLVER =====================

bool isValid(const Board& board, int row, int col, int num) {
    // Check row
    for (int j = 0; j < 9; j++)
        if (board[row][j] == num) return false;

    // Check column
    for (int i = 0; i < 9; i++)
        if (board[i][col] == num) return false;

    // Check 3x3 box
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[boxRow+i][boxCol+j] == num) return false;

    return true;
}

bool solveSudoku(Board& board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValid(board, i, j, num)) {
                        board[i][j] = num;
                        if (solveSudoku(board)) return true;
                        board[i][j] = 0; // backtrack
                    }
                }
                return false; // no valid number found
            }
        }
    }
    return true; // all cells filled
}

int countEmpty(const Board& board) {
    int count = 0;
    for (auto& row : board)
        for (int cell : row)
            if (cell == 0) count++;
    return count;
}

bool isValidBoard(const Board& board) {
    // Check rows
    for (int i = 0; i < 9; i++) {
        vector<bool> seen(10, false);
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) {
                if (seen[board[i][j]]) return false;
                seen[board[i][j]] = true;
            }
        }
    }
    // Check cols
    for (int j = 0; j < 9; j++) {
        vector<bool> seen(10, false);
        for (int i = 0; i < 9; i++) {
            if (board[i][j] != 0) {
                if (seen[board[i][j]]) return false;
                seen[board[i][j]] = true;
            }
        }
    }
    // Check boxes
    for (int bi = 0; bi < 3; bi++) {
        for (int bj = 0; bj < 3; bj++) {
            vector<bool> seen(10, false);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++) {
                    int val = board[bi*3+i][bj*3+j];
                    if (val != 0) {
                        if (seen[val]) return false;
                        seen[val] = true;
                    }
                }
        }
    }
    return true;
}

// ===================== PRESET PUZZLES =====================

Board getPresetPuzzle(int level) {
    // Easy puzzle
    Board easy = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},

        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},

        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    // Medium puzzle
    Board medium = {
        {0,0,0, 2,6,0, 7,0,1},
        {6,8,0, 0,7,0, 0,9,0},
        {1,9,0, 0,0,4, 5,0,0},

        {8,2,0, 1,0,0, 0,4,0},
        {0,0,4, 6,0,2, 9,0,0},
        {0,5,0, 0,0,3, 0,2,8},

        {0,0,9, 3,0,0, 0,7,4},
        {0,4,0, 0,5,0, 0,3,6},
        {7,0,3, 0,1,8, 0,0,0}
    };

    // Hard puzzle
    Board hard = {
        {0,0,0, 0,0,0, 0,1,2},
        {0,0,0, 0,3,5, 0,0,0},
        {0,0,0, 6,0,0, 0,7,0},

        {7,0,0, 0,0,0, 3,0,0},
        {0,0,0, 4,0,1, 0,0,0},
        {0,0,0, 0,0,0, 0,0,6},

        {0,0,0, 0,6,0, 0,0,0},
        {0,0,0, 0,0,0, 0,0,0},
        {0,4,0, 0,0,0, 0,0,0}
    };

    if (level == 1) return easy;
    if (level == 2) return medium;
    return hard;
}

// ===================== MANUAL INPUT =====================

Board getManualPuzzle() {
    Board board(9, vector<int>(9, 0));

    printHeader("ENTER SUDOKU PUZZLE");
    cout << "\n  Enter 9 rows, each with 9 digits.\n";
    cout << "  Use 0 for empty cells.\n";
    cout << "  Example row: 5 3 0 0 7 0 0 0 0\n\n";

    for (int i = 0; i < 9; i++) {
        bool validRow = false;
        while (!validRow) {
            cout << "  Row " << (i+1) << ": ";
            validRow = true;
            for (int j = 0; j < 9; j++) {
                int val;
                cin >> val;
                if (cin.fail() || val < 0 || val > 9) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  Invalid! Re-enter row " << (i+1) << ": ";
                    validRow = false;
                    j = -1; // restart row
                    for (int k = 0; k < 9; k++) board[i][k] = 0;
                } else {
                    board[i][j] = val;
                }
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return board;
}

// ===================== SOLVE FLOW =====================

void runSolver() {
    printHeader("SUDOKU SOLVER");
    cout << "\n";
    cout << "  1. Easy Puzzle (preset)\n";
    cout << "  2. Medium Puzzle (preset)\n";
    cout << "  3. Hard Puzzle (preset)\n";
    cout << "  4. Enter My Own Puzzle\n";
    cout << "\n  Choose: ";
    int ch = getValidInt(1, 4);

    Board board, original;

    if (ch <= 3) {
        board = getPresetPuzzle(ch);
        string lvl = (ch==1?"Easy":ch==2?"Medium":"Hard");
        cout << "\n  Loading " << lvl << " puzzle...\n";
    } else {
        board = getManualPuzzle();
    }

    original = board;

    // Validate
    if (!isValidBoard(board)) {
        cout << "\n  ERROR: This puzzle has conflicts! Please check your input.\n\n";
        return;
    }

    int emptyCells = countEmpty(board);
    cout << "\n  Puzzle loaded! Empty cells: " << emptyCells << "\n";

    printHeader("PUZZLE TO SOLVE");
    printBoard(board, original);

    cout << "  Solving...\n\n";

    Board toSolve = board;
    if (solveSudoku(toSolve)) {
        printHeader("SOLVED!");
        printBoard(toSolve, original);
        cout << "  Sudoku solved successfully!\n";
        cout << "  Filled " << emptyCells << " empty cells.\n\n";
    } else {
        cout << "  This puzzle has NO solution! Please check your input.\n\n";
    }
}

// ===================== ABOUT =====================

void showAbout() {
    printHeader("ABOUT SUDOKU");
    cout << "\n";
    cout << "  Sudoku is a logic-based number placement puzzle.\n\n";
    cout << "  Rules:\n";
    cout << "  - Fill a 9x9 grid with digits 1-9\n";
    cout << "  - Each row must contain digits 1-9 (no repeats)\n";
    cout << "  - Each column must contain digits 1-9 (no repeats)\n";
    cout << "  - Each 3x3 box must contain digits 1-9 (no repeats)\n\n";
    cout << "  This solver uses BACKTRACKING algorithm.\n";
    cout << "  It tries numbers 1-9, backtracks if invalid.\n\n";
    cout << "  Developed for: Rhombix Technologies Internship\n";
    cout << "  Domain: C++ Programming - Month 3 - Task 2\n\n";
}

// ===================== MAIN MENU =====================

void showMainMenu() {
    printHeader("SUDOKU SOLVER");
    cout << "\n";
    cout << "  1. Solve a Puzzle\n";
    cout << "  2. About Sudoku\n";
    cout << "  3. Exit\n";
    cout << "\n  Choose: ";
}

// ===================== MAIN =====================

int main() {
    cout << "\n";
    printLine('*');
    cout << "  Sudoku Solver - C++ Program\n";
    cout << "  Rhombix Technologies Internship\n";
    printLine('*');
    cout << "\n";

    int choice;
    while (true) {
        showMainMenu();
        choice = getValidInt(1, 3);

        switch (choice) {
            case 1: runSolver(); break;
            case 2: showAbout(); break;
            case 3:
                printLine('=');
                cout << "  Goodbye!\n";
                printLine('=');
                return 0;
        }
    }

    return 0;
}
