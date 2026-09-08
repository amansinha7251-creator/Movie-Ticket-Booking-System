// 05_Show.cpp
// ONE responsibility: represent one screening = a Movie on a Screen at a time, and own its ShowSeats.
// Knows: which movie (borrowed), which screen (borrowed), start time, this show's own seat statuses.
// Does: builds one ShowSeat per physical seat, prints its own seat layout (F3), finds a ShowSeat by number.
// Must NOT: calculate price, take payment, or print a full ticket.
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"

class Show {
private:
    const Movie* movie;      // AGGREGATION: Show borrows a Movie, does not own/destroy it
    const Screen* screen;    // AGGREGATION: Show borrows a Screen, does not own/destroy it
    std::string startTime;   // e.g. "06:00 PM"
    std::vector<ShowSeat> showSeats;   // COMPOSITION: these belong only to this Show

public:
    Show(const Movie* movie, const Screen* screen, std::string startTime)
        : movie(movie), screen(screen), startTime(startTime) {
        for (const Seat& s : screen->getSeats()) {
            showSeats.push_back(ShowSeat(&s));
        }
    }

    const Movie* getMovie() const { return movie; }
    const Screen* getScreen() const { return screen; }
    std::string getStartTime() const { return startTime; }
    std::vector<ShowSeat>& getShowSeats() { return showSeats; }

    // returns nullptr if no such seat number exists on this show's screen
    ShowSeat* findShowSeat(const std::string& seatNumber) {
        for (ShowSeat& ss : showSeats) {
            if (ss.getSeat()->getSeatNumber() == seatNumber) return &ss;
        }
        return nullptr;
    }

    // F3: display the seat layout with AVAILABLE / BOOKED status, grouped by type
    void printLayout() const {
        std::cout << "\n  SCREEN-" << screen->getScreenNumber() << "  " << startTime
                  << "  |  " << movie->getTitle() << "\n";
        printRow("SILVER", SeatType::SILVER);
        printRow("GOLD", SeatType::GOLD);
        printRow("PLATINUM", SeatType::PLATINUM);
        std::cout << "\n  ( [ ] = available   [X] = booked )\n";
    }

private:
    void printRow(const std::string& label, SeatType type) const {
        std::cout << "  " << std::left << std::setw(9) << label;
        for (const ShowSeat& ss : showSeats) {
            if (ss.getSeat()->getType() == type) {
                std::cout << ss.getSeat()->getSeatNumber()
                          << (ss.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        std::cout << "\n";
    }
};
