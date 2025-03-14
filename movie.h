#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Movie {
private:
    string movieID;
    string title;
    string genre;
    int duration;
    double rating;
public:
    Movie(string movieID, string title, string genre, int duration, double rating);
    string getMovieID() const;
    string getDetails() const;
    void display() const;
    int getDuration() const;
    // Setters for editing movie details
    void setTitle(const string &newTitle);
    void setGenre(const string &newGenre);
    void setDuration(int newDuration);
    void setRating(double newRating);
};

vector<Movie> loadMovies();
bool saveMovie(const Movie &movie);
bool updateAllMovies(const vector<Movie>& movies);

#endif // MOVIE_H


