// 02_Seat.cpp
// ONE responsibility: represent ONE physical seat - its number and its type.
// Knows: seat number (e.g. "A1"), seat type (SILVER/GOLD/PLATINUM).
// Does: reports its number and type.
// Must NOT: know whether it is booked for any particular show - that is ShowSeat's job.
#pragma once
#include <string>

enum class SeatType { SILVER, GOLD, PLATINUM };

inline std::string seatTypeToString(SeatType t) {
    switch (t) {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
    }
    return "UNKNOWN";
}

class Seat {
private:
    std::string seatNumber;   // e.g. "A1"
    SeatType type;

public:
    Seat(std::string seatNumber, SeatType type)
        : seatNumber(seatNumber), type(type) {}

    std::string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};
