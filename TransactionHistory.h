#pragma once
#include <vector>
#include "Transaction.h"

// ─────────────────────────────────────────────
// Node of a singly linked list
// ─────────────────────────────────────────────
class TransactionNode {
private:
    Transaction      data;
    TransactionNode* next;   // raw, non-owning link

public:
    explicit TransactionNode(const Transaction& t);

    // Accessors
    const Transaction& getData()    const;
    Transaction&       getDataRef();
    TransactionNode*   getNext()    const;

    // Mutator
    void setNext(TransactionNode* node);
};

// ─────────────────────────────────────────────
// Singly linked list of TransactionNodes
// ─────────────────────────────────────────────
class TransactionHistory {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int              size;

    // ── Merge Sort internals ──────────────────
    TransactionNode* mergeSort(TransactionNode* source);
    TransactionNode* getMiddle(TransactionNode* source);   // fast/slow pointer
    TransactionNode* merge(TransactionNode* left,
                           TransactionNode* right);

    // Compare two timestamps chronologically
    bool isEarlier(const Timestamp& a, const Timestamp& b) const;

public:
    TransactionHistory();
    ~TransactionHistory();   // frees all nodes

    // Insert at tail — O(1)
    TransactionNode* addTransaction(const Transaction& transaction);

    // Walk head→tail and print each transaction
    void traverseAndDisplay() const;

    // In-place merge sort by timestamp — O(n log n)
    void sortHistoryByYear();

    // Copy all records into a vector for cache-friendly reporting
    std::vector<Transaction> exportToVector() const;
};
