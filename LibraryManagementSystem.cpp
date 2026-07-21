/*
    Library Management System (Console-Based)
    Rhombix Technologies Internship - C++ Programming Domain - Task 2

    Features:
    - Admin: Add/Delete books, view all books, view issued books & due dates
    - Member: Search books, view all books, issue book, return book, view due dates
    - File handling: books.txt (book data), issued.txt (issue records)
    - OOP: Inheritance (Person -> Admin/Member), Classes, Constructors, File I/O
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

// ======================= Book Class =======================
class Book {
private:
    int bookID;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

public:
    Book() : bookID(0), totalCopies(0), availableCopies(0) {}

    Book(int id, string t, string a, int total, int available)
        : bookID(id), title(t), author(a), totalCopies(total), availableCopies(available) {}

    int getID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }

    void increaseAvailable() { availableCopies++; }
    void decreaseAvailable() { if (availableCopies > 0) availableCopies--; }

    void display() const {
        cout << "ID: " << bookID
             << " | Title: " << title
             << " | Author: " << author
             << " | Total: " << totalCopies
             << " | Available: " << availableCopies << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << bookID << "|" << title << "|" << author << "|" << totalCopies << "|" << availableCopies;
        return ss.str();
    }

    static Book fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, t, a, totalStr, availStr;
        getline(ss, idStr, '|');
        getline(ss, t, '|');
        getline(ss, a, '|');
        getline(ss, totalStr, '|');
        getline(ss, availStr, '|');
        return Book(stoi(idStr), t, a, stoi(totalStr), stoi(availStr));
    }
};

// ======================= IssueRecord Class =======================
class IssueRecord {
public:
    int bookID;
    string memberName;
    string issueDate;
    string dueDate;
    bool returned;

    IssueRecord() : bookID(0), returned(false) {}

    IssueRecord(int id, string mName, string iDate, string dDate, bool ret = false)
        : bookID(id), memberName(mName), issueDate(iDate), dueDate(dDate), returned(ret) {}

    string toFileString() const {
        stringstream ss;
        ss << bookID << "|" << memberName << "|" << issueDate << "|" << dueDate << "|" << (returned ? 1 : 0);
        return ss.str();
    }

    static IssueRecord fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, mName, iDate, dDate, retStr;
        getline(ss, idStr, '|');
        getline(ss, mName, '|');
        getline(ss, iDate, '|');
        getline(ss, dDate, '|');
        getline(ss, retStr, '|');
        return IssueRecord(stoi(idStr), mName, iDate, dDate, retStr == "1");
    }
};

// ======================= Person (Base Class) =======================
class Person {
protected:
    string name;
    int id;

public:
    Person() : name(""), id(0) {}
    Person(string n, int i) : name(n), id(i) {}

    virtual void displayInfo() const = 0; // pure virtual -> abstract class

    string getName() const { return name; }
    int getId() const { return id; }

    virtual ~Person() {}
};

// ======================= Member (Derived Class) =======================
class Member : public Person {
public:
    Member() : Person() {}
    Member(string n, int i) : Person(n, i) {}

    void displayInfo() const override {
        cout << "Member -> ID: " << id << " | Name: " << name << endl;
    }
};

// ======================= Admin (Derived Class) =======================
class Admin : public Person {
private:
    string password;

public:
    Admin() : Person(), password("admin123") {}
    Admin(string n, int i, string pass) : Person(n, i), password(pass) {}

    bool authenticate(const string& inputPassword) const {
        return password == inputPassword;
    }

    void displayInfo() const override {
        cout << "Admin -> ID: " << id << " | Name: " << name << endl;
    }
};

// ======================= Library Class (Core Logic) =======================
class Library {
private:
    vector<Book> books;
    vector<IssueRecord> records;

    const string BOOKS_FILE = "books.txt";
    const string RECORDS_FILE = "issued.txt";

    string getCurrentDate() const {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[16];
        sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return string(buffer);
    }

    string addDaysToDate(const string& date, int days) const {
        tm t = {};
        sscanf(date.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
        t.tm_hour = 12; // avoid DST edge cases

        time_t time = mktime(&t);
        time += days * 24 * 60 * 60;

        tm* newDate = localtime(&time);
        char buffer[16];
        sprintf(buffer, "%04d-%02d-%02d", 1900 + newDate->tm_year, 1 + newDate->tm_mon, newDate->tm_mday);
        return string(buffer);
    }

public:
    Library() {
        loadBooks();
        loadRecords();
    }

    void loadBooks() {
        books.clear();
        ifstream file(BOOKS_FILE);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                books.push_back(Book::fromFileString(line));
        }
        file.close();
    }

    void saveBooks() const {
        ofstream file(BOOKS_FILE);
        for (const auto& b : books) file << b.toFileString() << endl;
        file.close();
    }

    void loadRecords() {
        records.clear();
        ifstream file(RECORDS_FILE);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                records.push_back(IssueRecord::fromFileString(line));
        }
        file.close();
    }

    void saveRecords() const {
        ofstream file(RECORDS_FILE);
        for (const auto& r : records) file << r.toFileString() << endl;
        file.close();
    }

    int getNextBookID() const {
        int maxID = 0;
        for (const auto& b : books) if (b.getID() > maxID) maxID = b.getID();
        return maxID + 1;
    }

    void addBook(const string& title, const string& author, int copies) {
        int newID = getNextBookID();
        books.push_back(Book(newID, title, author, copies, copies));
        saveBooks();
        cout << "Book added successfully with ID: " << newID << endl;
    }

    bool deleteBook(int bookID) {
        auto it = find_if(books.begin(), books.end(), [bookID](const Book& b) {
            return b.getID() == bookID;
        });
        if (it != books.end()) {
            books.erase(it);
            saveBooks();
            return true;
        }
        return false;
    }

    void searchBook(const string& keyword) const {
        bool found = false;
        for (const auto& b : books) {
            if (b.getTitle().find(keyword) != string::npos ||
                b.getAuthor().find(keyword) != string::npos) {
                b.display();
                found = true;
            }
        }
        if (!found) cout << "No matching books found.\n";
    }

    void listAllBooks() const {
        if (books.empty()) { cout << "No books in the library yet.\n"; return; }
        for (const auto& b : books) b.display();
    }

    bool issueBook(int bookID, const string& memberName) {
        auto it = find_if(books.begin(), books.end(), [bookID](const Book& b) {
            return b.getID() == bookID;
        });

        if (it == books.end()) { cout << "Book ID not found.\n"; return false; }
        if (it->getAvailableCopies() <= 0) { cout << "No copies available right now.\n"; return false; }

        it->decreaseAvailable();
        saveBooks();

        string issueDate = getCurrentDate();
        string dueDate = addDaysToDate(issueDate, 14); // 14-day loan period

        records.push_back(IssueRecord(bookID, memberName, issueDate, dueDate, false));
        saveRecords();

        cout << "Book issued successfully! Due date: " << dueDate << endl;
        return true;
    }

    bool returnBook(int bookID, const string& memberName) {
        for (auto& r : records) {
            if (r.bookID == bookID && r.memberName == memberName && !r.returned) {
                r.returned = true;
                saveRecords();

                auto it = find_if(books.begin(), books.end(), [bookID](const Book& b) {
                    return b.getID() == bookID;
                });
                if (it != books.end()) { it->increaseAvailable(); saveBooks(); }

                cout << "Book returned successfully.\n";
                return true;
            }
        }
        cout << "No matching active issue record found.\n";
        return false;
    }

    void viewDueDates() const {
        bool any = false;
        string today = getCurrentDate();
        for (const auto& r : records) {
            if (!r.returned) {
                cout << "Book ID: " << r.bookID
                     << " | Member: " << r.memberName
                     << " | Issued: " << r.issueDate
                     << " | Due: " << r.dueDate
                     << (r.dueDate < today ? "  [OVERDUE]" : "")
                     << endl;
                any = true;
            }
        }
        if (!any) cout << "No books currently issued.\n";
    }
};

// ======================= Helper =======================
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ======================= Menus =======================
void adminMenu(Library& lib, Admin& admin) {
    string pass;
    cout << "\nEnter admin password: ";
    cin >> pass;

    if (!admin.authenticate(pass)) {
        cout << "Incorrect password. Access denied.\n";
        return;
    }

    int choice;
    do {
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Add New Book\n";
        cout << "2. Delete Book\n";
        cout << "3. View All Books\n";
        cout << "4. View Issued Books / Due Dates\n";
        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        if (cin.fail()) { clearInputBuffer(); choice = -1; }

        switch (choice) {
            case 1: {
                clearInputBuffer();
                string title, author;
                int copies;
                cout << "Book Title: "; getline(cin, title);
                cout << "Author: "; getline(cin, author);
                cout << "Number of Copies: "; cin >> copies;
                lib.addBook(title, author, copies);
                break;
            }
            case 2: {
                int id;
                cout << "Enter Book ID to delete: ";
                cin >> id;
                cout << (lib.deleteBook(id) ? "Book deleted successfully.\n" : "Book ID not found.\n");
                break;
            }
            case 3: lib.listAllBooks(); break;
            case 4: lib.viewDueDates(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void memberMenu(Library& lib) {
    int choice;
    do {
        cout << "\n===== MEMBER MENU =====\n";
        cout << "1. Search Book\n";
        cout << "2. View All Books\n";
        cout << "3. Issue a Book\n";
        cout << "4. Return a Book\n";
        cout << "5. View My Due Dates\n";
        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        if (cin.fail()) { clearInputBuffer(); choice = -1; }

        switch (choice) {
            case 1: {
                clearInputBuffer();
                string keyword;
                cout << "Enter title or author keyword: ";
                getline(cin, keyword);
                lib.searchBook(keyword);
                break;
            }
            case 2: lib.listAllBooks(); break;
            case 3: {
                clearInputBuffer();
                int id; string name;
                cout << "Enter Book ID: "; cin >> id;
                clearInputBuffer();
                cout << "Enter your name: "; getline(cin, name);
                lib.issueBook(id, name);
                break;
            }
            case 4: {
                clearInputBuffer();
                int id; string name;
                cout << "Enter Book ID: "; cin >> id;
                clearInputBuffer();
                cout << "Enter your name: "; getline(cin, name);
                lib.returnBook(id, name);
                break;
            }
            case 5: lib.viewDueDates(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ======================= Main =======================
int main() {
    Library lib;
    Admin admin("Administrator", 1, "admin123");

    int choice;
    cout << "======================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM (C++)\n";
    cout << "   Rhombix Technologies Internship\n";
    cout << "======================================\n";

    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Admin Login\n";
        cout << "2. Member Access\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        if (cin.fail()) { clearInputBuffer(); choice = -1; }

        switch (choice) {
            case 1: adminMenu(lib, admin); break;
            case 2: memberMenu(lib); break;
            case 0: cout << "Thank you for using the Library Management System!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
