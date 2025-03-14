#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Booking {
private:
    string customerName;
    string userEmail;
    string showtimeID;
    vector<string> bookedSeats;  // Format: "row,column" (1-indexed)
    double price;
    time_t bookingTime;
    string status; // "active" or "cancelled"
public:
    // For new bookings
    Booking(string customerName, string userEmail, string showtimeID, vector<string> bookedSeats, double price);
    // For loading existing bookings
    Booking(string customerName, string userEmail, string showtimeID, vector<string> bookedSeats, double price, time_t bookingTime, string status);
    string toRecord() const;
    string getTicketInfo() const;
    void displayTicket() const;
    string getUserEmail() const;
    double getPrice() const;
    int getNumberOfSeats() const;
    time_t getBookingTime() const;
    string getStatus() const;
    void setStatus(const string &newStatus);
    string getShowtimeID() const;
    vector<string> getBookedSeats() const;
};

vector<Booking> loadBookings();
bool saveBooking(const Booking &booking);
bool updateBookingsFile(const vector<Booking>& bookings);
bool saveTicketFile(const Booking &booking);

#endif // BOOKING_H


