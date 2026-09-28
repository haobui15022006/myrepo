#pragma once
#include <string>
#include "Customer.h"

// Authentication layer — stores credentials and a non-owning pointer to Customer
class Account {
private:
    std::string username;
    std::string password;
    Customer*   customerRef;   // raw, non-owning — Customer is owned by CustomerManager

public:
    Account(const std::string& usr, const std::string& pwd, Customer* customer);

    std::string getUsername() const;

    // Returns true only when pwd matches stored password
    bool verifyPassword(const std::string& pwd) const;

    // Returns raw non-owning pointer to the associated Customer
    Customer* getCustomerRef() const;
    std:: string getPassword() const;
};
