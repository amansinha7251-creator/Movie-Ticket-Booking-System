// main.cpp
// ONE responsibility: the console menu and input reading - the "Cinema/main menu" role.
// Must NOT: contain booking/pricing/payment logic itself - it only reads input and calls
//           into Cinema / BookingService / TicketPrinter.
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <limits>

#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "13_BookingService.cpp"

// ---- small input helpers (NFR3: invalid input must not crash the program) ----

int readMenuChoice(int minVal, int maxVal) {
    int choice;
    while (true) {
        std::cout << "Choose: ";
        if (std::cin >> choice && choice >= minVal && choice <= maxVal) return choice;
        std::cout << "  Invalid choice. Please enter a number between " << minVal << " and " << maxVal << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::vector<std::string> splitSeatList(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // trim whitespace
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != std::string::npos) result.push_back(token.substr(start, end - start + 1));
    }
    return result;
}

// ---- demo data setup ----

void setupDemoData(Cinema& cinema) {
    cinema.addMovie(Movie("3 Idiots", "Hindi", 170));
    cinema.addMovie(Movie("Interstellar", "English", 169));

    cinema.addScreen(Screen(1));
    cinema.addScreen(Screen(2));

    cinema.scheduleShow(0, 0, "06:00 PM");   // 3 Idiots on Screen-1
    cinema.scheduleShow(0, 1, "09:00 PM");   // 3 Idiots on Screen-2
    cinema.scheduleShow(1, 1, "10:00 PM");   // Interstellar on Screen-2
}

// ---- menu actions ----

void listMovies(Cinema& cinema) {
    const auto& movies = cinema.getMovies();
    if (movies.empty()) { std::cout << "\n  No movies are currently playing.\n"; return; }
    std::cout << "\n";
    for (size_t i = 0; i < movies.size(); i++) {
        std::cout << "  [" << (i + 1) << "] " << movies[i].getTitle()
                  << "\t" << movies[i].getLanguage()
                  << "\t" << movies[i].getDurationMinutes() << " min\n";
    }
}

Show* chooseShowForMovie(Cinema& cinema) {
    listMovies(cinema);
    const auto& movies = cinema.getMovies();
    if (movies.empty()) return nullptr;
    std::cout << "Choose movie: ";
    int movieChoice = readMenuChoice(1, (int)movies.size());

    std::vector<Show*> shows = cinema.getShowsForMovie(movieChoice - 1);
    if (shows.empty()) { std::cout << "\n  No shows scheduled for this movie.\n"; return nullptr; }

    std::cout << "\n";
    for (size_t i = 0; i < shows.size(); i++) {
        std::cout << "  [" << (i + 1) << "] Screen-" << shows[i]->getScreen()->getScreenNumber()
                  << "\t" << shows[i]->getStartTime() << "\n";
    }
    std::cout << "Choose show: ";
    int showChoice = readMenuChoice(1, (int)shows.size());
    return shows[showChoice - 1];
}

void bookFlow(Cinema& cinema, BookingService& bookingService, const Customer& customer) {
    Show* show = chooseShowForMovie(cinema);
    if (show == nullptr) return;

    show->printLayout();

    std::cout << "\nSeats (e.g. A1,B2): ";
    std::string line;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, line);
    std::vector<std::string> seatNumbers = splitSeatList(line);

    if (seatNumbers.empty()) { std::cout << "  No seats entered.\n"; return; }

    bool allValid = true;
    std::vector<ShowSeat*> seats = bookingService.reserveSeats(*show, seatNumbers, allValid);
    if (!allValid) {
        // covers edge case 1 (already BOOKED) and invalid seat numbers - nothing changes
        std::cout << "  Booking rejected: one or more seats are invalid or already BOOKED. Nothing was changed.\n";
        return;
    }

    for (ShowSeat* s : seats) {
        std::cout << "  " << s->getSeat()->getSeatNumber() << " "
                  << seatTypeToString(s->getSeat()->getType()) << " Rs."
                  << PriceCalculator::priceOf(s->getSeat()->getType()) << "\n";
    }
    double total = bookingService.priceSeats(seats);
    std::cout << "  TOTAL          Rs." << total << "\n";

    std::cout << "\nPay by: 1.UPI  2.Card  3.Cash > ";
    int payChoice = readMenuChoice(1, 3);

    std::unique_ptr<Payment> payment;
    if (payChoice == 1) payment = std::make_unique<UpiPayment>();
    else if (payChoice == 2) payment = std::make_unique<CardPayment>();
    else payment = std::make_unique<CashPayment>();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Confirm payment of Rs." << total << "? (y = pay, n = simulate a failed payment): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    // FR6: a failed payment must not confirm the booking - simulated here by sending a non-positive
    // chargeAmount into pay(), which every Payment subclass treats as failure.
    bool wantsToPay = (!confirm.empty() && (confirm[0] == 'y' || confirm[0] == 'Y'));
    double chargeAmount = wantsToPay ? total : -1.0;

    Booking& booking = bookingService.createAndPay(*show, &customer, seats, total, chargeAmount, *payment);

    if (booking.getStatus() == BookingStatus::FAILED) {
        std::cout << "  [" << payment->methodName() << "] Payment FAILED. Seats released, booking NOT confirmed.\n";
        return;
    }

    std::cout << "  [" << payment->methodName() << "] Rs." << total << " paid successfully\n";
    bookingService.printTicket(booking);
}

void cancelFlow(BookingService& bookingService) {
    std::cout << "\nEnter Booking ID to cancel: ";
    std::string id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, id);
    if (bookingService.cancelBooking(id)) {
        std::cout << "  Booking " << id << " CANCELLED. Seats are AVAILABLE again.\n";
    } else {
        std::cout << "  No such confirmed booking with ID " << id << ".\n";
    }
}

void myTickets(BookingService& bookingService) {
    auto& bookings = bookingService.getAllBookings();
    if (bookings.empty()) { std::cout << "\n  No bookings yet.\n"; return; }
    for (Booking& b : bookings) {
        std::cout << "  " << b.getBookingId() << "  " << b.getShow()->getMovie()->getTitle()
                  << "  Rs." << b.getTotalAmount() << "  " << bookingStatusToString(b.getStatus()) << "\n";
    }
}

int main() {
    Cinema cinema("PVR Demo Cinema");
    setupDemoData(cinema);
    BookingService bookingService;
    Customer customer("Demo Customer", "9999999999");   // ASSOCIATION: customer interacts with bookingService

    std::cout << "===== MOVIE TICKET BOOKING =====\n";
    while (true) {
        std::cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\n";
        int choice = readMenuChoice(0, 4);
        switch (choice) {
            case 1: listMovies(cinema); break;
            case 2: bookFlow(cinema, bookingService, customer); break;
            case 3: cancelFlow(bookingService); break;
            case 4: myTickets(bookingService); break;
            case 0: std::cout << "Goodbye!\n"; return 0;
        }
    }
}
