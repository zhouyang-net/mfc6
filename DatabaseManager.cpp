#include "pch.h"
#include "DatabaseManager.h"
#include <afx.h>
#include <atlstr.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "sqlite3.h" // 需要把 sqlite3.h/.c 加入工程或链接 sqlite3.lib

DatabaseManager& DatabaseManager::Instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::DatabaseManager() = default;
DatabaseManager::~DatabaseManager()
{
    Close();
}

bool DatabaseManager::Initialize(const std::wstring& dbPath)
{
    Close();
    int rc = sqlite3_open16(dbPath.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        if (m_db) { sqlite3_close(m_db); m_db = nullptr; }
        return false;
    }

    // 创建表
    const char* createUsers =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL);";
    if (!ExecSQL(createUsers)) return false;

    const char* createRecords =
        "CREATE TABLE IF NOT EXISTS records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER DEFAULT 0, "
        "is_income INTEGER NOT NULL, "
        "category TEXT, "
        "amount REAL, "
        "date TEXT, "
        "notes TEXT);";
    if (!ExecSQL(createRecords)) return false;

    return true;
}

void DatabaseManager::Close()
{
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool DatabaseManager::ExecSQL(const std::string& sql)
{
    char* errmsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) {
            sqlite3_free(errmsg);
        }
        return false;
    }
    return true;
}

bool DatabaseManager::AddUser(const std::wstring& username, const std::wstring& password)
{
    if (!m_db) return false;
    std::wstring wsql = L"INSERT INTO users(username,password) VALUES (?,?);";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare16_v2(m_db, wsql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;
    sqlite3_bind_text16(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::DeleteUser(int userId)
{
    if (!m_db) return false;
    std::string sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, userId);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<std::tuple<int, std::wstring, std::wstring>> DatabaseManager::GetUsers()
{
    std::vector<std::tuple<int, std::wstring, std::wstring>> out;
    if (!m_db) return out;
    const char* sql = "SELECT id, username, password FROM users;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return out;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const void* txt1 = sqlite3_column_text16(stmt, 1);
        const void* txt2 = sqlite3_column_text16(stmt, 2);
        std::wstring name = txt1 ? (const wchar_t*)txt1 : L"";
        std::wstring pwd = txt2 ? (const wchar_t*)txt2 : L"";
        out.emplace_back(id, name, pwd);
    }
    sqlite3_finalize(stmt);
    return out;
}

bool DatabaseManager::AddRecord(const Record& rec)
{
    if (!m_db) return false;
    std::wstring sql = L"INSERT INTO records(user_id,is_income,category,amount,date,notes) VALUES(?,?,?,?,?,?);";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare16_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, rec.userId);
    sqlite3_bind_int(stmt, 2, rec.isIncome ? 1 : 0);
    sqlite3_bind_text16(stmt, 3, rec.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, rec.amount);
    sqlite3_bind_text16(stmt, 5, rec.date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt, 6, rec.notes.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::UpdateRecord(const Record& rec)
{
    if (!m_db) return false;
    std::wstring sql = L"UPDATE records SET category=?, amount=?, date=?, notes=?, is_income=? WHERE id=?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare16_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;
    sqlite3_bind_text16(stmt, 1, rec.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, rec.amount);
    sqlite3_bind_text16(stmt, 3, rec.date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt, 4, rec.notes.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, rec.isIncome ? 1 : 0);
    sqlite3_bind_int(stmt, 6, rec.id);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::DeleteRecord(int recordId)
{
    if (!m_db) return false;
    std::string sql = "DELETE FROM records WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, recordId);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<Record> DatabaseManager::GetRecords(bool isIncome)
{
    std::vector<Record> out;
    if (!m_db) return out;
    std::string sql = "SELECT id, user_id, is_income, category, amount, date, notes FROM records WHERE is_income = ? ORDER BY date DESC;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return out;
    sqlite3_bind_int(stmt, 1, isIncome ? 1 : 0);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Record r;
        r.id = sqlite3_column_int(stmt, 0);
        r.userId = sqlite3_column_int(stmt, 1);
        r.isIncome = sqlite3_column_int(stmt, 2) != 0;
        const void* cat = sqlite3_column_text16(stmt, 3);
        r.category = cat ? (const wchar_t*)cat : L"";
        r.amount = sqlite3_column_double(stmt, 4);
        const void* date = sqlite3_column_text16(stmt, 5);
        r.date = date ? (const wchar_t*)date : L"";
        const void* notes = sqlite3_column_text16(stmt, 6);
        r.notes = notes ? (const wchar_t*)notes : L"";
        out.push_back(r);
    }
    sqlite3_finalize(stmt);
    return out;
}