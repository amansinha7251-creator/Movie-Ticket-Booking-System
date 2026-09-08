// 04_ShowSeat.cpp
// ONE responsibility: hold the AVAILABLE/BOOKED status of ONE seat FOR ONE show.
// Why this class exists and not just Seat: Seat A1 physically exists once on the screen,
// but its status is different for every show (booked for 6 PM, free for 9 PM).
// Status belongs to the show, not the physical chair.
// Knows: which physical Seat it mirrors, its current status.
// Does: reports/changes its own status (book / release), validates it isn't double-booked.
// Must NOT: calculate price, know about Booking or Payment.
#pragma once
#include "02_Seat.cpp"

enum class SeatStatus { AVAILABLE, BOOKED };

class ShowSeat {
private:
    const Seat* seat;     // ASSOCIATION/pointer back to the physical seat it mirrors (does not own it)
    SeatStatus status;

public:
    explicit ShowSeat(const Seat* seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    const Seat* getSeat() const { return seat; }
    SeatStatus getStatus() const { return status; }
    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    // ENCAPSULATION: status can only change through these controlled methods, never directly.
    bool book() {
        if (!isAvailable()) return false;   // reject an already-booked seat (FR4)
        status = SeatStatus::BOOKED;
        return true;
    }

    void release() {
        status = SeatStatus::AVAILABLE;     // used on cancellation (F8) and on payment failure (F6)
    }
};
