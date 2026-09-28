#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Account.h"

class AccountFileHelper {
private:
    const std::string FILENAME = "Account.txt";

public:
    // Lưu danh sách account ra file
    void saveAccounts(const std::vector<Account>& accounts);

    // Đọc danh sách account từ file
    std::vector<Account> loadAccounts();
};
