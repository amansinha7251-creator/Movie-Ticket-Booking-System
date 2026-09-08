// 13_BookingService.cpp
// ONE responsibility: run the booking flow end to end - the orchestrator.
// Knows: the PriceCalculator and TicketPrinter it depends on, the list of Bookings made so far.
// Does: books seats (F4), prices them (F5), takes payment (F6), confirms/fails, prints (F7), cancels (F8).
// Must NOT: read raw console input itself or format the seat-layout screen - Cinema/main owns the menu,
//           Show owns printing its own layout. BookingService only coordinates other objects.
#pragma once
#include <vector>
#include <string>
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"

class BookingService {
private:
    PriceCalculator priceCalculator;   // COMPOSITION: created and owned by BookingService
    TicketPrinter ticketPrinter;       // COMPOSITION: created and owned by BookingService
    std::vector<Booking> bookings;     // every booking ever made, successful or not

public:
    // Step 1: try to reserve the requested seats. FR4 - if ANY seat is already booked,
    // reject the whole booking and change nothing.
    // Returns the list of ShowSeat pointers on success, or an empty vector on rejection.
    std::vector<ShowSeat*> reserveSeats(Show& show, const std::vector<std::string>& seatNumbers, bool& allValid) {
        std::vector<ShowSeat*> found;
        allValid = true;
        for (const std::string& num : seatNumbers) {
            ShowSeat* ss = show.findShowSeat(num);
            if (ss == nullptr) { allValid = false; return {}; }        // invalid seat number
            if (!ss->isAvailable()) { allValid = false; return {}; }   // already booked - reject whole booking
            found.push_back(ss);
        }
        // all valid and available - now actually mark them booked
        for (ShowSeat* ss : found) ss->book();
        return found;
    }

    double priceSeats(const std::vector<ShowSeat*>& seats) const {
        return priceCalculator.calculateTotal(seats);
    }

    // OOP: RUNTIME POLYMORPHISM - 'payment' is a reference to the abstract Payment;
    // which pay() actually runs (Upi/Card/Cash) is decided at runtime by the object passed in.
    // FR6 - exactly one payment method per booking; a failed payment must not confirm the booking.
    // 'amount' is the real price shown on the ticket; 'chargeAmount' is what is actually sent to
    // pay() (a caller can pass a non-positive chargeAmount to simulate a failed transaction).
    Booking& createAndPay(Show& show, const Customer* customer, std::vector<ShowSeat*> seats,
                           double amount, double chargeAmount, Payment& payment) {
        bookings.push_back(Booking(&show, customer, seats, amount));   // <<create>> Booking
        Booking& booking = bookings.back();

        bool success = payment.pay(chargeAmount);                      // <<create>> Payment already happened by caller
        if (success) {
            booking.markConfirmed();
        } else {
            booking.markFailed();
            for (ShowSeat* ss : seats) ss->release();   // release seats so they are free again
        }
        return booking;
    }

    void printTicket(const Booking& booking) const {
        ticketPrinter.printTicket(booking);
    }

    // F8: cancel a booking - seats become AVAILABLE again.
    // Returns false (with no state change) if the id doesn't exist or is already cancelled.
    bool cancelBooking(const std::string& bookingId) {
        for (Booking& b : bookings) {
            if (b.getBookingId() == bookingId) {
                if (b.getStatus() != BookingStatus::CONFIRMED) return false;
                for (ShowSeat* ss : b.getBookedSeats()) ss->release();
                b.markCancelled();
                return true;
            }
        }
        return false;
    }

    std::vector<Booking>& getAllBookings() { return bookings; }
};
