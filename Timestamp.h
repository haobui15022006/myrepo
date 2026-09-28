#pragma once
#include <iostream>
#include <iomanip>

// Represents the exact time of a transaction
struct Timestamp {
    int day;
    int month;
    int year;
    int hour;
    int minute;

    Timestamp(int d, int mo, int y, int h, int mi)
        : day(d), month(mo), year(y), hour(h), minute(mi) {}

    // Display format: DD/MM/YYYY HH:MM
    void display() const {
        std::cout << std::setfill('0')
                  << std::setw(2) << day   << "/"
                  << std::setw(2) << month << "/"
                  << std::setw(4) << year  << " "
                  << std::setw(2) << hour  << ":"
                  << std::setw(2) << minute;
    }
};

// Enum class avoids string-based type errors
enum class TransactionType {
    DEPOSIT,
    WITHDRAW
};
