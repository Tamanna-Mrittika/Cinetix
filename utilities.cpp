#include "utilities.h"

bool saveTicketPrices(double vipPrice, double normalPrice) {
    ofstream fout("prices.txt", ios::trunc);
    if (!fout)
        return false;
    fout << vipPrice << "|" << normalPrice;
    fout.close();
    return true;
}

pair<double, double> loadTicketPrices() {
    ifstream fin("prices.txt");
    if (!fin)
        return make_pair(200.0, 150.0);
    string line;
    getline(fin, line);
    fin.close();
    stringstream ss(line);
    string token;
    double vipPrice, normalPrice;
    getline(ss, token, '|');
    vipPrice = stod(token);
    getline(ss, token, '|');
    normalPrice = stod(token);
    return make_pair(vipPrice, normalPrice);
}

bool isValidMobileNumber(const string &mobile) {
    if (mobile.length() != 11) return false;
    for (char c : mobile) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool isValidCardNumber(const string &card) {
    if (card.length() != 12) return false;
    for (char c : card) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int getBookingCountForUser(const string &email) {
    int count = 0;
    ifstream fin("bookings.txt");
    if (!fin)
        return 0;
    string line;
    while(getline(fin, line)) {
        if(line.empty())
            continue;
        stringstream ss(line);
        string customerName, userEmail;
        getline(ss, customerName, '|');
        getline(ss, userEmail, '|');
        if (userEmail == email)
            count++;
    }
    fin.close();
    return count;
}

time_t parseDate(const string &dateStr) {
    struct tm tmDate = {0};
    istringstream ss(dateStr);
    string token;
    getline(ss, token, '/');
    tmDate.tm_mday = stoi(token);
    getline(ss, token, '/');
    tmDate.tm_mon = stoi(token) - 1;
    getline(ss, token, '/');
    tmDate.tm_year = stoi(token) - 1900;
    tmDate.tm_hour = tmDate.tm_min = tmDate.tm_sec = 0;
    return mktime(&tmDate);
}

time_t parseShowtime(const string &dateStr, const string &timeStr) {
    struct tm tmDate = {0};
    // Parse date
    istringstream ds(dateStr);
    string token;
    getline(ds, token, '/');
    tmDate.tm_mday = stoi(token);
    getline(ds, token, '/');
    tmDate.tm_mon = stoi(token) - 1;
    getline(ds, token, '/');
    tmDate.tm_year = stoi(token) - 1900;
    // Parse time
    istringstream ts(timeStr);
    int hour, minute;
    char colon;
    string ampm;
    ts >> hour >> colon >> minute >> ampm;
    if (ampm == "PM" || ampm == "pm") {
        if (hour != 12)
            hour += 12;
    } else if (ampm == "AM" || ampm == "am") {
        if (hour == 12)
            hour = 0;
    }
    tmDate.tm_hour = hour;
    tmDate.tm_min = minute;
    tmDate.tm_sec = 0;
    return mktime(&tmDate);
}


