#include "CustomerManager.h"
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>


// Default constructor
CustomerManager::CustomerManager() {
    // Initialize maps or any setup if needed
    customersById.clear();
    accountsByUsername.clear();

    // Khởi tạo sẵn Customer để khớp với Account.txt
    auto custPtr = std::make_unique<Customer>("12345", "Bui Anh Hao", 1000.0);
    Customer* rawCust = custPtr.get();
    customersById["12345"] = std::move(custPtr);
     // Tạo Account khớp với Account.txt
    accountsByUsername["Buianhhao"] = std::make_unique<Account>("Buianhhao", "15022006", rawCust);
}

// ── registerNewCustomer ───────────────────────────────────────────────────────
bool CustomerManager::registerNewCustomer(const std::string& customerId,
        const std::string& fullName,
        const std::string& username,
        const std::string& password,
        double             initialBalance) {
    // 1. Validate uniqueness of both customerId and username
    if (customerIndex.count(customerId) || accountIndex.count(username)) {
        std::cerr << "[Register] Duplicate customerId or username.\n";
        return false;
    }

    // 2. Allocate Customer on the heap via unique_ptr
    auto owningPtr = std::make_unique<Customer>(customerId, fullName, initialBalance);

    // 3. Grab a raw (non-owning) pointer before we move ownership
    Customer* rawPtr = owningPtr.get();

    // 4. Insert into customerIndex (non-owning raw pointer)
    customerIndex[customerId] = rawPtr;

    // 5 & 6. Create Account holding raw pointer; insert into accountIndex
    accountIndex.emplace(username, Account(username, password, rawPtr));

    // 7. Transfer ownership into the customers vector
    customers.push_back(std::move(owningPtr));

    return true;
}

// ── login — avg O(1) ─────────────────────────────────────────────────────────
Customer* CustomerManager::login(const std::string& username, const std::string& password) {
    Account* acc = findAccountByUsername(username);
    if (acc && acc->verifyPassword(password)) {
        if (acc->getCustomerRef() != nullptr) {
            return acc->getCustomerRef();
        }
    }
    return nullptr;
}

// ── findTransactionGlobal — avg O(1) ─────────────────────────────────────────
TransactionNode* CustomerManager::findTransactionGlobal(const std::string& txId) {
    auto it = globalTransactionIndex.find(txId);
    return (it != globalTransactionIndex.end()) ? it->second : nullptr;
}

// ── processTransaction — avg O(1) ────────────────────────────────────────────
bool CustomerManager::processTransaction(Customer*          customerRef,
        TransactionType    type,
        double             amount,
        const Timestamp&   timestamp,
        const std::string& txId) {
    // 1. Reject duplicate transactionId
    if (globalTransactionIndex.count(txId)) {
        std::cerr << "[Transaction] Duplicate txId: " << txId << "\n";
        return false;
    }

    // 2. Dispatch to the appropriate Customer method
    TransactionNode* node = nullptr;
    if (type == TransactionType::DEPOSIT) {
        node = customerRef->executeDeposit(amount, timestamp, txId);
    } else {
        node = customerRef->executeWithdraw(amount, timestamp, txId);
    }

    // 4. nullptr → operation failed (e.g. insufficient funds)
    if (!node) {
        std::cerr << "[Transaction] Failed for txId: " << txId
                  << " (insufficient funds or invalid amount)\n";
        return false;
    }

    // 5. Register the node pointer in the global index
    globalTransactionIndex[txId] = node;
    return true;
}

// ── runStressTestSimulation ───────────────────────────────────────────────────
void CustomerManager::runStressTestSimulation(int n) {
    std::cout << "\n[Stress Test] Generating " << n << " transactions...\n";

    const std::string stressId   = "STRESS_CUST";
    const std::string stressUser = "stress_user";

    if (!customerIndex.count(stressId)) {
        registerNewCustomer(stressId, "Stress Tester", stressUser,
                            "stress_pass", 1'000'000.0);
    }
    Customer* cust = customerIndex[stressId];

    std::mt19937 rng(42);
    std::uniform_int_distribution<int>   typeDist(0, 1);
    std::uniform_real_distribution<double> amtDist(1.0, 500.0);
    std::uniform_int_distribution<int>   yearDist(2000, 2025);
    std::uniform_int_distribution<int>   monDist(1, 12);
    std::uniform_int_distribution<int>   dayDist(1, 28);
    std::uniform_int_distribution<int>   hrDist(0, 23);
    std::uniform_int_distribution<int>   minDist(0, 59);

    auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "STRESS_TX_" << std::setw(8) << std::setfill('0') << i;

        TransactionType type = typeDist(rng) ? TransactionType::DEPOSIT
                                             : TransactionType::WITHDRAW;
        double  amount = amtDist(rng);
        Timestamp ts(dayDist(rng), monDist(rng), yearDist(rng),
                     hrDist(rng),  minDist(rng));

        processTransaction(cust, type, amount, ts, oss.str());
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    double insertMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::cout << "[Stress Test] Inserted " << n
              << " transactions in " << insertMs << " ms\n";

    auto t2 = std::chrono::high_resolution_clock::now();
    cust->getHistory().sortHistoryByYear();
    auto t3 = std::chrono::high_resolution_clock::now();
    double sortMs = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::cout << "[Stress Test] Merge sort on " << n
              << " nodes took " << sortMs << " ms\n";

    std::ostringstream lastKey;
    lastKey << "STRESS_TX_" << std::setw(8) << std::setfill('0') << (n - 1);

    auto t4 = std::chrono::high_resolution_clock::now();
    TransactionNode* found = findTransactionGlobal(lastKey.str());
    auto t5 = std::chrono::high_resolution_clock::now();
    double lookupUs = std::chrono::duration<double, std::micro>(t5 - t4).count();

    std::cout << "[Stress Test] Global lookup took " << lookupUs
              << " µs  (found=" << (found ? "yes" : "no") << ")\n";
}

// ── findAccount ──────────────────────────────────────────────────────────────
Account* CustomerManager::findAccount(const std::string& username) {
    auto it = accounts.find(username);
    if (it != accounts.end()) {
        return it->second.get(); // trả về con trỏ Account
    }
    return nullptr; // không tìm thấy
}

Customer* CustomerManager::findCustomerById(const std::string& customerId) {
    auto it = customerIndex.find(customerId);
    if (it != customerIndex.end()) {
        return it->second; // trả về con trỏ Customer
    }
    return nullptr; // không tìm thấy
}

void CustomerManager::addAccount(const std::string& username, const std::string& password, Customer* customer){
	accounts[username] = std::make_unique<Account>(username, password, customer);
}

Account* CustomerManager::findAccountByUsername(const std::string& usr) {
    auto it = accountsByUsername.find(usr);
    if (it != accountsByUsername.end()) {
        return it->second.get(); // trả về con trỏ Account
    }
    return nullptr;
}

// ── addAccount