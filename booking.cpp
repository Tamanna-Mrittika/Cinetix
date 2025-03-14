#include "booking.h"
#include "showtime.h"    // For loadShowtimes() used in getTicketInfo()
#include "movie.h"       // For loadMovies() used in getTicketInfo()
#include "utilities.h"   // For parseShowtime()
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

// Constructors
Booking::Booking(string customerName, string userEmail, string showtimeID, vector<string> bookedSeats, double price)
    : customerName(customerName), userEmail(userEmail), showtimeID(showtimeID), bookedSeats(bookedSeats), price(price) {
    bookingTime = time(0);
    status = "active";
}

Booking::Booking(string customerName, string userEmail, string showtimeID, vector<string> bookedSeats, double price, time_t bookingTime, string status)
    : customerName(customerName), userEmail(userEmail), showtimeID(showtimeID), bookedSeats(bookedSeats), price(price), bookingTime(bookingTime), status(status) { }

string Booking::toRecord() const {
    ostringstream oss;
    oss << customerName << "|" << userEmail << "|" << showtimeID << "|"
        << price << "|" << bookingTime << "|" << status << "|";
    for (size_t i = 0; i < bookedSeats.size(); i++) {
        oss << bookedSeats[i];
        if (i != bookedSeats.size() - 1)
            oss << ";";
    }
    return oss.str();
}

string Booking::getTicketInfo() const {
    ostringstream oss;
    oss << "Name: " << customerName << "\n";
    oss << "Number of Seats Booked: " << bookedSeats.size() << "\n";
    // Find the matching showtime and movie details
    vector<Showtime> showtimes = loadShowtimes();
    Showtime matchingShowtime("", "", "", "", "", "", 0, 0);
    bool found = false;
    for (auto &st : showtimes) {
        if (st.getShowtimeID() == showtimeID) {
            matchingShowtime = st;
            found = true;
            break;
        }
    }
    if (found) {
        oss << "Showtime Details: Date: " << matchingShowtime.getDate()
            << ", Time: " << matchingShowtime.getTime()
            << ", Hall: " << matchingShowtime.getHallID()
            << " (" << matchingShowtime.getHallType() << ")\n";
        vector<Movie> movies = loadMovies();
        bool movieFound = false;
        for (auto &m : movies) {
            if (m.getMovieID() == matchingShowtime.getMovieID()) {
                oss << "Movie Details: " << m.getDetails() << "\n";
                movieFound = true;
                break;
            }
        }
        if (!movieFound)
            oss << "Movie Details: Not found\n";
    } else {
        oss << "Showtime Details: Not found\n";
        oss << "Movie Details: Not found\n";
    }
    oss << "Price: " << price << " TK\n";
    return oss.str();
}

void Booking::displayTicket() const {
    cout << "\n----- Ticket -----" << endl;
    cout << getTicketInfo() << endl;
    cout << "------------------" << endl;
}

string Booking::getUserEmail() const { return userEmail; }
double Booking::getPrice() const { return price; }
int Booking::getNumberOfSeats() const { return bookedSeats.size(); }
time_t Booking::getBookingTime() const { return bookingTime; }
string Booking::getStatus() const { return status; }
void Booking::setStatus(const string &newStatus) { status = newStatus; }
string Booking::getShowtimeID() const { return showtimeID; }
vector<string> Booking::getBookedSeats() const { return bookedSeats; }


vector<Booking> loadBookings() {
    vector<Booking> bookings;
    ifstream fin("bookings.txt");
    if (!fin)
        return bookings;
    string line;
    while(getline(fin, line)) {
        if(line.empty())
            continue;
        stringstream ss(line);
        string customerName, userEmail, showtimeID, token, status, seatsStr;
        double price = 0.0;
        time_t bookingTime = 0;
        try {
            getline(ss, customerName, '|');
            getline(ss, userEmail, '|');
            getline(ss, showtimeID, '|');
            getline(ss, token, '|');
            if(token.empty()) continue;
            price = stod(token);
            getline(ss, token, '|');
            if(token.empty()) continue;
            bookingTime = static_cast<time_t>(stoll(token));
            getline(ss, status, '|');
            getline(ss, seatsStr, '|');
        } catch (const invalid_argument &e) {
            cerr << "Conversion error in record: " << line << " : " << e.what() << endl;
            continue;
        }
        vector<string> seats;
        stringstream sstream(seatsStr);
        string seat;
        while(getline(sstream, seat, ';')) {
            if(!seat.empty())
                seats.push_back(seat);
        }
        bookings.push_back(Booking(customerName, userEmail, showtimeID, seats, price, bookingTime, status));
    }
    fin.close();
    return bookings;
}

bool saveBooking(const Booking &booking) {
    ofstream fout("bookings.txt", ios::app);
    if (!fout)
        return false;
    fout << booking.toRecord() << "\n";
    fout.close();
    return true;
}

bool updateBookingsFile(const vector<Booking>& bookings) {
    ofstream fout("bookings.txt", ios::trunc);
    if (!fout)
        return false;
    for (auto &b : bookings)
        fout << b.toRecord() << "\n";
    fout.close();
    return true;
}

bool saveTicketFile(const Booking &booking) {
    static int ticketCounter = 0;
    ticketCounter++;
    string filename = "ticket_" + to_string(ticketCounter) + ".txt";
    ofstream fout(filename);
    if (!fout)
        return false;
    fout << booking.getTicketInfo() << "\n";
    fout.close();
    cout << "Ticket saved to file: " << filename << endl;
    return true;
}


