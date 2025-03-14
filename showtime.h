#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <string>
#include <vector>
using namespace std;

class Showtime {
private:
    string showtimeID;
    string movieID;
    string time;
    string date;
    string hallID;
    string hallType;
    int rows;
    int columns;
    vector<char> seats;
public:
    Showtime(string showtimeID, string movieID, string time, string date,
             string hallID, string hallType, int rows, int columns);
    void setSeats(const vector<char>& newSeats);
    string getShowtimeID() const;
    string getMovieID() const;
    int getRows() const;
    int getColumns() const;
    string getHallID() const;
    string getHallType() const;
    string getTime() const;
    string getDate() const;
    string toRecord() const;
    void displaySeatingArrangement() const;
    bool bookSeat(int row, int col);
    bool isSeatAvailable(int row, int col) const;
    vector<char> getSeats() const;

};

vector<Showtime> loadShowtimes();
bool saveShowtime(const Showtime &st);
bool updateAllShowtimes(const vector<Showtime> &showtimes);

#endif // SHOWTIME_H


