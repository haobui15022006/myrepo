#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Account.h"
#include "Customer.h"
#include "TransactionHistory.h"

// Central controller: owns all Customer objects and maintains three indexes
class CustomerManager {
private:
    // ── Ownership ───────────────────────────────────────────────────────────
    std::vector<std::unique_ptr<Customer>> customers;   // sole owner of Customer objects

    // ── Indexes ─────────────────────────────────────────────────────────────
    std::unordered_map<std::string, Account>          accountIndex;           // username → Account
    std::unordered_map<std::string, Customer*>        customerIndex;          // customerId → Customer*
    std::unordered_map<std::string, TransactionNode*> globalTransactionIndex; // txId → node*
    std::unordered_map<std::string, std::unique_ptr<Account>> accounts;
    // Store customers by ID
    std::unordered_map<std::string, std::unique_ptr<Customer>> customersById;

    // Store accounts by username
    std::unordered_map<std::string, std::unique_ptr<Account>> accountsByUsername;
    const std::string FILENAME = "Account.txt";

public:
	CustomerManager();

    Customer* findCustomerById(const std::string& id);

    Account* findAccountByUsername(const std::string& usr);

    void addAccount(const std::string& username,
                    const std::string& password,
                    Customer* customer);

    Customer* login(const std::string& username, const std::string& password);
    // Register a brand-new customer; returns false on duplicate id or username
    bool registerNewCustomer(const std::string& customerId,
                             const std::string& fullName,
                             const std::string& username,
                             const std::string& password,
                             double             initialBalance);


    // Direct hash-table lookup of any transaction across all customers — avg O(1)
    TransactionNode* findTransactionGlobal(const std::string& txId);

    // Dispatch a DEPOSIT or WITHDRAW; registers node in globalTransactionIndex — avg O(1)
    bool processTransaction(Customer*          customerRef,
                            TransactionType    type,
                            double             amount,
                            const Timestamp&   timestamp,
                            const std::string& txId);

    // Generate n random transactions to benchmark sorting and hash performance
    void runStressTestSimulation(int n);
    
    // Hàm tìm account theo username
    Account* findAccount(const std::string& username);
    
    void saveAccount(const std::vector<Account>& accounts);
    std::vector<Account> loadAccounts();
    
};
