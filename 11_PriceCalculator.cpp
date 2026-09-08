// 11_PriceCalculator.cpp
// ONE responsibility: turn a list of seats into a total amount. Nothing else.
// Knows: the fixed price of each seat type (constants, not magic numbers).
// Does: prices one seat type, prices a whole list of ShowSeats.
// Must NOT: know about payment, booking ids, or printing.
#pragma once
#include <vector>
#include "06_ShowSeat.cpp"

class PriceCalculator {
public:
    // constants instead of magic numbers (clean-code checklist)
    static constexpr double SILVER_PRICE = 150.0;
    static constexpr double GOLD_PRICE = 250.0;
    static constexpr double PLATINUM_PRICE = 400.0;

    static double priceOf(SeatType type) {
        switch (type) {
            case SeatType::SILVER:   return SILVER_PRICE;
            case SeatType::GOLD:     return GOLD_PRICE;
            case SeatType::PLATINUM: return PLATINUM_PRICE;
        }
        return 0.0;
    }

    // pure function - no side effects, does ONE thing
    double calculateTotal(const std::vector<ShowSeat*>& seats) const {
        double total = 0.0;
        for (ShowSeat* s : seats) {
            total += priceOf(s->getSeat()->getType());
        }
        return total;
    }
};
