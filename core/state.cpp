#include <iostream>
#include <cstring>

#include "sqlite/sqlite3.h"

#include "state.h"
#include "user.h"
#include "db.h"

State::State()
{
    // Open database connection
    char *filename = "data.sqlite";
    int rc = sqlite3_open(filename, &this->db);
    if (rc) {
        std::cerr << "Could not open " << filename << "\n";
        exit(1);
    }

    // Insert data if not exists
    create_tables(this->db);
    insert_admin(this->db);
    this->users = select_users(this->db);
}

State::~State()
{
    // Save data
    delete_users(this->db);
    insert_users(this->db, this->users);

    // Close database connection
    sqlite3_close(this->db);
}

bool State::user_register(User user)
{
    if (this->users.contains(user.email)) {
        return false;
    }

    this->users[user.email] = user;
    return true;
}

bool State::user_unregister(const User *user)
{
    this->users.erase(user->email);
}

bool State::user_login(std::string email, std::string password)
{
    this->logged_in = false;

    auto it = this->users.find(email);
    if (it == this->users.end()) {
        return false;
    }

    User *found = &it->second;
    if (found->password != password) {
        return false;
    }

    this->logged_in = true;
    this->user = found;

    // Clean previous buffers before logging in
    this->clear_tabs();

    return true;
}

void State::user_logout()
{
    this->logged_in = false;
}

bool State::user_change_email(std::string email)
{
    auto itnew = this->users.find(email);
    if (itnew != this->users.end()) {
        return false;
    }

    // Copy old bucket (*this->user) to new bucket (this->users[email])
    this->users[email] = *this->user;

    // Delete old bucket
    this->users.erase(this->user->email);

    // Set global user to new bucket
    this->user = &this->users[email];

    // Set global user email to new email
    this->user->email = email;

    return true;
}

void State::user_change_password(std::string password)
{
    this->user->password = password;
}

bool State::user_is_admin()
{
    return this->user->role == Admin;
}

void State::clear_tabs()
{
    // Plots tab
    this->plots_tab.show_plots = false;
    this->plots_tab.show_error = false;
    this->plots_tab.fit_next = false;
    strcpy(this->plots_tab.buf_path, "csv_samples/acceleration.csv");
    memset(this->plots_tab.err_buf_path, 0, BUF_SIZE);
    this->plots_tab.parser.clear();
    this->plots_tab.map.clear();

    // Profile tab
    memset(this->profile_tab.buf_email, 0, BUF_SIZE);
    memset(this->profile_tab.buf_password1, 0, BUF_SIZE);
    memset(this->profile_tab.buf_password2, 0, BUF_SIZE);
    this->profile_tab.message_email = 0;
    this->profile_tab.message_password = 0;

    // Settings tab
    memset(this->settings_tab.buf_email, 0, BUF_SIZE);
    memset(this->settings_tab.buf_password1, 0, BUF_SIZE);
    memset(this->settings_tab.buf_password2, 0, BUF_SIZE);
    this->settings_tab.role = Admin;
    this->settings_tab.message = 0;
}
