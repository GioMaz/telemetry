#include <iostream>

#include "sqlite/sqlite3.h"

#include "common.h"
#include "user.h"
#include "db.h"

bool create_tables(sqlite3 *db)
{
    static char *query =
        "CREATE TABLE IF NOT EXISTS Users("
        "email VARCHAR(256) PRIMARY KEY,"
        "password VARCHAR(256),"
        "role INT);";

    SQLITE3_EXEC(db, query);
}

bool insert_admin(sqlite3 *db)
{
    char query[BUF_SIZE] = {0};
    sprintf(query, "INSERT INTO Users(email, password, role)"
                   "VALUES (\"admin\", \"admin\", %d)", Admin);

    SQLITE3_EXEC(db, query);
}

static int to_users(void *vusers, int argc, char **argv, char **columns)
{
    // Don't take ownership of the map
    auto *users = (std::unordered_map<std::string, User> *) vusers;

    auto email    = std::string(argv[0]);
    auto password = std::string(argv[1]);
    int  role     = atoi(argv[2]);
    (*users)[email]  = User(email, password, (UserRole)role);

    return 0;
}

std::unordered_map<std::string, User> select_users(sqlite3 *db)
{
    char *query =
        "SELECT email, password, role\n"
        "FROM Users;";

    char *msg = NULL;
    std::unordered_map<std::string, User> users;
    int rc = sqlite3_exec(db, query, to_users, &users, &msg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << msg << "\n";
        sqlite3_free(msg);
    }

    return users;
}

bool delete_users(sqlite3 *db)
{
    char *query = "DELETE FROM Users;";

    SQLITE3_EXEC(db, query);
}

bool insert_user(sqlite3 *db, User &user)
{
    char query[BUF_SIZE] = {0};
    sprintf(query, "INSERT INTO Users (email, password, role)\n"
                   "VALUES (\"%s\", \"%s\", %d);",
            user.email.data(),
            user.password.data(),
            (int)user.role);

    SQLITE3_EXEC(db, query);
}

bool insert_users(sqlite3 *db, std::unordered_map<std::string, User> users)
{
    bool result = true;
    for (auto &pair : users) {
        result = result && insert_user(db, pair.second);
    }
    return result;
}
