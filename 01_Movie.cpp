// 01_Movie.cpp
// ONE responsibility: hold a movie's own data (title, language, duration). Nothing else.
// Knows: its title, language, duration.
// Does: exposes that data through getters.
// Must NOT: know about screens, shows, seats, or prices.
#pragma once
#include <string>

// OOP: ENCAPSULATION - data members are private, only reachable via public getters.
class Movie {
private:
    std::string title;
    std::string language;
    int durationMinutes;

public:
    // Compile-time polymorphism example: overloaded constructor with a default duration.
    Movie(std::string title, std::string language, int durationMinutes)
        : title(title), language(language), durationMinutes(durationMinutes) {}

    Movie(std::string title, std::string language)
        : Movie(title, language, 120) {}   // uses delegating constructor (this-based) with default 120 min

    std::string getTitle() const { return title; }
    std::string getLanguage() const { return language; }
    int getDurationMinutes() const { return durationMinutes; }
};
