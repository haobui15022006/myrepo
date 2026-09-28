#pragma once
#include <string>
#include <vector>
#include <memory>
#include "TransactionHistory.h"

class Customer {
private:
    std::string        customerId;
    std::string        fullName;
    double             balance;
    TransactionHistory history;   // composition — Customer owns its history

public:
    Customer(const std::string& id,
             const std::string& name,
             double             initialBalance);

    // Getters
    std::string getCustomerId() const;
    std::string getFullName()   const;
    double      getBalance()    const;

    // Mutable access so CustomerManager can call sortHistoryByYear, etc.
    TransactionHistory& getHistory();

    // ── Transaction processing ──────────────────────────────────────────────

    // Increase balance, record DEPOSIT, return new node pointer (never nullptr)
    TransactionNode* executeDeposit(double           amount,
                                    const Timestamp& ts,
                                    const std::string& txId);

    // Deduct balance, record WITHDRAW; returns nullptr if insufficient funds
    TransactionNode* executeWithdraw(double           amount,
                                     const Timestamp& ts,
                                     const std::string& txId);

    void displayProfile() const;
    
};
