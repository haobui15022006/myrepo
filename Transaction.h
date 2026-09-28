#pragma once
#include <string>
#include "Timestamp.h"

// Immutable transaction record — all fields set at construction, no setters
class Transaction {
private:
    std::string      transactionId;
    TransactionType  type;
    double           amount;
    Timestamp        timestamp;

public:
    Transaction(const std::string& id,
                TransactionType    t,
                double             amt,
                const Timestamp&   ts);

    // Getters
    std::string     getTransactionId() const;
    TransactionType getType()          const;
    double          getAmount()        const;
    Timestamp       getTimestamp()     const;

    // Pretty-print one transaction line
    void display() const;
};
