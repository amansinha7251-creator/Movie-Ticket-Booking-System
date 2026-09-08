// 09_Payment.cpp
// ONE responsibility: define the payment CONTRACT - pay(amount) - and nothing about how it works.
// Knows: nothing of its own (no data members) - it is a pure contract.
// Does: forces every subclass to implement pay().
// Must NOT: contain any real payment logic itself, and must NOT force unrelated behaviour
//           (e.g. refund()) onto every payment type - see Step H, Interface Segregation.
#pragma once
#include <string>

// OOP: ABSTRACTION - abstract class with a pure virtual method. Cannot be instantiated directly.
class Payment {
public:
    virtual bool pay(double amount) = 0;   // pure virtual - the contract every payment type must fulfil
    virtual std::string methodName() const = 0;
    virtual ~Payment() {}                  // virtual destructor - required whenever a class is used polymorphically
};
