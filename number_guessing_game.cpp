/*
 * Number Guessing Game
 * Rhombix Technologies - C++ Programming Internship
 * Task 1: Month 3
 *
 * How to compile: g++ -o number_guessing_game number_guessing_game.cpp
 * How to run:     ./number_guessing_game
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// Function to play one round
bool playRound(int difficulty) {
    int minNum = 1, maxNum = 10, maxAttempts = 5;

    if (difficulty == 2) { maxNum = 50;  maxAttempts = 7; }
    if (difficulty == 3) { maxNum = 100; maxAttempts = 10; }

    // Generate random number
    srand(time(0));
    int secretNumber = rand() % maxNum + minNum;

    cout << "\n========================================\n";
    cout << "  Guess a number between " << minNum << " and " << maxNum << "\n";
    cout << "  You have " << maxAttempts << " attempts!\n";
    cout << "========================================\n";

    int guess = 0;
    int attempts = 0;

    while (attempts < maxAttempts) {
        cout << "\nAttempt " << (attempts + 1) << "/" << maxAttempts << " -- Enter your guess: ";
        cin >> guess;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }

        attempts++;

        if (guess == secretNumber) {
            cout << "\n  *** Correct! You guessed it in " << attempts << " attempt(s)! ***\n";
            return true;
        } else if (guess < secretNumber) {
            cout << "  Too LOW!  Try a higher number.";
        } else {
            cout << "  Too HIGH! Try a lower number.";
        }

        int remaining = maxAttempts - attempts;
        if (remaining > 0)
            cout << " (" << remaining << " attempt(s) left)\n";
    }

    cout << "\n  Out of attempts! The number was: " << secretNumber << "\n";
    return false;
}

int main() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║      NUMBER GUESSING GAME            ║\n";
    cout << "║   Rhombix Technologies Internship    ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
        // Choose difficulty
        int difficulty = 0;
        cout << "\nSelect Difficulty:\n";
        cout << "  1. Easy   (1-10,  5 attempts)\n";
        cout << "  2. Medium (1-50,  7 attempts)\n";
        cout << "  3. Hard   (1-100, 10 attempts)\n";
        cout << "Enter choice (1/2/3): ";
        cin >> difficulty;

        if (difficulty < 1 || difficulty > 3) {
            cout << "  Invalid choice. Setting to Medium.\n";
            difficulty = 2;
        }

        bool won = playRound(difficulty);

        if (won)
            cout << "\n  Great job! You are a guessing champion!\n";
        else
            cout << "\n  Better luck next time!\n";

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nThanks for playing! Goodbye!\n\n";
    return 0;
}
