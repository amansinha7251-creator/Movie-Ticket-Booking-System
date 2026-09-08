// 03_Screen.cpp
// ONE responsibility: represent one auditorium (screen number) and own its physical seats.
// Knows: screen number, the fixed list of Seats in this auditorium.
// Does: builds its seat layout once, and hands out that list of seats.
// Must NOT: know about movies, shows, or booking status - a seat's booking status is per-show (ShowSeat).
#pragma once
#include <string>
#include <vector>
#include "02_Seat.cpp"

class Screen {
private:
    int screenNumber;
    std::vector<Seat> seats;   // COMPOSITION: seats are created by, and live inside, this Screen

    void buildDefaultLayout() {
        // 4 SILVER (A1-A4), 3 GOLD (B1-B3), 2 PLATINUM (C1-C2) - matches the sample demo run
        char row = 'A';
        for (int i = 1; i <= 4; i++) seats.push_back(Seat(row + std::to_string(i), SeatType::SILVER));
        row = 'B';
        for (int i = 1; i <= 3; i++) seats.push_back(Seat(row + std::to_string(i), SeatType::GOLD));
        row = 'C';
        for (int i = 1; i <= 2; i++) seats.push_back(Seat(row + std::to_string(i), SeatType::PLATINUM));
    }

public:
    explicit Screen(int screenNumber) : screenNumber(screenNumber) {
        buildDefaultLayout();
    }

    int getScreenNumber() const { return screenNumber; }
    const std::vector<Seat>& getSeats() const { return seats; }
};
