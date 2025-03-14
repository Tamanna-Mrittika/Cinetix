#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
using namespace std;

// Data Structures for storing user information
struct UserRecord {
    int userID;
    string name;
    string email;
    string password;
};

struct EmployeeRecord {
    int employeeID;
    string employeeCode;
    string name;
    string phone;
    string password;
};

struct AssignmentRecord {
    int employeeID;
    string employeeCode;
    string showtimeID;
};

// Free function declarations for file I/O
vector<UserRecord> loadUsers();
bool saveUser(const string &name, const string &email, const string &password);
vector<EmployeeRecord> loadEmployees();
vector<AssignmentRecord> loadAssignments();

// Abstract Base Class for Users
class UserBase {
protected:
    int userID;
    string name;
    string email;
    string password;
public:
    UserBase(int userID, string name, string email, string password);
    virtual void showMenu() = 0;
    string getEmail() const;
    string getName() const;
};

// Customer Class
class Customer : public UserBase {
public:
    Customer(int userID, string name, string email, string password);
    virtual void showMenu();
    void listMoviesAndShowtimes();
    bool processPayment(double amount);
    void bookTicket();
    void viewPurchasedTickets();
    void cancelTicket();
};

// Employee Class
class Employee : public UserBase {
private:
    string employeeCode;
    string phone;
public:
    Employee(int userID, string name, string employeeCode, string phone, string password);
    string getEmployeeCode() const;
    string getPhone() const;
    void viewAssignmentDetails();
    void updateShowDetails();
    virtual void showMenu();
};

// Admin Class
class Admin : public UserBase {
public:
    Admin(int userID, string name, string email, string password);
    virtual void showMenu();
    void addMovie();
    void addShowtime();
    void listMovies();
    void setTicketPrices();
    void generateSalesReport();
    void removeMovie();
    void editMovie();
    void registerEmployee();
    void assignEmployee();
    // New functions:
    void listEmployees();
    void removeEmployeeAssignment();
};

#endif // USER_H
