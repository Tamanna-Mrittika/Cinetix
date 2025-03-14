#include "showtime.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Constructor
Showtime::Showtime(string showtimeID, string movieID, string time, string date,
                   string hallID, string hallType, int rows, int columns)
    : showtimeID(showtimeID), movieID(movieID), time(time), date(date),
      hallID(hallID), hallType(hallType), rows(rows), columns(columns) {
    seats.resize(rows * columns, 'O');
}

void Showtime::setSeats(const vector<char>& newSeats) {
    seats = newSeats;
}

string Showtime::getShowtimeID() const { return showtimeID; }
string Showtime::getMovieID() const { return movieID; }
int Showtime::getRows() const { return rows; }
int Showtime::getColumns() const { return columns; }
string Showtime::getHallID() const { return hallID; }
string Showtime::getHallType() const { return hallType; }
string Showtime::getTime() const { return time; }
string Showtime::getDate() const { return date; }
vector<char> Showtime::getSeats() const { return seats; }


string Showtime::toRecord() const {
    ostringstream oss;
    oss << showtimeID << "|" << movieID << "|" << time << "|" << date << "|"
        << hallID << "|" << hallType << "|" << rows << "|" << columns << "|";
    for (char c : seats)
        oss << c;
    return oss.str();
}

void Showtime::displaySeatingArrangement() const {
    cout << "\nSeating Arrangement for Showtime " << showtimeID
         << " (" << rows << "x" << columns << "):\n       ";
    for (int j = 0; j < columns; j++)
        cout <<setw(5)<< j + 1 << " ";
    cout << "\n";
    cout << "------------------------------------------------------\n";

    for (int i = 0; i < rows; i++) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < columns; j++) {
            cout << setw(5)<< seats[i * columns + j] << " ";
        }
        cout << "\n";
    }
}

bool Showtime::bookSeat(int row, int col) {
    int index = row * columns + col;
    if (seats[index] == 'O') {
        seats[index] = 'X';
        return true;
    }
    return false;
}

bool Showtime::isSeatAvailable(int row, int col) const {
    int index = row * columns + col;
    return seats[index] == 'O';
}

vector<Showtime> loadShowtimes() {
    vector<Showtime> showtimes;
    ifstream fin("showtimes.txt");
    if (!fin)
        return showtimes;
    string line;
    while (getline(fin, line)) {
        if (line.empty())
            continue;
        stringstream ss(line);
        string showtimeID, movieID, time, date, hallID, hallType, token, seatsStr;
        int rows, columns;
        getline(ss, showtimeID, '|');
        getline(ss, movieID, '|');
        getline(ss, time, '|');
        getline(ss, date, '|');
        getline(ss, hallID, '|');
        getline(ss, hallType, '|');
        getline(ss, token, '|');
        rows = stoi(token);
        getline(ss, token, '|');
        columns = stoi(token);
        getline(ss, seatsStr, '|');
        Showtime st(showtimeID, movieID, time, date, hallID, hallType, rows, columns);
        vector<char> seats(seatsStr.begin(), seatsStr.end());
        st.setSeats(seats);
        showtimes.push_back(st);
    }
    fin.close();
    return showtimes;
}

bool saveShowtime(const Showtime &st) {
    ofstream fout("showtimes.txt", ios::app);
    if (!fout)
        return false;
    fout << st.toRecord() << "\n";
    fout.close();
    return true;
}

bool updateAllShowtimes(const vector<Showtime> &showtimes) {
    ofstream fout("showtimes.txt", ios::trunc);
    if (!fout)
        return false;
    for (auto &st : showtimes)
        fout << st.toRecord() << "\n";
    fout.close();
    return true;
}


