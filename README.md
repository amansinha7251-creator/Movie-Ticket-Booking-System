# Movie Ticket Booking System 

A console-based **Movie Ticket Booking System** built in modern C++, designed to demonstrate core **Object-Oriented Programming (OOP)** principles and **SOLID design practices**. The system models a real cinema workflow — browsing movies and shows, selecting seats, calculating prices, taking payment, and printing tickets — with each responsibility cleanly separated into its own class.

## Features

- View currently playing movies and their scheduled shows
- Interactive seat layout for a selected show (SILVER / GOLD / PLATINUM tiers)
- Book one or more seats at once, with rejection of the whole request if any seat is invalid or already booked
- Automatic price calculation based on seat type
- Multiple payment methods (UPI, Card, Cash) via a polymorphic payment interface
- Ticket printing for confirmed bookings
- Booking cancellation, which releases seats back to AVAILABLE
- View all bookings made so far ("My Tickets")
- Defensive input handling — invalid menu input never crashes the program

## Architecture & Design Principles

Each class has a single, clearly documented responsibility (see the header comment at the top of every file):

| # | Class | Responsibility |
|---|-------|-----------------|
| 01 | `Movie` | Holds a movie's own data (title, language, duration) |
| 02 | `Seat` | Represents one physical seat (number, type) |
| 03 | `Screen` | Represents one auditorium and owns its physical seats |
| 04 | `Cinema` | Represents the theatre; owns screens, movies, and shows |
| 05 | `Show` | A movie screening on a screen at a given time; owns its `ShowSeat`s |
| 06 | `ShowSeat` | Tracks AVAILABLE/BOOKED status of one seat for one specific show |
| 07 | `Customer` | Holds a customer's identity (name, phone) |
| 08 | `Booking` | Holds one booking record (id, show, seats, total, status) |
| 09 | `Payment` | Abstract contract defining `pay()` for all payment methods |
| 10 | `PaymentTypes` | Concrete payment methods: `UpiPayment`, `CardPayment`, `CashPayment` |
| 11 | `PriceCalculator` | Converts a list of seats into a total price |
| 12 | `TicketPrinter` | Formats and prints a ticket for a confirmed booking |
| 13 | `BookingService` | Orchestrates the end-to-end booking flow |
| — | `main.cpp` | Console menu and input handling only — no business logic |

### OOP concepts demonstrated
- **Encapsulation** — private data members exposed only through public getters/controlled methods (e.g. `ShowSeat::book()`)
- **Abstraction** — `Payment` is an abstract class with a pure virtual `pay()` method
- **Inheritance & Runtime Polymorphism** — `UpiPayment`, `CardPayment`, and `CashPayment` all derive from `Payment` and are invoked through a base class pointer/reference
- **Compile-time Polymorphism** — overloaded/delegating constructors (e.g. `Movie`)
- **Composition vs. Aggregation vs. Association** — e.g. a `Screen` *owns* its `Seat`s (composition), a `Show` *borrows* a `Movie`/`Screen` (aggregation), and a `Customer` is *linked* to a `Booking` via `BookingService` (association)
- **Static members** — `Booking::nextBookingId` generates unique IDs shared across all instances
- **Single Responsibility & Interface Segregation** — pricing, payment, printing, and orchestration are all kept in separate classes rather than bundled together

## Project Structure

```
.
├── 01_Movie.cpp
├── 02_Seat.cpp
├── 03_Screen.cpp
├── 04_Cinema.cpp
├── 05_Show.cpp
├── 06_ShowSeat.cpp
├── 07_Customer.cpp
├── 08_Booking.cpp
├── 09_Payment.cpp
├── 10_PaymentTypes.cpp
├── 11_PriceCalculator.cpp
├── 12_TicketPrinter.cpp
├── 13_BookingService.cpp
└── main.cpp
```

> Note: Files use `.cpp` with `#pragma once` and are included directly (header-style) into `main.cpp`, so the entire program compiles from a single translation unit.

## Build & Run

Requires a C++11 (or later) compiler such as `g++` or `clang++`.

```bash
# Clone the repository
git clone <your-repo-url>
cd <your-repo-folder>

# Compile
g++ -std=c++17 -o booking_system main.cpp

# Run
./booking_system
```

On Windows (MinGW):

```bash
g++ -std=c++17 -o booking_system.exe main.cpp
booking_system.exe
```

## Sample Usage

 MOVIE TICKET BOOKING 

1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit
Choose: 1

  [1] 3 Idiots       Hindi      170 min
  [2] Interstellar   English    169 min

Choose: 2
```

Booking flow: pick a movie → pick a show → view the seat layout → enter seats (e.g. `A1,B2`) → confirm price → choose a payment method → get your ticket printed.

## Seating & Pricing

| Seat Type | Rows | Price |
|-----------|------|-------|
| SILVER | A1–A4 | ₹150 |
| GOLD | B1–B3 | ₹250 |
| PLATINUM | C1–C2 | ₹400 |

## Demo Data

The program seeds itself with sample data on startup (`setupDemoData()` in `main.cpp`):

- **Movies:** *3 Idiots* (Hindi, 170 min), *Interstellar* (English, 169 min)
- **Screens:** Screen-1, Screen-2
- **Shows:** 3 Idiots @ 06:00 PM (Screen-1), 3 Idiots @ 09:00 PM (Screen-2), Interstellar @ 10:00 PM (Screen-2)

## License

This project was created for academic purposes as part of the TCS-504 course assignment. Feel free to fork and adapt for learning purposes.
