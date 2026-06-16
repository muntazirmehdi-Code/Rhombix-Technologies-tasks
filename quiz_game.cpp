#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

// ===================== DATA STRUCTURES =====================

struct Question {
    string questionText;
    string options[4];
    int correctOption; // 0-indexed (0=A, 1=B, 2=C, 3=D)
    string topic;
};

struct User {
    string name;
    int score;
    int totalQuestions;
    int timeTaken; // in seconds
};

// ===================== QUESTION BANK =====================

vector<Question> loadQuestions() {
    vector<Question> questions;

    // --- Science Questions ---
    questions.push_back({"What is the chemical symbol for water?",
        {"H2SO4", "CO2", "H2O", "NaCl"}, 2, "Science"});

    questions.push_back({"How many planets are in our solar system?",
        {"7", "8", "9", "10"}, 1, "Science"});

    questions.push_back({"What gas do plants absorb during photosynthesis?",
        {"Oxygen", "Nitrogen", "Carbon Dioxide", "Hydrogen"}, 2, "Science"});

    // --- History Questions ---
    questions.push_back({"In which year did World War II end?",
        {"1943", "1944", "1945", "1946"}, 2, "History"});

    questions.push_back({"Who was the first President of the United States?",
        {"Abraham Lincoln", "Thomas Jefferson", "George Washington", "John Adams"}, 2, "History"});

    questions.push_back({"Which ancient wonder was located in Alexandria?",
        {"Colosseum", "Lighthouse of Alexandria", "Hanging Gardens", "Sphinx"}, 1, "History"});

    // --- Technology Questions ---
    questions.push_back({"Who is known as the father of computers?",
        {"Alan Turing", "Charles Babbage", "Bill Gates", "Steve Jobs"}, 1, "Technology"});

    questions.push_back({"What does 'CPU' stand for?",
        {"Central Processing Unit", "Computer Personal Unit", "Central Program Utility", "Core Processing Unit"}, 0, "Technology"});

    questions.push_back({"Which programming language was created by Bjarne Stroustrup?",
        {"Java", "Python", "C++", "Ruby"}, 2, "Technology"});

    // --- Geography Questions ---
    questions.push_back({"What is the capital of Pakistan?",
        {"Lahore", "Karachi", "Islamabad", "Peshawar"}, 2, "Geography"});

    questions.push_back({"Which is the longest river in the world?",
        {"Amazon", "Nile", "Yangtze", "Mississippi"}, 1, "Geography"});

    questions.push_back({"Which country has the largest area in the world?",
        {"China", "USA", "Canada", "Russia"}, 3, "Geography"});

    // --- Math Questions ---
    questions.push_back({"What is the value of Pi (approx)?",
        {"3.14159", "2.71828", "1.61803", "1.41421"}, 0, "Mathematics"});

    questions.push_back({"What is 15 x 15?",
        {"200", "215", "225", "230"}, 2, "Mathematics"});

    questions.push_back({"What is the square root of 144?",
        {"10", "11", "12", "13"}, 2, "Mathematics"});

    return questions;
}

// ===================== UTILITY FUNCTIONS =====================

void printLine(char ch = '-', int len = 55) {
    cout << string(len, ch) << endl;
}

void printHeader(const string& title) {
    printLine('=');
    int padding = (55 - title.length()) / 2;
    cout << string(padding, ' ') << title << endl;
    printLine('=');
}

void clearScreen() {
    // Works on most systems
    cout << "\n\n";
}

int getValidInt(int min, int max) {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < min || choice > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input! Enter a number between " << min << " and " << max << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

// ===================== USER REGISTRATION =====================

User registerUser() {
    User user;
    user.score = 0;
    user.totalQuestions = 0;
    user.timeTaken = 0;

    printHeader("USER REGISTRATION");
    cout << "\n  Enter your name: ";
    getline(cin, user.name);

    if (user.name.empty()) user.name = "Player";

    cout << "\n  Welcome, " << user.name << "! Get ready to play!\n";
    return user;
}

// ===================== QUIZ ENGINE =====================

void displayQuestion(const Question& q, int qNum, int total) {
    printLine();
    cout << "  Question " << qNum << "/" << total
         << "  [Topic: " << q.topic << "]\n";
    printLine();
    cout << "\n  " << q.questionText << "\n\n";
    cout << "  A) " << q.options[0] << "\n";
    cout << "  B) " << q.options[1] << "\n";
    cout << "  C) " << q.options[2] << "\n";
    cout << "  D) " << q.options[3] << "\n";
    cout << "\n  Your answer (1=A, 2=B, 3=C, 4=D): ";
}

void runQuiz(User& user, vector<Question>& questions, int numQuestions) {
    printHeader("QUIZ STARTED");
    cout << "\n  Answer each question by entering 1, 2, 3, or 4.\n";
    cout << "  Good luck, " << user.name << "!\n\n";

    // Shuffle questions randomly
    srand((unsigned)time(0));
    random_shuffle(questions.begin(), questions.end());

    if (numQuestions > (int)questions.size())
        numQuestions = questions.size();

    user.totalQuestions = numQuestions;

    vector<int> userAnswers(numQuestions);
    time_t startTime = time(0);

    for (int i = 0; i < numQuestions; i++) {
        displayQuestion(questions[i], i + 1, numQuestions);
        int ans = getValidInt(1, 4);
        userAnswers[i] = ans - 1; // convert to 0-indexed

        if (userAnswers[i] == questions[i].correctOption) {
            cout << "  ✓ Correct!\n\n";
            user.score++;
        } else {
            cout << "  ✗ Wrong!\n\n";
        }
    }

    time_t endTime = time(0);
    user.timeTaken = (int)difftime(endTime, startTime);

    // Show correct answers review
    printHeader("ANSWER REVIEW");
    cout << "\n";
    for (int i = 0; i < numQuestions; i++) {
        cout << "  Q" << (i+1) << ": " << questions[i].questionText << "\n";
        cout << "      Your Answer  : " << questions[i].options[userAnswers[i]] << "\n";
        cout << "      Correct Answer: " << questions[i].options[questions[i].correctOption];
        if (userAnswers[i] == questions[i].correctOption)
            cout << " ✓";
        else
            cout << " ✗";
        cout << "\n\n";
    }
}

// ===================== RESULTS =====================

void showResults(const User& user) {
    printHeader("QUIZ RESULTS");

    double percentage = (double)user.score / user.totalQuestions * 100.0;
    string grade;

    if (percentage >= 90)      grade = "A+ - Excellent!";
    else if (percentage >= 80) grade = "A  - Very Good!";
    else if (percentage >= 70) grade = "B  - Good";
    else if (percentage >= 60) grade = "C  - Average";
    else if (percentage >= 50) grade = "D  - Below Average";
    else                       grade = "F  - Needs Improvement";

    cout << "\n";
    cout << "  Player     : " << user.name << "\n";
    cout << "  Score      : " << user.score << " / " << user.totalQuestions << "\n";
    cout << "  Percentage : " << fixed << setprecision(1) << percentage << "%\n";
    cout << "  Grade      : " << grade << "\n";
    cout << "  Time Taken : " << user.timeTaken << " seconds\n";
    cout << "\n";
    printLine('=');
}

// ===================== LEADERBOARD =====================

vector<User> leaderboard;

void updateLeaderboard(const User& user) {
    leaderboard.push_back(user);
    // Sort by score descending, then by time ascending
    sort(leaderboard.begin(), leaderboard.end(), [](const User& a, const User& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.timeTaken < b.timeTaken;
    });
}

void showLeaderboard() {
    if (leaderboard.empty()) {
        cout << "\n  No scores yet!\n";
        return;
    }
    printHeader("LEADERBOARD");
    cout << "\n";
    cout << "  " << left << setw(5) << "Rank"
         << setw(20) << "Name"
         << setw(10) << "Score"
         << setw(12) << "Percentage"
         << "Time(s)\n";
    printLine();

    for (int i = 0; i < (int)leaderboard.size(); i++) {
        double pct = (double)leaderboard[i].score / leaderboard[i].totalQuestions * 100.0;
        cout << "  " << left << setw(5) << (i+1)
             << setw(20) << leaderboard[i].name
             << setw(10) << (to_string(leaderboard[i].score) + "/" + to_string(leaderboard[i].totalQuestions))
             << setw(12) << (to_string((int)pct) + "%")
             << leaderboard[i].timeTaken << "s\n";
    }
    cout << "\n";
}

// ===================== MAIN MENU =====================

void showMainMenu() {
    printHeader("ONLINE QUIZ GAME");
    cout << "\n";
    cout << "  1. Start New Quiz\n";
    cout << "  2. View Leaderboard\n";
    cout << "  3. About\n";
    cout << "  4. Exit\n";
    cout << "\n  Choose an option: ";
}

void showAbout() {
    printHeader("ABOUT");
    cout << "\n";
    cout << "  Online Quiz Game - C++ Project\n";
    cout << "  Developed for Rhombix Technologies Internship\n\n";
    cout << "  Features:\n";
    cout << "  - User Registration\n";
    cout << "  - 15 Questions across 5 Topics\n";
    cout << "  - Randomized Question Order\n";
    cout << "  - Score & Grade Calculation\n";
    cout << "  - Answer Review after Quiz\n";
    cout << "  - Leaderboard with Rankings\n";
    cout << "  - Time Tracking\n";
    cout << "\n";
}

void showDifficultyMenu(int& numQ) {
    printHeader("SELECT NUMBER OF QUESTIONS");
    cout << "\n";
    cout << "  1.  5 Questions  (Quick)\n";
    cout << "  2. 10 Questions  (Medium)\n";
    cout << "  3. 15 Questions  (Full Quiz)\n";
    cout << "\n  Your choice: ";
    int ch = getValidInt(1, 3);
    if (ch == 1) numQ = 5;
    else if (ch == 2) numQ = 10;
    else numQ = 15;
}

// ===================== MAIN =====================

int main() {
    vector<Question> questions = loadQuestions();
    int choice;

    cout << "\n";
    printLine('*');
    cout << "  Welcome to the Online Quiz Game!\n";
    cout << "  Powered by Rhombix Technologies\n";
    printLine('*');
    cout << "\n";

    while (true) {
        showMainMenu();
        choice = getValidInt(1, 4);

        switch (choice) {
            case 1: {
                User user = registerUser();
                int numQ = 10;
                showDifficultyMenu(numQ);
                runQuiz(user, questions, numQ);
                showResults(user);
                updateLeaderboard(user);
                break;
            }
            case 2:
                showLeaderboard();
                break;
            case 3:
                showAbout();
                break;
            case 4:
                printLine('=');
                cout << "  Thanks for playing! Goodbye!\n";
                printLine('=');
                return 0;
        }
    }

    return 0;
}
