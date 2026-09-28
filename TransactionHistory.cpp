#include "TransactionHistory.h"
#include <iostream>

// ═══════════════════════════════════════════════
// TransactionNode
// ═══════════════════════════════════════════════

TransactionNode::TransactionNode(const Transaction& t)
    : data(t), next(nullptr) {}

const Transaction& TransactionNode::getData()    const { return data; }
Transaction&       TransactionNode::getDataRef()       { return data; }
TransactionNode*   TransactionNode::getNext()    const { return next; }
void               TransactionNode::setNext(TransactionNode* node) { next = node; }

// ═══════════════════════════════════════════════
// TransactionHistory
// ═══════════════════════════════════════════════

TransactionHistory::TransactionHistory()
    : head(nullptr), tail(nullptr), size(0) {}

TransactionHistory::~TransactionHistory() {
    // Traverse and free every node to avoid memory leaks
    TransactionNode* current = head;
    while (current) {
        TransactionNode* next = current->getNext();
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

// ── addTransaction — O(1) tail insertion ─────────────────────────────────────
TransactionNode* TransactionHistory::addTransaction(const Transaction& transaction) {
    TransactionNode* newNode = new TransactionNode(transaction);

    if (!tail) {
        // List is empty
        head = tail = newNode;
    } else {
        tail->setNext(newNode);
        tail = newNode;
    }
    ++size;
    return newNode;   // caller may store this raw pointer in a global index
}

// ── traverseAndDisplay ────────────────────────────────────────────────────────
void TransactionHistory::traverseAndDisplay() const {
    TransactionNode* current = head;
    while (current) {
        current->getData().display();
        current = current->getNext();
    }
}

// ── sortHistoryByYear — O(n log n) merge sort ────────────────────────────────
void TransactionHistory::sortHistoryByYear() {
    head = mergeSort(head);

    // Repair tail pointer after sort
    if (head) {
        tail = head;
        while (tail->getNext()) tail = tail->getNext();
    }
}

// ── isEarlier — chronological comparison ─────────────────────────────────────
bool TransactionHistory::isEarlier(const Timestamp& a, const Timestamp& b) const {
    if (a.year   != b.year)   return a.year   < b.year;
    if (a.month  != b.month)  return a.month  < b.month;
    if (a.day    != b.day)    return a.day    < b.day;
    if (a.hour   != b.hour)   return a.hour   < b.hour;
    return a.minute < b.minute;
}

// ── getMiddle — fast/slow pointer — O(n) ─────────────────────────────────────
TransactionNode* TransactionHistory::getMiddle(TransactionNode* source) {
    if (!source) return nullptr;

    TransactionNode* slow = source;
    TransactionNode* fast = source->getNext();   // one step ahead so slow lands at mid

    while (fast && fast->getNext()) {
        slow = slow->getNext();
        fast = fast->getNext()->getNext();
    }
    return slow;   // slow is the last node of the left half
}

// ── merge — iterative merge of two sorted half-lists (avoids stack overflow) ──
TransactionNode* TransactionHistory::merge(TransactionNode* left,
                                           TransactionNode* right) {
    // Dummy head simplifies edge cases
    TransactionNode dummy(Transaction("", TransactionType::DEPOSIT, 0.0,
                                      Timestamp(1,1,2000,0,0)));
    TransactionNode* tail = &dummy;

    while (left && right) {
        if (isEarlier(left->getData().getTimestamp(),
                      right->getData().getTimestamp())) {
            tail->setNext(left);
            left = left->getNext();
        } else {
            tail->setNext(right);
            right = right->getNext();
        }
        tail = tail->getNext();
    }
    // Attach the remaining non-empty list
    tail->setNext(left ? left : right);
    return dummy.getNext();
}

// ── mergeSort — recursive divide-and-conquer — O(n log n) ────────────────────
TransactionNode* TransactionHistory::mergeSort(TransactionNode* source) {
    if (!source || !source->getNext()) return source;  // base case: 0 or 1 node

    // Split into two halves
    TransactionNode* mid   = getMiddle(source);
    TransactionNode* right = mid->getNext();
    mid->setNext(nullptr);   // sever the list

    // Recursively sort each half
    TransactionNode* sortedLeft  = mergeSort(source);
    TransactionNode* sortedRight = mergeSort(right);

    return merge(sortedLeft, sortedRight);
}

// ── exportToVector — copy to contiguous memory for cache-friendly reporting ──
std::vector<Transaction> TransactionHistory::exportToVector() const {
    std::vector<Transaction> result;
    result.reserve(size);

    TransactionNode* current = head;
    while (current) {
        result.push_back(current->getData());
        current = current->getNext();
    }
    return result;
}
