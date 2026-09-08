// 04_Cinema.cpp
// ONE responsibility: represent the theatre itself - its name, and it owns its screens.
// Knows: cinema name, list of Screens, list of Movies, list of Shows it is running.
// Does: sets up the demo data (movies/screens/shows) and looks shows up by index.
// Must NOT: read console input or print menus - that belongs to main()/the driver loop, and
//           must NOT calculate price or take payment - that belongs to PriceCalculator/Payment.
#pragma once
#include <string>
#include <vector>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "05_Show.cpp"

class Cinema {
private:
    std::string name;
    std::vector<Screen> screens;   // COMPOSITION: Screens live inside this Cinema
    std::vector<Movie> movies;     // movies known to this cinema
    std::vector<Show> shows;       // AGGREGATION of Movie+Screen, owned/created by the cinema for scheduling

public:
    explicit Cinema(std::string name) : name(name) {}

    std::string getName() const { return name; }

    void addMovie(const Movie& m) { movies.push_back(m); }
    void addScreen(const Screen& s) { screens.push_back(s); }

    // A show is built from a movie already known to this cinema and a screen already known to this cinema.
    void scheduleShow(int movieIndex, int screenIndex, const std::string& startTime) {
        shows.push_back(Show(&movies[movieIndex], &screens[screenIndex], startTime));
    }

    const std::vector<Movie>& getMovies() const { return movies; }

    std::vector<Show*> getShowsForMovie(int movieIndex) {
        std::vector<Show*> result;
        const Movie* target = &movies[movieIndex];
        for (Show& s : shows) {
            if (s.getMovie() == target) result.push_back(&s);
        }
        return result;
    }

    std::vector<Show>& getAllShows() { return shows; }
};
