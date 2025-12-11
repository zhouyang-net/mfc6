#pragma once
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include "Record.h"

// 前向声明 sqlite3
struct sqlite3;

class DatabaseManager
{
public:
    static DatabaseManager& Instance();

    bool Initialize(const std::wstring& dbPath = L"finance.db");
    void Close();

    // 用户
    bool AddUser(const std::wstring& username, const std::wstring& password);
    bool DeleteUser(int userId);
    // 返回 (id, username, password)
    std::vector<std::tuple<int, std::wstring, std::wstring>> GetUsers();

    // 记录（收入/支出）
    bool AddRecord(const Record& rec);
    bool UpdateRecord(const Record& rec);
    bool DeleteRecord(int recordId);
    std::vector<Record> GetRecords(bool isIncome = true); // 按类型获取

private:
    DatabaseManager();
    ~DatabaseManager();

    bool ExecSQL(const std::string& sql);
    sqlite3* m_db = nullptr;
};