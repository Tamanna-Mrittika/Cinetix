#include "utilities.h"
#include "movie.h"
#include "showtime.h"
#include "booking.h"
#include "user.h"
#include <iostream>
using namespace std;

int main()
{
    int choice;
    while (true)
    {
        cout << "\n====== Main Menu ======" << endl;
        cout << "1. Admin Panel" << endl;
        cout << "2. Customer Panel" << endl;
        cout << "3. Employee Panel" << endl;
        cout << "4. Register (Customer)" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1)
        {
            string pwd = getHiddenInput("Enter Admin Password: ");

            if (pwd == ADMIN_PASSWORD)
            {
                Admin admin(0, "Admin", "admin@example.com", ADMIN_PASSWORD);
                admin.showMenu();
            }
            else
            {
                cout << "Incorrect password." << endl;
            }
        }
        else if (choice == 2)
        {
            string email;
            cout << "Enter Email: ";
            getline(cin, email);
            string pwd = getHiddenInput("Enter Password: ");
            vector<UserRecord> users = loadUsers();
            bool found = false;
            int uid = 0;
            string uname;
            for (auto &u : users)
            {
                if (u.email == email && u.password == pwd)
                {
                    found = true;
                    uid = u.userID;
                    uname = u.name;
                    break;
                }
            }
            if (found)
            {
                Customer cust(uid, uname, email, pwd);
                cust.showMenu();
            }
            else
            {
                cout << "Invalid credentials." << endl;
            }
        }
        else if (choice == 3)
        {
            string empCode;
            cout << "Enter Employee Code: ";
            getline(cin, empCode);
            string pwd = getHiddenInput("Enter Password: ");

            vector<EmployeeRecord> employees = loadEmployees();
            bool found = false;
            int empID = 0;
            string empName;
            string empPhone;
            for (auto &e : employees)
            {
                if (e.employeeCode == empCode && e.password == pwd)
                {
                    found = true;
                    empID = e.employeeID;
                    empName = e.name;
                    empPhone = e.phone;
                    break;
                }
            }
            if (found)
            {
                Employee emp(empID, empName, empCode, empPhone, pwd);
                emp.showMenu();
            }
            else
            {
                cout << "Invalid employee credentials." << endl;
            }
        }
        else if (choice == 4)
        {
            string name, email;
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Email: ";
            getline(cin, email);
            string pwd = getHiddenInput("Enter Password: ");
            if (saveUser(name, email, pwd))
                cout << "Registration successful." << endl;
            else
                cout << "Registration failed." << endl;
        }
        else if (choice == 5)
        {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
