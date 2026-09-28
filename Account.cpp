#include "Account.h"

Account::Account(const std::string& usr,
                 const std::string& pwd,
                 Customer*          customer)
    : username(usr), password(pwd), customerRef(customer) {}

std::string Account::getUsername() const { return username; }

bool Account::verifyPassword(const std::string& pwd) const {
    return password == pwd;
}

Customer* Account::getCustomerRef() const { return customerRef; }
std::string Account::getPassword() const {return password;}