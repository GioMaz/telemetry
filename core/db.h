#pragma once

#include <unordered_map>

#define SQLITE3_EXEC(db, query) \
    char *msg = NULL;                                   \
    int rc = sqlite3_exec(db, query, NULL, NULL, &msg); \
    if (rc != SQLITE_OK) {                              \
        std::cerr << "SQLite error: " << msg << "\n";   \
        sqlite3_free(msg);                              \
        return false;                                   \
    }                                                   \
    return true;


bool create_tables(sqlite3 *db);
std::unordered_map<std::string, User> select_users(sqlite3 *db);
bool insert_admin(sqlite3 *db);
bool delete_users(sqlite3 *db);
bool insert_users(sqlite3 *db, std::unordered_map<std::string, User> users);
