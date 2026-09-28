#include "AccountFileHelper.h"
#include <iostream>

void AccountFileHelper::saveAccounts(const std::vector<Account>& accounts) {
    std::ofstream ofs(FILENAME, std::ios::out);
    if (!ofs) {
        throw std::runtime_error("Không thể mở file Account.txt để ghi!");
    }

    for (const auto& acc : accounts) {
        ofs << acc.getUsername() << ";"
            << acc.getPassword() << ";"
            << acc.getCustomerRef()->getCustomerId() << "\n";
    }

    ofs.close();
    std::cout << "[INFO] Đã lưu dữ liệu account vào Account.txt\n";
}

std::vector<Account> AccountFileHelper::loadAccounts() {
    std::vector<Account> accounts;
    std::ifstream ifs(FILENAME, std::ios::in);
    if (!ifs) {
        std::cerr << "[WARNING] Không tìm thấy file Account.txt, sẽ tạo mới.\n";
        return accounts;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string username, password, customerId;

        if (std::getline(ss, username, ';') &&
            std::getline(ss, password, ';') &&
            std::getline(ss, customerId, ';')) {
            // Lưu ý: ở đây cần Customer* để tạo Account
            // Tạm thời ta chỉ lưu username/password, còn Customer* sẽ được map lại trong CustomerManager
            accounts.emplace_back(username, password, nullptr);
        }
    }

    ifs.close();
    std::cout << "[INFO] Đã load dữ liệu account từ Account.txt\n";
    return accounts;
}
