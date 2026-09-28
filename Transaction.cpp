#include "Transaction.h"
#include <iostream>

Transaction::Transaction(const std::string& id,
                         TransactionType    t,
                         double             amt,
                         const Timestamp&   ts)
    : transactionId(id), type(t), amount(amt), timestamp(ts) {}

std::string Transaction::getTransactionId() const { return transactionId; }
TransactionType Transaction::getType()      const { return type; }
double Transaction::getAmount()             const { return amount; }
Timestamp Transaction::getTimestamp()       const { return timestamp; }

void Transaction::display() const {
    std::string typeStr = (type == TransactionType::DEPOSIT) ? "DEPOSIT" : "WITHDRAW";
    std::cout << "[" << transactionId << "] "
              << typeStr << " $" << amount << "  @ ";
    timestamp.display();
    std::cout << "\n";
}
