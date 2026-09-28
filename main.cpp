#include <iostream>
#include <string>
#include <conio.h>   // dùng _getch() để che mật khẩu
#include "CustomerManager.h"
#include "AccountFileHelper.h"

using namespace std;

// Function to input password with masking
string inputPassword(bool show = false) {
    string pwd;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) break; // Enter
        if (ch == 8 && !pwd.empty()) { // Backspace
            pwd.pop_back();
            cout << "\b \b";
        } else if (ch == 'a') { // key a to go back
            return "BACK";
        } else if (ch == 'b') { // key b to show current password
            cout << "\n[Current password]: " << pwd << endl;
        } else {
            pwd.push_back(ch);
            if (!show) cout << '*';
            else cout << ch;
        }
    }
    cout << endl;
    return pwd;
}

// Login function
bool login(CustomerManager& manager) {
    string username;
    cout << "LOGIN (0 EXIT): ";
    cin >> username;

    if (username == "0") {
        cout << "EXIT PROGRAM...\n";
        exit(0);
    }

    // Check if username exists
    Account* acc = manager.findAccountByUsername(username);
    if (!acc) {
        cout << "Error: Username does not exist!\n";
        return false;
    }

    string password = inputPassword();
    if (acc->verifyPassword(password)) {
        if (acc->getCustomerRef() != nullptr) {
            cout << "Login success!\n";
            return true;
        } else {
            cout << "Error: Account has no valid customer reference!\n";
            return false;
        }
    } else {
        cout << "Incorrect password.\n";
        return false;
    }
}

// Main menu
void mainMenu() {
    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "(1) Transaction\n";
        cout << "(2) Sort transactions\n";
        cout << "(3) Search transactions\n";
        cout << "(0) Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int sub;
                do {
                    cout << "\n--- TRANSACTION ---\n";
                    cout << "(1) Withdraw\n";
                    cout << "(2) Transfer\n";
                    cout << "(0) Back to main menu\n";
                    cout << "Choice: ";
                    cin >> sub;
                    if(sub==1){
                    	double amt;
                    	cout << "Enter "
					}
                } while (sub != 0);
                break;
            }
            case 2: {
                int sub;
                do {
                    cout << "\n--- SORT TRANSACTIONS ---\n";
                    cout << "(1) Sort by time\n";
                    cout << "(2) Sort withdrawals\n";
                    cout << "(3) Sort deposits\n";
                    cout << "(0) Back to main menu\n";
                    cout << "Choice: ";
                    cin >> sub;
                } while (sub != 0);
                break;
            }
            case 3: {
                int sub;
                do {
                    cout << "\n--- SEARCH TRANSACTIONS ---\n";
                    cout << "(1) Search withdrawals\n";
                    cout << "(2) Search deposits\n";
                    cout << "(0) Back to main menu\n";
                    cout << "Choice: ";
                    cin >> sub;
                } while (sub != 0);
                break;
            }
            case 0:
                cout << "Logout successful.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    CustomerManager manager;
    AccountFileHelper fileHelper;

    // Load accounts from file
    vector<Account> loadedAccounts = fileHelper.loadAccounts();

    // Map accounts into CustomerManager safely
    for (auto& acc : loadedAccounts) {
        Customer* cust = nullptr;
        if (acc.getCustomerRef() != nullptr) {
            cust = manager.findCustomerById(acc.getCustomerRef()->getCustomerId());
        }

        if (cust) {
            manager.addAccount(acc.getUsername(), acc.getPassword(), cust);
        } else {
            cout << "Warning: Account " << acc.getUsername()
                 << " has no valid customer reference!" << endl;
        }
    }

    cout << "Accounts loaded from Account.txt\n";

    while (true) {
        if (login(manager)) {
            mainMenu();
        } else {
            cout << "Back to login screen.\n";
        }
    }
    return 0;
}
