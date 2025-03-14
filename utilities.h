#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <map>
#include <ctime>
#include <cctype>

using namespace std;

// ------------------- Global Constants -------------------
const string ADMIN_PASSWORD = "admin123";

// ------------------- Ticket Price Functions -------------------
bool saveTicketPrices(double vipPrice, double normalPrice);
pair<double, double> loadTicketPrices();

// ------------------- Payment Validation Helper Functions -------------------
bool isValidMobileNumber(const string &mobile);
bool isValidCardNumber(const string &card);

// ------------------- Rewards Helper Function -------------------
int getBookingCountForUser(const string &email);

// ------------------- Date Parsing Helper Functions -------------------
time_t parseDate(const string &dateStr);
time_t parseShowtime(const string &dateStr, const string &timeStr);

// ------------------- Hidden Password Input Function -------------------
#ifdef _WIN32
    #include <conio.h>
    inline string getHiddenInput(const string &prompt) {
        cout << prompt;
        string password;
        char ch;
        while ((ch = _getch()) != '\r') { // '\r' is Enter
            if (ch == '\b') { // Handle backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b"; // Erase last '*'
                }
            } else {
                password.push_back(ch);
                cout << "*";
            }
        }
        cout << endl;
        return password;
    }
#else
    #include <termios.h>
    #include <unistd.h>
    inline string getHiddenInput(const string &prompt) {
        cout << prompt;
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ECHO;  // Disable echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        string password;
        getline(cin, password);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        cout << endl;
        return password;
    }
#endif

#endif // UTILITIES_H

