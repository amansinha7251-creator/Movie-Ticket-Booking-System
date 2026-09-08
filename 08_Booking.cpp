// 08_Booking.cpp
// ONE responsibility: hold ONE booking's own record - id, which show, which seats, total, status.
// Knows: booking id, the Show it belongs to, the ShowSeats it holds, total amount, its own status.
// Does: reports its own fields, changes its own status (confirm/fail/cancel).
// Must NOT: calculate price itself, take payment itself, or print a ticket itself -
//           those are PriceCalculator's, Payment's and TicketPrinter's jobs (Single Responsibility).
#pragma once
#include <string>
#include <vector>
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"

enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

inline std::string bookingStatusToString(BookingStatus s) {
    switch (s) {
        case BookingStatus::PENDING:   return "PENDING";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::FAILED:    return "FAILED";
        case BookingStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

class Booking {
private:
    static int nextBookingId;              // OOP: STATIC MEMBER - shared by every Booking, generates unique ids

    std::string bookingId;
    Show* show;                             // ASSOCIATION: which show this booking is for
    const Customer* customer;               // ASSOCIATION: who made the booking
    std::vector<ShowSeat*> bookedSeats;     // AGGREGATION: references to ShowSeats that belong to the Show
    double totalAmount;
    BookingStatus status;

public:
    Booking(Show* show, const Customer* customer, std::vector<ShowSeat*> seats, double totalAmount)
        : show(show), customer(customer), bookedSeats(seats), totalAmount(totalAmount),
          status(BookingStatus::PENDING) {
        bookingId = "BK" + std::to_string(1000 + nextBookingId);
        nextBookingId++;
    }

    std::string getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    const Customer* getCustomer() const { return customer; }
    const std::vector<ShowSeat*>& getBookedSeats() const { return bookedSeats; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    void markConfirmed() { status = BookingStatus::CONFIRMED; }
    void markFailed() { status = BookingStatus::FAILED; }
    void markCancelled() { status = BookingStatus::CANCELLED; }
};

int Booking::nextBookingId = 1;
