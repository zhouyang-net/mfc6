#pragma once
#include <string>

struct Record
{
    int id = 0;
    int userId = 0;
    bool isIncome = true;
    std::wstring category;
    double amount = 0.0;
    std::wstring date; // YYYY-MM-DD
    std::wstring notes;
};