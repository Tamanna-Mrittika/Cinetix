#include "user.h"
#include "movie.h"
#include "showtime.h"
#include "booking.h"
#include "utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
using namespace std;

// ---------- File I/O for Users ----------
vector<UserRecord> loadUsers()
{
    vector<UserRecord> users;
    ifstream fin("users.txt");
    if (!fin)
        return users;
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string token;
        UserRecord ur;
        getline(ss, token, '|');
        ur.userID = stoi(token);
        getline(ss, ur.name, '|');
        getline(ss, ur.email, '|');
        getline(ss, ur.password, '|');
        users.push_back(ur);
    }
    fin.close();
    return users;
}

bool saveUser(const string &name, const string &email, const string &password)
{
    int newUserID = 1;
    vector<UserRecord> users = loadUsers();
    if (!users.empty())
        newUserID = users.size() + 1;
    ofstream fout("users.txt", ios::app);
    if (!fout)
        return false;
    fout << newUserID << "|" << name << "|" << email << "|" << password << "\n";
    fout.close();
    return true;
}

// ---------- File I/O for Employees ----------
vector<EmployeeRecord> loadEmployees()
{
    vector<EmployeeRecord> employees;
    ifstream fin("employees.txt");
    if (!fin)
        return employees;
    string line;
    while(getline(fin, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string token;
        EmployeeRecord er;
        getline(ss, token, '|');
        er.employeeID = stoi(token);
        getline(ss, er.employeeCode, '|');
        getline(ss, er.name, '|');
        getline(ss, er.phone, '|');
        getline(ss, er.password, '|');
        employees.push_back(er);
    }
    fin.close();
    return employees;
}

bool saveEmployee(int employeeID, string employeeCode, string name, string phone, string password)
{
    ofstream fout("employees.txt", ios::app);
    if (!fout)
        return false;
    fout << employeeID << "|" << employeeCode << "|" << name << "|" << phone << "|" << password << "\n";
    fout.close();
    return true;
}

// ---------- File I/O for Assignments ----------
vector<AssignmentRecord> loadAssignments()
{
    vector<AssignmentRecord> assignments;
    ifstream fin("assignments.txt");
    if (!fin)
        return assignments;
    string line;
    while(getline(fin, line))
    {
        if(line.empty())
            continue;
        stringstream ss(line);
        AssignmentRecord ar;
        string token;
        getline(ss, token, '|');
        ar.employeeID = stoi(token);
        getline(ss, ar.employeeCode, '|');
        getline(ss, ar.showtimeID, '|');
        assignments.push_back(ar);
    }
    fin.close();
    return assignments;
}

bool saveAssignment(const AssignmentRecord &assignment)
{
    ofstream fout("assignments.txt", ios::app);
    if (!fout)
        return false;
    fout << assignment.employeeID << "|" << assignment.employeeCode << "|" << assignment.showtimeID << "\n";
    fout.close();
    return true;
}

// Helper function to update the assignments file (used in removeEmployeeAssignment)
bool updateAssignmentsFile(const vector<AssignmentRecord>& assignments) {
    ofstream fout("assignments.txt", ios::trunc);
    if (!fout)
        return false;
    for (auto &a : assignments)
        fout << a.employeeID << "|" << a.employeeCode << "|" << a.showtimeID << "\n";
    fout.close();
    return true;
}

// ---------- UserBase Implementation ----------
UserBase::UserBase(int userID, string name, string email, string password)
    : userID(userID), name(name), email(email), password(password) { }

string UserBase::getEmail() const
{
    return email;
}

string UserBase::getName() const
{
    return name;
}

// ---------- Customer Implementation ----------
Customer::Customer(int userID, string name, string email, string password)
    : UserBase(userID, name, email, password) { }

void Customer::showMenu()
{
    int choice;
    while (true)
    {
        cout << "\n====== Customer Menu ======" << endl;
        cout << "1. List Movies and Showtimes" << endl;
        cout << "2. Book Ticket" << endl;
        cout << "3. View Purchased Tickets" << endl;
        cout << "4. Cancel Ticket" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1)
            listMoviesAndShowtimes();
        else if (choice == 2)
            bookTicket();
        else if (choice == 3)
            viewPurchasedTickets();
        else if (choice == 4)
            cancelTicket();
        else if (choice == 5)
            break;
        else
            cout << "Invalid choice. Try again." << endl;
    }
}

void Customer::listMoviesAndShowtimes()
{
    vector<Movie> movies = loadMovies();
    vector<Showtime> showtimes = loadShowtimes();
    cout << "\n====== Movies and Showtimes ======" << endl;
    for (auto &m : movies)
    {
        m.display();
        for (auto &st : showtimes)
        {
            if (st.getMovieID() == m.getMovieID())
            {
                cout << "   Showtime ID: " << st.getShowtimeID()
                     << " | Date: " << st.getDate()
                     << " | Time: " << st.getTime()
                     << " | Hall: " << st.getHallID()
                     << " (" << st.getHallType() << ")" << endl;
            }
        }
    }
}

bool Customer::processPayment(double amount)
{
    cout << "\nTotal amount to pay: " << amount << " TK" << endl;
    cout << "Select Payment Method:\n1. Bkash\n2. Nagad\n3. Card\nEnter choice: ";
    int method;
    cin >> method;
    cin.ignore();
    string paymentMethod;
    if (method == 1)
        paymentMethod = "Bkash";
    else if (method == 2)
        paymentMethod = "Nagad";
    else if (method == 3)
        paymentMethod = "Card";
    else
    {
        cout << "Invalid payment method." << endl;
        return false;
    }
    string number;
    while (true)
    {
        cout << "Enter your " << (paymentMethod == "Card" ? "card number" : "mobile number") << ": ";
        getline(cin, number);
        if (paymentMethod == "Card")
        {
            if (isValidCardNumber(number))
                break;
            else
                cout << "Invalid card number. It must be 12 digits. Please try again." << endl;
        }
        else
        {
            if (isValidMobileNumber(number))
                break;
            else
                cout << "Invalid mobile number. It must be 11 digits. Please try again." << endl;
        }
    }
    cout << "Payment of " << amount << " TK via " << paymentMethod << " confirmed." << endl;
    return true;
}

void Customer::bookTicket()
{
    string stID;
    cout << "\nEnter Showtime ID to book: ";
    getline(cin, stID);
    vector<Showtime> showtimes = loadShowtimes();
    int index = -1;
    for (size_t i = 0; i < showtimes.size(); i++)
    {
        if (showtimes[i].getShowtimeID() == stID)
        {
            index = (int)i;
            break;
        }
    }
    if (index == -1)
    {
        cout << "Showtime not found." << endl;
        return;
    }
    showtimes[index].displaySeatingArrangement();
    int numSeats;
    cout << "\nHow many seats to book? ";
    cin >> numSeats;
    cin.ignore();
    vector<string> booked;
    for (int i = 0; i < numSeats; i++)
    {
        int r, c;
        cout << "Enter row and column (e.g., 1 2): ";
        cin >> r >> c;
        cin.ignore();
        r--;
        c--;
        if (r < 0 || r >= showtimes[index].getRows() || c < 0 || c >= showtimes[index].getColumns())
        {
            cout << "Invalid seat. Try again." << endl;
            i--;
            continue;
        }
        if (!showtimes[index].isSeatAvailable(r, c))
        {
            cout << "Seat already booked. Choose another." << endl;
            i--;
            continue;
        }
        showtimes[index].bookSeat(r, c);
        booked.push_back(to_string(r + 1) + "," + to_string(c + 1));
    }
    pair<double,double> prices = loadTicketPrices();
    double pricePerSeat = (showtimes[index].getHallType() == "VIP" ? prices.first : prices.second);
    double totalAmount = pricePerSeat * numSeats;
    int bookingCount = getBookingCountForUser(email);
    if (bookingCount >= 5)
    {
        cout << "\nCongratulations! You have watched at least 5 movies. A 10% discount applies to this booking!" << endl;
        totalAmount *= 0.9;
    }
    cout << "\nFor hall " << showtimes[index].getHallID() << " (" << showtimes[index].getHallType()
         << "), Price per seat is: " << pricePerSeat << " TK." << endl;
    cout << "Total amount for " << numSeats << " seat(s): " << totalAmount << " TK." << endl;
    cout << "Do you want to proceed with payment? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    if (confirm != 'y' && confirm != 'Y')
    {
        cout << "Booking cancelled." << endl;
        return;
    }
    if (!processPayment(totalAmount))
    {
        cout << "Payment failed. Booking cancelled." << endl;
        return;
    }
    if (updateAllShowtimes(showtimes))
    {
        cout << "Seats booked successfully." << endl;
        Booking bk(getName(), email, stID, booked, totalAmount);
        if (saveBooking(bk))
        {
            bk.displayTicket();
            if (!saveTicketFile(bk))
                cout << "Error saving ticket file." << endl;
        }
        else
            cout << "Error saving booking." << endl;
        cout << "\nTicket Confirmed for Date: " << showtimes[index].getDate()
             << ", Hall: " << showtimes[index].getHallID()
             << ", Time: " << showtimes[index].getTime() << "." << endl;
    }
    else
    {
        cout << "Error updating showtime." << endl;
    }
}

void Customer::viewPurchasedTickets()
{
    vector<Booking> bookings = loadBookings();
    vector<Booking> upcoming, past;
    time_t now = time(0);
    vector<Showtime> showtimes = loadShowtimes();
    for (auto &b : bookings)
    {
        if (b.getUserEmail() == email && b.getStatus() == "active")
        {
            for (auto &st : showtimes)
            {
                if (st.getShowtimeID() == b.getShowtimeID())
                {
                    time_t showTime = parseShowtime(st.getDate(), st.getTime());
                    if (difftime(showTime, now) >= 0)
                        upcoming.push_back(b);
                    else
                        past.push_back(b);
                    break;
                }
            }
        }
    }
    cout << "\n===== Upcoming Tickets =====" << endl;
    if (upcoming.empty())
        cout << "No upcoming tickets." << endl;
    else
    {
        for (auto &b : upcoming)
            b.displayTicket();
    }
    cout << "\n===== Past Tickets =====" << endl;
    if (past.empty())
        cout << "No past tickets." << endl;
    else
    {
        for (auto &b : past)
            b.displayTicket();
    }
}

void Customer::cancelTicket() {
    vector<Booking> bookings = loadBookings();
    vector<int> validIndices;  // Maps displayed number to actual booking index
    cout << "\n===== Your Upcoming Active Bookings =====" << endl;
    int count = 0;
    time_t now = time(0);
    vector<Showtime> showtimes = loadShowtimes();

    // List active upcoming bookings
    for (int i = 0; i < (int)bookings.size(); i++) {
        if (bookings[i].getUserEmail() == email && bookings[i].getStatus() == "active") {
            for (auto &st : showtimes) {
                if (st.getShowtimeID() == bookings[i].getShowtimeID()) {
                    time_t showTime = parseShowtime(st.getDate(), st.getTime());
                    if (difftime(showTime, now) >= 0) {
                        cout << count + 1 << ": " << bookings[i].getTicketInfo() << endl;
                        validIndices.push_back(i);
                        count++;
                    }
                    break;
                }
            }
        }
    }

    if (validIndices.empty()) {
        cout << "No upcoming active bookings to cancel." << endl;
        return;
    }

    cout << "Enter the number of the booking to cancel: ";
    int selection;
    cin >> selection;
    cin.ignore();
    if (selection < 1 || selection > (int)validIndices.size()) {
        cout << "Invalid selection." << endl;
        return;
    }

    int indexToCancel = validIndices[selection - 1];
    time_t currentTime = time(0);
    Booking &bookingToCancel = bookings[indexToCancel];

    // Calculate refund amount (as before)
    double refundPercentage = 0.0;
    double hoursDiff = difftime(currentTime, bookingToCancel.getBookingTime()) / 3600.0;
    if (hoursDiff <= 24)
        refundPercentage = 0.90;
    else if (hoursDiff <= 48)
        refundPercentage = 0.50;
    else
        refundPercentage = 0.0;
    double refundAmount = bookingToCancel.getPrice() * refundPercentage;
    cout << "Refund Amount: " << refundAmount << " TK (Refund Percentage: " << refundPercentage * 100 << "%)" << endl;

    // Revert the seating arrangement:
    string stID = bookingToCancel.getShowtimeID();
    // Find the corresponding showtime
    for (auto &st : showtimes) {
        if (st.getShowtimeID() == stID) {
            vector<char> seats = st.getSeats();
            vector<string> booked = bookingToCancel.getBookedSeats();
            for (const auto &seatStr : booked) {
                size_t commaPos = seatStr.find(',');
                if (commaPos != string::npos) {
                    int row = stoi(seatStr.substr(0, commaPos)) - 1;
                    int col = stoi(seatStr.substr(commaPos + 1)) - 1;
                    int idx = row * st.getColumns() + col;
                    if (idx >= 0 && idx < (int)seats.size())
                        seats[idx] = 'O';
                }
            }
            st.setSeats(seats);
            break;
        }
    }

    bookingToCancel.setStatus("cancelled");

    if (updateBookingsFile(bookings) && updateAllShowtimes(showtimes))
        cout << "Booking cancelled. Refund of " << refundAmount << " TK processed." << endl;
    else
        cout << "Error updating bookings or showtime seating arrangement." << endl;
}


// ---------- Employee Implementation ----------
Employee::Employee(int userID, string name, string employeeCode, string phone, string password)
    : UserBase(userID, name, "", password), employeeCode(employeeCode), phone(phone) { }

string Employee::getEmployeeCode() const
{
    return employeeCode;
}

string Employee::getPhone() const
{
    return phone;
}

void Employee::viewAssignmentDetails()
{
    vector<AssignmentRecord> assignments = loadAssignments();
    bool foundAssignment = false;
    for (auto &assignment : assignments)
    {
        if (assignment.employeeID == this->userID)
        {
            foundAssignment = true;
            vector<Showtime> showtimes = loadShowtimes();
            Showtime assignedShowtime("", "", "", "", "", "", 0, 0);
            bool showtimeFound = false;
            for (auto &st : showtimes)
            {
                if (st.getShowtimeID() == assignment.showtimeID)
                {
                    assignedShowtime = st;
                    showtimeFound = true;
                    break;
                }
            }
            if (!showtimeFound)
            {
                cout << "Assigned showtime " << assignment.showtimeID << " not found." << endl;
                continue;
            }
            vector<Movie> movies = loadMovies();
            Movie assignedMovie("", "", "", 0, 0.0);
            bool movieFound = false;
            for (auto &m : movies)
            {
                if (m.getMovieID() == assignedShowtime.getMovieID())
                {
                    assignedMovie = m;
                    movieFound = true;
                    break;
                }
            }
            cout << "\nAssignment for Showtime " << assignedShowtime.getShowtimeID() << ":" << endl;
            if (movieFound)
                cout << "Movie: " << assignedMovie.getDetails() << endl;
            else
                cout << "Movie details not found." << endl;
            cout << "Showtime: " << assignedShowtime.getDate() << " " << assignedShowtime.getTime() << endl;
            cout << "Hall: " << assignedShowtime.getHallID() << " (" << assignedShowtime.getHallType() << ")" << endl;
        }
    }
    if (!foundAssignment)
        cout << "No assignments found for you." << endl;
}

void Employee::updateShowDetails()
{
    string showtimeID;
    cout << "Enter Showtime ID to update: ";
    getline(cin, showtimeID);
    vector<AssignmentRecord> assignments = loadAssignments();
    bool assigned = false;
    for (auto &a : assignments)
    {
        if (a.employeeID == this->userID && a.showtimeID == showtimeID)
        {
            assigned = true;
            break;
        }
    }
    if (!assigned)
    {
        cout << "You are not assigned to this showtime." << endl;
        return;
    }
    vector<Showtime> showtimes = loadShowtimes();
    int stIdx = -1;
    for (size_t i = 0; i < showtimes.size(); i++)
    {
        if (showtimes[i].getShowtimeID() == showtimeID)
        {
            stIdx = (int)i;
            break;
        }
    }
    if (stIdx == -1)
    {
        cout << "Showtime not found." << endl;
        return;
    }
    vector<Movie> movies = loadMovies();
    int mIdx = -1;
    for (size_t i = 0; i < movies.size(); i++)
    {
        if (movies[i].getMovieID() == showtimes[stIdx].getMovieID())
        {
            mIdx = (int)i;
            break;
        }
    }
    cout << "\nSelect update option:" << endl;
    cout << "1. Update Showtime Time and Date" << endl;
    cout << "2. Update Movie Running Time" << endl;
    cout << "3. View Booking Details (with seat coordinates)" << endl;
    cout << "Enter choice: ";
    int opt;
    cin >> opt;
    cin.ignore();
    if (opt == 1)
    {
        string newTime, newDate;
        cout << "Enter new time (e.g., 12:30 PM): ";
        getline(cin, newTime);
        cout << "Enter new date (e.g., 1/12/2024): ";
        getline(cin, newDate);
        Showtime updatedST(showtimes[stIdx].getShowtimeID(), showtimes[stIdx].getMovieID(), newTime, newDate,
                           showtimes[stIdx].getHallID(), showtimes[stIdx].getHallType(),
                           showtimes[stIdx].getRows(), showtimes[stIdx].getColumns());
        showtimes[stIdx] = updatedST;
        if (updateAllShowtimes(showtimes))
            cout << "Showtime updated successfully." << endl;
        else
            cout << "Error updating showtime." << endl;
    }
    else if (opt == 2)
    {
        if (mIdx == -1)
        {
            cout << "Associated movie not found." << endl;
        }
        else
        {
            cout << "Current running time: " << movies[mIdx].getDuration() << " minutes." << endl;
            cout << "Enter new running time (in minutes): ";
            int newDuration;
            cin >> newDuration;
            cin.ignore();
            movies[mIdx].setDuration(newDuration);
            if (updateAllMovies(movies))
                cout << "Movie running time updated successfully." << endl;
            else
                cout << "Error updating movie details." << endl;
        }
    }
    else if (opt == 3) {
        vector<Booking> bookings = loadBookings();
        bool foundBooking = false;
        for (auto &br : bookings) {
            if (br.getShowtimeID() == showtimeID) {
                if (br.getNumberOfSeats() == 0)
                    continue;
                foundBooking = true;
                cout << "Customer Email: " << br.getUserEmail() << "\nTicket Info: " << endl;
                cout << br.getTicketInfo() << endl;
                cout << "Seat Position (Row, Column): ";
                vector<string> seats = br.getBookedSeats();
                for (const auto &seat : seats) {
                    cout << seat << " ";
                }
                cout << "\n";
            }
        }
        if (!foundBooking)
            cout << "No bookings found for this showtime." << endl;
    }
    else
    {
        cout << "Invalid option." << endl;
    }
}

void Employee::showMenu()
{
    int choice;
    while (true)
    {
        cout << "\n====== Employee Panel ======" << endl;
        cout << "1. View Assignment Details" << endl;
        cout << "2. Update Show Details" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1)
            viewAssignmentDetails();
        else if (choice == 2)
            updateShowDetails();
        else if (choice == 3)
            break;
        else
            cout << "Invalid choice. Try again." << endl;
    }
}

// ---------- Admin Implementation ----------
Admin::Admin(int userID, string name, string email, string password)
    : UserBase(userID, name, email, password) { }

/*void Admin::showMenu()
{
    int choice;
    while (true)
    {
        cout << "\n====== Admin Menu ======" << endl;
        cout << "1. Add New Movie" << endl;
        cout << "2. Add New Showtime" << endl;
        cout << "3. List Movies" << endl;
        cout << "4. Set Ticket Prices" << endl;
        cout << "5. View Sales Report" << endl;
        cout << "6. Remove Movie" << endl;
        cout << "7. Edit Movie Details" << endl;
        cout << "8. Register Employee" << endl;
        cout << "9. Assign Employee to Hall" << endl;
        cout << "10. List All Employees" << endl;
        cout << "11. Remove Employee Assignment" << endl;
        cout << "12. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1)
            addMovie();
        else if (choice == 2)
            addShowtime();
        else if (choice == 3)
            listMovies();
        else if (choice == 4)
            setTicketPrices();
        else if (choice == 5)
            generateSalesReport();
        else if (choice == 6)
            removeMovie();
        else if (choice == 7)
            editMovie();
        else if (choice == 8)
            registerEmployee();
        else if (choice == 9)
            assignEmployee();
        else if (choice == 10)
            listEmployees();
        else if (choice == 11)
            removeEmployeeAssignment();
        else if (choice == 12)
            break;
        else
            cout << "Invalid choice. Try again." << endl;
    }
}*/

void Admin::addMovie()
{
    string mID, title, genre;
    int duration;
    double rating;
    cout << "Enter Movie ID: ";
    getline(cin, mID);
    cout << "Enter Title: ";
    getline(cin, title);
    cout << "Enter Genre: ";
    getline(cin, genre);
    cout << "Enter Duration (minutes): ";
    cin >> duration;
    cin.ignore();
    cout << "Enter Fan Rating: ";
    cin >> rating;
    cin.ignore();
    Movie m(mID, title, genre, duration, rating);
    if (saveMovie(m))
        cout << "Movie added successfully." << endl;
    else
        cout << "Error adding movie." << endl;
}

void Admin::addShowtime()
{
    string stID, mID, time, date;
    int hallSelection;
    cout << "Enter Showtime ID: ";
    getline(cin, stID);
    cout << "Enter Movie ID for this showtime: ";
    getline(cin, mID);
    cout << "Enter Showtime (e.g., 11:00 PM): ";
    getline(cin, time);
    cout << "Enter Date (e.g., 1/11/2024): ";
    getline(cin, date);
    cout << "Select Hall number (1 to 5): ";
    cin >> hallSelection;
    cin.ignore();
    string hallID = "Hall-" + to_string(hallSelection);
    string hallType;
    int rows, columns;
    if (hallSelection == 1 || hallSelection == 2)
    {
        hallType = "VIP";
        rows = 5;
        columns = 5;
    }
    else if (hallSelection >= 3 && hallSelection <= 5)
    {
        hallType = "Normal";
        rows = 7;
        columns = 7;
    }
    else
    {
        cout << "Invalid hall selection. Showtime not added." << endl;
        return;
    }
    Showtime st(stID, mID, time, date, hallID, hallType, rows, columns);
    if (saveShowtime(st))
        cout << "Showtime added successfully." << endl;
    else
        cout << "Error adding showtime." << endl;
}

void Admin::listMovies() {
    vector<Movie> movies = loadMovies();
    vector<Showtime> showtimes = loadShowtimes();
    cout << "\n====== Movies List ======" << endl;
    for (auto &m : movies) {
        m.display();
        cout << "   Showtimes:" << endl;
        bool hasShowtime = false;
        for (auto &st : showtimes) {
            if (st.getMovieID() == m.getMovieID()) {
                cout << "      Showtime ID: " << st.getShowtimeID()
                     << " | Date: " << st.getDate()
                     << " | Time: " << st.getTime()
                     << " | Hall: " << st.getHallID()
                     << " (" << st.getHallType() << ")" << endl;
                hasShowtime = true;
            }
        }
        if (!hasShowtime)
            cout << "      No showtimes available." << endl;
    }
}

void Admin::setTicketPrices()
{
    double vipPrice, normalPrice;
    cout << "Enter ticket price for VIP hall: ";
    cin >> vipPrice;
    cin.ignore();
    cout << "Enter ticket price for Normal hall: ";
    cin >> normalPrice;
    cin.ignore();
    if (saveTicketPrices(vipPrice, normalPrice))
        cout << "Ticket prices updated successfully." << endl;
    else
        cout << "Error updating ticket prices." << endl;
}

void Admin::removeMovie()
{
    vector<Movie> movies = loadMovies();
    cout << "\n====== Movies List ======" << endl;
    for (auto &m : movies)
        m.display();
    cout << "Enter the Movie ID to remove: ";
    string id;
    getline(cin, id);
    bool removed = false;
    vector<Movie> updated;
    for (auto &m : movies)
    {
        if (m.getMovieID() == id)
        {
            removed = true;
        }
        else
        {
            updated.push_back(m);
        }
    }
    if (!removed)
    {
        cout << "Movie ID not found." << endl;
        return;
    }
    if (updateAllMovies(updated))
        cout << "Movie removed successfully." << endl;
    else
        cout << "Error updating movies file." << endl;
}

void Admin::editMovie()
{
    vector<Movie> movies = loadMovies();
    cout << "\n====== Movies List ======" << endl;
    for (auto &m : movies)
        m.display();
    cout << "Enter the Movie ID to edit: ";
    string id;
    getline(cin, id);
    bool found = false;
    for (auto &m : movies)
    {
        if (m.getMovieID() == id)
        {
            found = true;
            cout << "Enter new Title (or press enter to keep current): ";
            string newTitle;
            getline(cin, newTitle);
            if (!newTitle.empty())
                m.setTitle(newTitle);
            cout << "Enter new Genre (or press enter to keep current): ";
            string newGenre;
            getline(cin, newGenre);
            if (!newGenre.empty())
                m.setGenre(newGenre);
            cout << "Enter new Duration in minutes (or 0 to keep current): ";
            int newDuration;
            cin >> newDuration;
            cin.ignore();
            if (newDuration != 0)
                m.setDuration(newDuration);
            cout << "Enter new Fan Rating (or 0 to keep current): ";
            double newRating;
            cin >> newRating;
            cin.ignore();
            if (newRating != 0)
                m.setRating(newRating);
            break;
        }
    }
    if (!found)
    {
        cout << "Movie ID not found." << endl;
        return;
    }
    if (updateAllMovies(movies))
        cout << "Movie details updated successfully." << endl;
    else
        cout << "Error updating movies file." << endl;
}

void Admin::registerEmployee()
{
    string name, phone, password;
    cout << "Enter Employee Name: ";
    getline(cin, name);
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    cout << "Enter Password for Employee: ";
    getline(cin, password);
    vector<EmployeeRecord> employees = loadEmployees();
    int newEmployeeID = employees.size() + 1;
    string employeeCode = "EMP" + string((newEmployeeID < 10) ? "00" : (newEmployeeID < 100 ? "0" : "")) + to_string(newEmployeeID);
    if (saveEmployee(newEmployeeID, employeeCode, name, phone, password))
        cout << "Employee registered successfully. Employee Code: " << employeeCode << endl;
    else
        cout << "Error registering employee." << endl;
}

// New function: List All Employees
void Admin::listEmployees() {
    vector<EmployeeRecord> employees = loadEmployees();
    if (employees.empty()) {
        cout << "No employees registered." << endl;
        return;
    }
    cout << "\n====== Employee List ======" << endl;
    for (auto &emp : employees) {
        cout << "Employee Code: " << emp.employeeCode
             << " | Name: " << emp.name
             << " | Phone: " << emp.phone << endl;
    }
}

void Admin::assignEmployee()
{
    string empCode;
    cout << "Enter Employee Code to assign: ";
    getline(cin, empCode);
    vector<EmployeeRecord> employees = loadEmployees();
    EmployeeRecord selectedEmployee;
    bool found = false;
    for (auto &e : employees)
    {
        if (e.employeeCode == empCode)
        {
            selectedEmployee = e;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Employee not found." << endl;
        return;
    }
    string showtimeID;
    cout << "Enter Showtime ID to assign to employee: ";
    getline(cin, showtimeID);
    vector<Showtime> showtimes = loadShowtimes();
    bool stFound = false;
    for (auto &st : showtimes)
    {
        if (st.getShowtimeID() == showtimeID)
        {
            stFound = true;
            break;
        }
    }
    if (!stFound)
    {
        cout << "Showtime not found." << endl;
        return;
    }
    AssignmentRecord assignment;
    assignment.employeeID = selectedEmployee.employeeID;
    assignment.employeeCode = selectedEmployee.employeeCode;
    assignment.showtimeID = showtimeID;
    if (saveAssignment(assignment))
        cout << "Employee " << empCode << " assigned to Showtime " << showtimeID << " successfully." << endl;
    else
        cout << "Error saving assignment." << endl;
}

// New function: Remove Employee Assignment (with optional reassignment)
void Admin::removeEmployeeAssignment() {
    vector<AssignmentRecord> assignments = loadAssignments();
    if (assignments.empty()) {
        cout << "No employee assignments found." << endl;
        return;
    }

    // Display current assignments
    cout << "\n====== Current Employee Assignments ======" << endl;
    for (auto &a : assignments) {
        cout << "Employee Code: " << a.employeeCode
             << " | Assigned to Showtime: " << a.showtimeID << endl;
    }

    // Get input for the assignment to remove
    cout << "Enter Employee Code of the assignment to remove: ";
    string empCode;
    getline(cin, empCode);
    cout << "Enter Showtime ID from which to remove the assignment: ";
    string stID;
    getline(cin, stID);

    // Remove the matching assignment(s)
    bool found = false;
    vector<AssignmentRecord> updated;
    for (auto &a : assignments) {
        if (a.employeeCode == empCode && a.showtimeID == stID) {
            found = true;
            continue; // Skip this record (remove it)
        }
        updated.push_back(a);
    }

    if (!found) {
        cout << "No matching assignment found for Employee Code: " << empCode
             << " and Showtime ID: " << stID << endl;
        return;
    }

    if (updateAssignmentsFile(updated))
        cout << "Assignment removed successfully." << endl;
    else {
        cout << "Error updating assignments file." << endl;
        return;
    }

    // Optional: prompt for reassigning an employee
    cout << "Do you want to reassign an employee to this showtime? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore();
    if (choice == 'y' || choice == 'Y') {
        assignEmployee();  // Reuse existing assignEmployee() function
    }
}

// New function: Admin::generateSalesReport as a member function
void Admin::generateSalesReport()
{
    vector<Booking> bookings = loadBookings();
    map<string, pair<int, double>> report;
    char buffer[11];
    for (auto &b : bookings)
    {
        if (b.getStatus() != "active")
            continue;
        time_t t = b.getBookingTime();
        struct tm *lt = localtime(&t);
        strftime(buffer, 11, "%d/%m/%Y", lt);
        string purchaseDate(buffer);
        report[purchaseDate].first += b.getNumberOfSeats();
        report[purchaseDate].second += b.getPrice();
    }
    ofstream fout("sales_report.txt", ios::trunc);
    if (!fout)
    {
        cout << "Error writing sales report." << endl;
        return;
    }
    fout << "Sales Report (Grouped by Purchase Date)\n";
    fout << "=======================================\n\n";
    for (auto &entry : report)
    {
        fout << "Purchase Date: " << entry.first << "\n";
        fout << "Tickets Sold: " << entry.second.first << "\n";
        fout << "Total Revenue: " << entry.second.second << " TK\n";
        fout << "---------------------------------------\n";
    }
    fout.close();
    cout << "Sales report generated and saved to sales_report.txt" << endl;
}

void Admin::showMenu()
{
    int choice;
    while (true)
    {
        cout << "\n====== Admin Menu ======" << endl;
        cout << "1. Add New Movie" << endl;
        cout << "2. Add New Showtime" << endl;
        cout << "3. List Movies" << endl;
        cout << "4. Set Ticket Prices" << endl;
        cout << "5. View Sales Report" << endl;
        cout << "6. Remove Movie" << endl;
        cout << "7. Edit Movie Details" << endl;
        cout << "8. Register Employee" << endl;
        cout << "9. Assign Employee to Hall" << endl;
        cout << "10. List All Employees" << endl;
        cout << "11. Remove Employee Assignment" << endl;
        cout << "12. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1)
            addMovie();
        else if (choice == 2)
            addShowtime();
        else if (choice == 3)
            listMovies();
        else if (choice == 4)
            setTicketPrices();
        else if (choice == 5)
            generateSalesReport();
        else if (choice == 6)
            removeMovie();
        else if (choice == 7)
            editMovie();
        else if (choice == 8)
            registerEmployee();
        else if (choice == 9)
            assignEmployee();
        else if (choice == 10)
            listEmployees();
        else if (choice == 11)
            removeEmployeeAssignment();
        else if (choice == 12)
            break;
        else
            cout << "Invalid choice. Try again." << endl;
    }
}
