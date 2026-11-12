# Library Book Management System (C++)

A menu-driven OOP mini project for managing a small library collection.

## Features
- Add books with a unique positive book ID, title and author.
- Display all books and their availability.
- Search by book ID; if the ID is not found, search by title.
- Issue a book only when it is available.
- Return an issued book and update its availability.
- Save book records in `books.txt` so data remains available after restarting.

## OOP concepts demonstrated
- **Classes and objects:** `Book` and `Library`.
- **Constructor:** `Book` parameterized constructor and `Library` constructor.
- **Encapsulation:** book data members are private and accessed through public methods.
- **Vector:** `vector<Book>` stores the collection dynamically.
- **File handling:** `ifstream` loads records and `ofstream` saves records.
- **Strings:** book title and author are stored using `string`.
- **Exception handling:** invalid input and invalid issue/return operations are handled using exceptions.

## Requirements
- A C++ compiler that supports C++17 (for example, g++).

## Run on macOS (VS Code terminal)
Open the project folder in VS Code, then run:

```bash
g++ -std=c++17 -Wall -Wextra -o library main.cpp
./library
```

The program creates `books.txt` in the current working directory when the first book is saved. Keep this file in the same directory when running the program again.

## Menu
1. Add a book
2. Display all books
3. Search a book (ID or title)
4. Issue a book
5. Return a book
0. Exit

## Notes
- Book IDs must be unique and greater than zero.
- A book marked as issued cannot be issued again until it is returned.
- This is a basic single-user project; it does not include fines, user accounts or multi-user access.
