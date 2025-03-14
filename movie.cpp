#include "movie.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

// Constructor
Movie::Movie(string movieID, string title, string genre, int duration, double rating)
    : movieID(movieID), title(title), genre(genre), duration(duration), rating(rating) { }

string Movie::getMovieID() const {
    return movieID;
}

string Movie::getDetails() const {
    ostringstream oss;
    oss << movieID << "|" << title << "|" << genre << "|" << duration << "|" << rating;
    return oss.str();
}

void Movie::display() const {
    cout << "Movie ID: " << movieID << ", Title: " << title
         << ", Genre: " << genre << ", Duration: " << duration
         << " minutes, Rating: " << rating << endl;
}

int Movie::getDuration() const {
    return duration;
}

void Movie::setTitle(const string &newTitle) {
    title = newTitle;
}

void Movie::setGenre(const string &newGenre) {
    genre = newGenre;
}

void Movie::setDuration(int newDuration) {
    duration = newDuration;
}

void Movie::setRating(double newRating) {
    rating = newRating;
}

vector<Movie> loadMovies() {
    vector<Movie> movies;
    ifstream fin("movies.txt");
    if (!fin)
        return movies;
    string line;
    while (getline(fin, line)) {
        if (line.empty())
            continue;
        stringstream ss(line);
        string movieID, title, genre, token;
        int duration;
        double rating;
        getline(ss, movieID, '|');
        getline(ss, title, '|');
        getline(ss, genre, '|');
        getline(ss, token, '|');
        duration = stoi(token);
        getline(ss, token, '|');
        rating = stod(token);
        movies.push_back(Movie(movieID, title, genre, duration, rating));
    }
    fin.close();
    return movies;
}

bool saveMovie(const Movie &movie) {
    ofstream fout("movies.txt", ios::app);
    if (!fout)
        return false;
    fout << movie.getDetails() << "\n";
    fout.close();
    return true;
}

bool updateAllMovies(const vector<Movie>& movies) {
    ofstream fout("movies.txt", ios::trunc);
    if (!fout)
        return false;
    for (auto &m : movies)
        fout << m.getDetails() << "\n";
    fout.close();
    return true;
}


