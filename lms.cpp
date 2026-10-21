#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Person
{
protected:
    int id;
    string name;

public:
    Person()
    {
        id = 0;
        name = "";
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }
};

class Student : public Person
{
public:
    void inputDetails()
    {
        cout << "Enter User ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);
    }
};

class Book
{
public:
    int id;
    int quantity;
    int available;
    string title;
    string author;

    Book()
    {
        id = 0;
        quantity = 0;
        available = 0;
    }
};

class Library
{
    Book books[100];
    int bookCount;

    struct Loan
    {
        int bookId;
        int userId;
        bool active;
    };

    Loan loans[1000];
    int loanCount;

public:
    Library()
    {
        bookCount = 0;
        loanCount = 0;

        ifstream adminFile("admins.txt");

        if (!adminFile)
        {
            ofstream createAdmin("admins.txt");
            createAdmin << "admin|admin123" << endl;
            createAdmin.close();
        }

        adminFile.close();

        ifstream userFile("users.txt");
        if (!userFile)
        {
            ofstream createUser("users.txt");
            createUser.close();
        }
        userFile.close();

        ifstream bookFile("books.txt");
        if (!bookFile)
        {
            ofstream createBook("books.txt");
            createBook.close();
        }
        bookFile.close();

        ifstream transactionFile("transactions.txt");
        if (!transactionFile)
        {
            ofstream createTransaction("transactions.txt");
            createTransaction.close();
        }
        transactionFile.close();

        loadBooks();
        loadTransactions();
    }

    int findBook(int id)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].id == id)
                return i;
        }

        return -1;
    }

    bool userExists(int id)
    {
        ifstream file("users.txt");
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            string savedId, name, password;

            getline(ss, savedId, '|');
            getline(ss, name, '|');
            getline(ss, password, '|');

            if (savedId == to_string(id))
                return true;
        }

        return false;
    }

    bool loginUser(int id, string password)
    {
        ifstream file("users.txt");
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            string savedId, name, savedPassword;

            getline(ss, savedId, '|');
            getline(ss, name, '|');
            getline(ss, savedPassword, '|');

            if (savedId == to_string(id) &&
                savedPassword == password)
                return true;
        }

        return false;
    }

    bool loginAdmin(string username, string password)
    {
        ifstream file("admins.txt");
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            string savedUsername, savedPassword;

            getline(ss, savedUsername, '|');
            getline(ss, savedPassword, '|');

            if (savedUsername == username &&
                savedPassword == password)
                return true;
        }

        return false;
    }





    
    void registerUser()
    {
        Student s;
        s.inputDetails();

        if (userExists(s.getId()))
        {
            cout << "User ID already exists!" << endl;
            return;
        }

        string password;

        cout << "Create Password: ";
        getline(cin, password);

        if (password.empty() || password.find('|') != string::npos)
        {
            cout << "Invalid password!" << endl;
            return;
        }

        ofstream file("users.txt", ios::app);

        file << s.getId() << "|"
             << s.getName() << "|"
             << password << endl;

        file.close();

        cout << "Registration successful!" << endl;
    }

    void loadBooks()
    {
        bookCount = 0;

        ifstream file("books.txt");
        string line;

        while (getline(file, line) && bookCount < 100)
        {
            stringstream ss(line);
            string id, title, author, quantity, available;

            getline(ss, id, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, quantity, '|');
            getline(ss, available, '|');

            if (id.empty())
                continue;

            books[bookCount].id = stoi(id);
            books[bookCount].title = title;
            books[bookCount].author = author;
            books[bookCount].quantity = stoi(quantity);
            books[bookCount].available = stoi(available);

            bookCount++;
        }

        file.close();
    }

    void saveBooks()
    {
        ofstream file("books.txt");

        for (int i = 0; i < bookCount; i++)
        {
            file << books[i].id << "|"
                 << books[i].title << "|"
                 << books[i].author << "|"
                 << books[i].quantity << "|"
                 << books[i].available << endl;
        }

        file.close();
    }

    void loadTransactions()
    {
        loanCount = 0;

        ifstream file("transactions.txt");
        string line;

        while (getline(file, line) && loanCount < 1000)
        {
            stringstream ss(line);
            string type, bookId, userId, days, fine;

            getline(ss, type, '|');
            getline(ss, bookId, '|');
            getline(ss, userId, '|');

            if (bookId.empty() || userId.empty())
                continue;

            int bId = stoi(bookId);
            int uId = stoi(userId);

            if (type == "ISSUE")
            {
                loans[loanCount].bookId = bId;
                loans[loanCount].userId = uId;
                loans[loanCount].active = true;
                loanCount++;
            }
            else if (type == "RETURN")
            {
                for (int i = loanCount - 1; i >= 0; i--)
                {
                    if (loans[i].bookId == bId &&
                        loans[i].userId == uId &&
                        loans[i].active)
                    {
                        loans[i].active = false;
                        break;
                    }
                }
            }
        }

        file.close();

        for (int i = 0; i < bookCount; i++)
        {
            int issued = 0;

            for (int j = 0; j < loanCount; j++)
            {
                if (loans[j].bookId == books[i].id &&
                    loans[j].active)
                {
                    issued++;
                }
            }

            books[i].available = books[i].quantity - issued;

            if (books[i].available < 0)
                books[i].available = 0;
        }

        saveBooks();
    }




    // ok
    void displayBooks()
    {
        if (bookCount == 0)
        {
            cout << "No books available." << endl;
            return;
        }

        cout << "\nID\tTitle\t\tAuthor\t\tTotal\tAvailable\n";

        for (int i = 0; i < bookCount; i++)
        {
            cout << books[i].id << "\t"
                 << books[i].title << "\t"
                 << books[i].author << "\t"
                 << books[i].quantity << "\t"
                 << books[i].available << endl;
        }
    }

    void addBook()
    {
        if (bookCount >= 100)
        {
            cout << "Book storage is full!" << endl;
            return;
        }

        Book b;

        cout << "Enter Book ID: ";
        cin >> b.id;

        if (findBook(b.id) != -1)
        {
            cout << "Book ID already exists!" << endl;
            return;
        }

        cin.ignore();

        cout << "Enter Book Title: ";
        getline(cin, b.title);

        cout << "Enter Author Name: ";
        getline(cin, b.author);

        cout << "Enter Quantity: ";
        cin >> b.quantity;

        if (b.quantity <= 0)
        {
            cout << "Quantity must be greater than zero!" << endl;
            return;
        }

        b.available = b.quantity;

        books[bookCount] = b;
        bookCount++;

        saveBooks();

        cout << "Book added successfully!" << endl;
    }

    void searchBook()
    {
        int id;

        cout << "Enter Book ID: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1)
        {
            cout << "Book not found!" << endl;
        }
        else
        {
            cout << "\nBook ID: " << books[index].id;
            cout << "\nTitle: " << books[index].title;
            cout << "\nAuthor: " << books[index].author;
            cout << "\nTotal Quantity: " << books[index].quantity;
            cout << "\nAvailable: " << books[index].available << endl;
        }
    }

    bool hasActiveLoan(int bookId, int userId)
    {
        for (int i = 0; i < loanCount; i++)
        {
            if (loans[i].bookId == bookId &&
                loans[i].userId == userId &&
                loans[i].active)
            {
                return true;
            }
        }

        return false;
    }

    void issueBook(int userId)
    {
        if (loanCount >= 1000)
        {
            cout << "Transaction storage is full!" << endl;
            return;
        }

        int id;

        cout << "Enter Book ID to issue: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1)
        {
            cout << "Book not found!" << endl;
        }
        else if (books[index].available <= 0)
        {
            cout << "Book is not available!" << endl;
        }
        else if (hasActiveLoan(id, userId))
        {
            cout << "You have already issued this book!" << endl;
        }
        else
        {
            ofstream file("transactions.txt", ios::app);

            file << "ISSUE|" << id << "|"
                 << userId << "|0|0" << endl;

            file.close();

            loans[loanCount].bookId = id;
            loans[loanCount].userId = userId;
            loans[loanCount].active = true;
            loanCount++;

            books[index].available--;

            saveBooks();

            cout << "Book issued successfully!" << endl;
        }
    }

    void returnBook(int userId)
    {
        int id;

        cout << "Enter Book ID to return: ";
        cin >> id;

        int loanIndex = -1;

        for (int i = loanCount - 1; i >= 0; i--)
        {
            if (loans[i].bookId == id &&
                loans[i].userId == userId &&
                loans[i].active)
            {
                loanIndex = i;
                break;
            }
        }

        if (loanIndex == -1)
        {
            cout << "You have not issued this book!" << endl;
            return;
        }

        int days;

        cout << "Enter number of days the book was kept: ";
        cin >> days;

        if (days < 0)
        {
            cout << "Invalid number of days!" << endl;
            return;
        }

        int fine = 0;

        if (days > 14)
            fine = (days - 14) * 5;

        ofstream file("transactions.txt", ios::app);

        file << "RETURN|" << id << "|"
             << userId << "|" << days << "|"
             << fine << endl;

        file.close();

        loans[loanIndex].active = false;

        int index = findBook(id);

        if (index != -1)
            books[index].available++;

        saveBooks();

        cout << "Book returned successfully!" << endl;
        cout << "Fine: Rs. " << fine << endl;
    }

    void viewMyIssuedBooks(int userId)
    {
        bool found = false;

        cout << "\nYour Issued Books:\n";

        for (int i = 0; i < loanCount; i++)
        {
            if (loans[i].userId == userId &&
                loans[i].active)
            {
                int index = findBook(loans[i].bookId);

                if (index != -1)
                {
                    cout << "Book ID: " << books[index].id
                         << " | Title: " << books[index].title
                         << endl;

                    found = true;
                }
            }
        }

        if (!found)
            cout << "You have no issued books." << endl;
    }

    void deleteBook()
    {
        int id;

        cout << "Enter Book ID to delete: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1)
        {
            cout << "Book not found!" << endl;
            return;
        }

        if (books[index].available != books[index].quantity)
        {
            cout << "Cannot delete a book that is currently issued!" << endl;
            return;
        }

        for (int i = index; i < bookCount - 1; i++)
        {
            books[i] = books[i + 1];
        }

        bookCount--;

        saveBooks();

        cout << "Book deleted successfully!" << endl;
    }

    void viewIssuedBooks()
    {
        bool found = false;

        cout << "\nAll Issued Books:\n";

        for (int i = 0; i < loanCount; i++)
        {
            if (loans[i].active)
            {
                int index = findBook(loans[i].bookId);

                if (index != -1)
                {
                    cout << "Book ID: " << loans[i].bookId
                         << " | Title: " << books[index].title
                         << " | User ID: " << loans[i].userId
                         << endl;

                    found = true;
                }
            }
        }

        if (!found)
            cout << "No books are currently issued." << endl;
    }

    void calculateFine()
    {
        int days;

        cout << "Enter number of days the book was kept: ";
        cin >> days;

        if (days < 0)
        {
            cout << "Invalid number of days!" << endl;
            return;
        }

        int fine = 0;

        if (days > 14)
            fine = (days - 14) * 5;

        cout << "Fine: Rs. " << fine << endl;
    }

    void libraryStatistics()
    {
        int totalBooks = 0;
        int availableBooks = 0;
        int issuedBooks = 0;

        for (int i = 0; i < bookCount; i++)
        {
            totalBooks += books[i].quantity;
            availableBooks += books[i].available;
        }

        for (int i = 0; i < loanCount; i++)
        {
            if (loans[i].active)
                issuedBooks++;
        }

        cout << "\n===== LIBRARY STATISTICS =====\n";
        cout << "Different Book Titles: " << bookCount << endl;
        cout << "Total Book Copies: " << totalBooks << endl;
        cout << "Available Copies: " << availableBooks << endl;
        cout << "Issued Copies: " << issuedBooks << endl;
    }

    void adminMenu()
    {
        int choice;

        do
        {
            cout << "\n===== ADMIN PANEL =====\n";
            cout << "1. Add New Book\n";
            cout << "2. Display All Books\n";
            cout << "3. Search for a Book\n";
            cout << "4. Delete a Book\n";
            cout << "5. View Issued Books\n";
            cout << "6. Calculate Fine\n";
            cout << "7. Library Statistics\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                deleteBook();
                break;

            case 5:
                viewIssuedBooks();
                break;

            case 6:
                calculateFine();
                break;

            case 7:
                libraryStatistics();
                break;

            case 0:
                cout << "Admin logged out." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
            }

        } while (choice != 0);
    }

    void userMenu(int userId)
    {
        int choice;

        do
        {
            cout << "\n===== USER PANEL =====\n";
            cout << "1. Display All Books\n";
            cout << "2. Search for a Book\n";
            cout << "3. Issue a Book\n";
            cout << "4. Return a Book\n";
            cout << "5. View My Issued Books\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                displayBooks();
                break;

            case 2:
                searchBook();
                break;

            case 3:
                issueBook(userId);
                break;

            case 4:
                returnBook(userId);
                break;

            case 5:
                viewMyIssuedBooks(userId);
                break;

            case 0:
                cout << "User logged out." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
            }

        } while (choice != 0);
    }



    // ok
    void menu()
    {
        int choice;

        do
        {
            cout << "\n================================\n";
            cout << "   LIBRARY MANAGEMENT SYSTEM\n";
            cout << "================================\n";
            cout << "1. Admin Login\n";
            cout << "2. User Login\n";
            cout << "3. User Registration\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
                string username, password;

                cout << "Enter Admin Username: ";
                cin >> username;

                cout << "Enter Admin Password: ";
                cin >> password;

                if (loginAdmin(username, password))
                {
                    cout << "Admin login successful!" << endl;
                    adminMenu();
                }
                else
                {
                    cout << "Invalid admin username or password!" << endl;
                }
            }
            else if (choice == 2)
            {
                int userId;
                string password;

                cout << "Enter User ID: ";
                cin >> userId;

                cout << "Enter Password: ";
                cin >> password;

                if (loginUser(userId, password))
                {
                    cout << "User login successful!" << endl;
                    userMenu(userId);
                }
                else
                {
                    cout << "Invalid user ID or password!" << endl;
                }
            }
            else if (choice == 3)
            {
                cin.ignore();
                registerUser();
            }
            else if (choice == 0)
            {
                cout << "Thank you for using the Library Management System!" << endl;
            }
            else
            {
                cout << "Invalid choice!" << endl;
            }

        } while (choice != 0);
    }
};


int main()
{
    try
    {
        Library library;
        library.menu();
    }
    catch (...)
    {
        cout << "Error occurred!" << endl;
    }

    return 0;
}
