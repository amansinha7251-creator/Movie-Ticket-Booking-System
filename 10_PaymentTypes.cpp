// 10_PaymentTypes.cpp
// ONE responsibility (each): actually perform ONE way of paying.
// Knows: whatever detail that method needs (kept minimal here for the console demo).
// Does: pay() either succeeds or fails for that method.
// Must NOT: know about Booking, ShowSeat, or price calculation - they only know how to move money.
#pragma once
#include <string>
#include <cstdlib>
#include "09_Payment.cpp"

// OOP: INHERITANCE - all three derive from the abstract Payment contract.
// OOP: RUNTIME POLYMORPHISM - a Payment* / Payment& calling pay() invokes the correct one of these
//      at runtime, decided by which object it actually points to (see BookingService).
class UpiPayment : public Payment {
public:
    bool pay(double amount) override {
        // Simulated: UPI payments succeed unless amount is invalid.
        return amount > 0;
    }
    std::string methodName() const override { return "UPI"; }
};

class CardPayment : public Payment {
public:
    bool pay(double amount) override {
        return amount > 0;
    }
    std::string methodName() const override { return "Card"; }
};

class CashPayment : public Payment {
public:
    bool pay(double amount) override {
        // Cash is collected in person - always succeeds if the amount is valid.
        return amount > 0;
    }
    std::string methodName() const override { return "Cash"; }
};
