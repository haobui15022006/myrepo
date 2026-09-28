#include "Customer.h"
#include <iostream>

Customer::Customer(const std::string& id,
                   const std::string& name,
                   double             initialBalance)
    : customerId(id), fullName(name), balance(initialBalance) {}

std::string Customer::getCustomerId() const { return customerId; }
std::string Customer::getFullName()   const { return fullName; }
double      Customer::getBalance()    const { return balance; }

TransactionHistory& Customer::getHistory() { return history; }

// ── executeDeposit ────────────────────────────────────────────────────────────
TransactionNode* Customer::executeDeposit(double             amount,
                                          const Timestamp&   ts,
                                          const std::string& txId) {
    balance += amount;                                      // 1. update balance
    Transaction tx(txId, TransactionType::DEPOSIT, amount, ts);  // 2. create record
    return history.addTransaction(tx);                     // 3 & 4. store + return node
}

// ── executeWithdraw ───────────────────────────────────────────────────────────
TransactionNode* Customer::executeWithdraw(double             amount,
                                           const Timestamp&   ts,
                                           const std::string& txId) {
    if (balance < amount) return nullptr;                  // 1-2. insufficient funds

    balance -= amount;                                     // 3. deduct
    Transaction tx(txId, TransactionType::WITHDRAW, amount, ts); // 4. create record
    return history.addTransaction(tx);                     // 5 & 6. store + return node
}

// ── displayProfile ────────────────────────────────────────────────────────────
void Customer::displayProfile() const {
    std::cout << "─────────────────────────────────────\n"
              << "Customer ID : " << customerId << "\n"
              << "Name        : " << fullName   << "\n"
              << "Balance     : $" << balance   << "\n"
              << "─────────────────────────────────────\n";
}
