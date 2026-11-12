#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// Represents one book in the library.
class Book {
private:
    int bookID;
    string title;
    string author;
    bool available;

public:
    // Parameterized constructor
    Book(int id, const string& bookTitle, const string& bookAuthor,
         bool isAvailable = true)
        : bookID(id), title(bookTitle), author(bookAuthor),
          available(isAvailable) {}

    // Getters provide controlled access to private data.
    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }

    void issue() {
        if (!available) {
            throw runtime_error("This book is already issued.");
        }
        available = false;
    }

    void returnBook() {
        if (available) {
            throw runtime_error("This book is already available.");
        }
        available = true;
    }

    void display() const {
        cout << left << setw(10) << bookID
             << setw(30) << title
             << setw(25) << author
             << (available ? "Available" : "Issued") << '\n';
    }
};

// Manages the collection of books and library operations.
class Library {
private:
    vector<Book> books;
    const string filename = "books.txt";

    Book* findBookByID(int id) {
        for (Book& book : books) {
            if (book.getBookID() == id) {
                return &book;
            }
        }
        return nullptr;
    }

    void saveToFile() const {
        ofstream outFile(filename);
        if (!outFile) {
            throw runtime_error("Unable to open the data file for saving.");
        }

        // Quoted strings allow titles/authors to contain spaces.
        for (const Book& book : books) {
            outFile << book.getBookID() << ' '
                    << quoted(book.getTitle()) << ' '
                    << quoted(book.getAuthor()) << ' '
                    << book.isAvailable() << '\n';
        }

        if (!outFile) {
            throw runtime_error("An error occurred while saving book data.");
        }
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) {
            return; // First run: the file will be created when data is saved.
        }

        int id;
        string title, author;
        bool available;

        while (inFile >> id >> quoted(title) >> quoted(author) >> available) {
            books.emplace_back(id, title, author, available);
        }
    }

public:
    Library() {
        loadFromFile();
    }

    void addBook() {
        int id;
        string title, author;

        cout << "Enter book ID: ";
        if (!(cin >> id)) {
            throw invalid_argument("Book ID must be a number.");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (id <= 0) {
            throw invalid_argument("Book ID must be greater than zero.");
        }
        if (findBookByID(id) != nullptr) {
            throw invalid_argument("A book with this ID already exists.");
        }

        cout << "Enter book title: ";
        getline(cin, title);
        cout << "Enter author name: ";
        getline(cin, author);

        if (title.empty() || author.empty()) {
            throw invalid_argument("Title and author cannot be empty.");
        }

        books.emplace_back(id, title, author);
        saveToFile();
        cout << "Book added successfully.\n";
    }

    void displayBooks() const {
        if (books.empty()) {
            cout << "No books are present in the library.\n";
            return;
        }

        cout << "\n-------------------- BOOK LIST --------------------\n";
        cout << left << setw(10) << "Book ID"
             << setw(30) << "Title"
             << setw(25) << "Author" << "Status\n";
        cout << string(75, '-') << '\n';

        for (const Book& book : books) {
            book.display();
        }
    }

    void searchBook() const {
        int searchChoice;
        cout << "Search by:\n1. Book ID\n2. Book title\nEnter choice: ";
        if (!(cin >> searchChoice)) {
            throw invalid_argument("Search choice must be a number.");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool found = false;
        if (searchChoice == 1) {
            int id;
            cout << "Enter book ID: ";
            if (!(cin >> id)) {
                throw invalid_argument("Book ID must be a number.");
            }

            for (const Book& book : books) {
                if (book.getBookID() == id) {
                    cout << "\nBook found:\n";
                    cout << left << setw(10) << "Book ID"
                         << setw(30) << "Title"
                         << setw(25) << "Author" << "Status\n";
                    book.display();
                    found = true;
                    break;
                }
            }
        } else if (searchChoice == 2) {
            string searchTitle;
            cout << "Enter book title (or part of title): ";
            getline(cin, searchTitle);

            for (const Book& book : books) {
                if (book.getTitle().find(searchTitle) != string::npos) {
                    if (!found) {
                        cout << "\nMatching book(s):\n";
                        cout << left << setw(10) << "Book ID"
                             << setw(30) << "Title"
                             << setw(25) << "Author" << "Status\n";
                    }
                    book.display();
                    found = true;
                }
            }
        } else {
            throw invalid_argument("Please choose 1 or 2.");
        }

        if (!found) {
            cout << "No matching book found.\n";
        }
    }

    void issueBook() {
        int id;
        cout << "Enter book ID to issue: ";
        if (!(cin >> id)) {
            throw invalid_argument("Book ID must be a number.");
        }

        Book* book = findBookByID(id);
        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        book->issue();
        saveToFile();
        cout << "Book issued successfully.\n";
    }

    void returnBook() {
        int id;
        cout << "Enter book ID to return: ";
        if (!(cin >> id)) {
            throw invalid_argument("Book ID must be a number.");
        }

        Book* book = findBookByID(id);
        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        book->returnBook();
        saveToFile();
        cout << "Book returned successfully.\n";
    }
};

int main() {
    try {
        Library library;
        int choice;

        do {
            cout << "\n========== LIBRARY BOOK MANAGEMENT SYSTEM ==========\n";
            cout << "1. Add a book\n";
            cout << "2. Display all books\n";
            cout << "3. Search a book (ID or title)\n";
            cout << "4. Issue a book\n";
            cout << "5. Return a book\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            try {
                switch (choice) {
                    case 1:
                        library.addBook();
                        break;
                    case 2:
                        library.displayBooks();
                        break;
                    case 3:
                        library.searchBook();
                        break;
                    case 4:
                        library.issueBook();
                        break;
                    case 5:
                        library.returnBook();
                        break;
                    case 0:
                        cout << "Thank you for using the Library System.\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << '\n';
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

        } while (choice != 0);

    } catch (const exception& e) {
        cerr << "Application error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
