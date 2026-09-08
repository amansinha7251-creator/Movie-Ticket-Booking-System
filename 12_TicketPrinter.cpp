// 12_TicketPrinter.cpp
// ONE responsibility: format and print a ticket - PRINTING ONLY.
// Knows: nothing of its own - it only reads whatever Booking gives it.
// Does: prints booking id, movie, screen, time, seats, total amount (F7).
// Must NOT: change any booking/seat state, calculate price, or take payment - it only reads and prints.
#pragma once
#include <iostream>
#include "08_Booking.cpp"

class TicketPrinter {
public:
    // no side effects: showSeats()-style method must not book anything - this one must not confirm anything.
    void printTicket(const Booking& booking) const {
        if (booking.getStatus() != BookingStatus::CONFIRMED) {
            std::cout << "\n  Cannot print ticket - booking is not CONFIRMED.\n";
            return;
        }
        Show* show = booking.getShow();
        std::cout << "\n  ================ TICKET ================\n";
        std::cout << "  Booking ID : " << booking.getBookingId() << "\n";
        std::cout << "  Movie      : " << show->getMovie()->getTitle() << "\n";
        std::cout << "  Screen     : Screen-" << show->getScreen()->getScreenNumber()
                  << "  " << show->getStartTime() << "\n";
        std::cout << "  Seats      : ";
        const auto& seats = booking.getBookedSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            std::cout << seats[i]->getSeat()->getSeatNumber();
            if (i + 1 < seats.size()) std::cout << ", ";
        }
        std::cout << "\n  Amount     : Rs." << booking.getTotalAmount()
                  << "        Status: " << bookingStatusToString(booking.getStatus()) << "\n";
        std::cout << "  =========================================\n";
    }
};
