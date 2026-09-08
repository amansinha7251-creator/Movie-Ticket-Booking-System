// 07_Customer.cpp
// ONE responsibility: hold a customer's own identity - name and phone number.
// Knows: name, phone number.
// Does: exposes that data through getters.
// Must NOT: know about bookings, shows or payments - BookingService links a Customer to a Booking,
//           the Customer class itself has no idea it has ever booked anything (ASSOCIATION, not ownership).
#pragma once
#include <string>

class Customer {
private:
    std::string name;
    std::string phone;

public:
    Customer(std::string name, std::string phone) : name(name), phone(phone) {}

    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
};
