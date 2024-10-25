#pragma once

#include <map>

#include "sqlite/sqlite3.h"
#include "parser/parser.h"

#include "user.h"
#include "common.h"

// Plots tab's state
struct PlotsTab {
    bool show_plots = false;        // Show all plots
    bool show_error = false;        // Show error
    bool fit_next   = false;        // Fit new PlotLines after new data is imported
    char buf_path[BUF_SIZE];        // Csv file path buffer
    char err_buf_path[BUF_SIZE];    // Error message buffer
    Parser parser;                  // Parser object
    std::map<std::string, std::vector<double>> map; // (column name, column data) map
};

// Profile tab's state
struct ProfileTab {
    char buf_email[BUF_SIZE];       // User email buffer
    char buf_password1[BUF_SIZE];   // Password buffer
    char buf_password2[BUF_SIZE];   // Confirm password buffer
    int message_email = 0;          // Error message for email
    int message_password = 0;       // Error message for password
};

// Settings tab's state
struct SettingsTab {
    char buf_email[BUF_SIZE];       // New user email buffer
    char buf_password1[BUF_SIZE];   // New user password buffer
    char buf_password2[BUF_SIZE];   // New user confirm password buffer
    UserRole role = Admin;          // New user role
    int message = 0;                // Error message for password
};

struct State {
    // Registered users
    std::unordered_map<std::string, User> users;
    sqlite3 *db;

    // Logged user
    User *user      = NULL;
    bool logged_in  = false;

    // Tabs state
    PlotsTab    plots_tab;
    ProfileTab  profile_tab;
    SettingsTab settings_tab;

    // Settings
    bool show_path      = true; // Show path plot
    bool show_throttle  = true; // Show throttle & brake plot
    bool show_distance  = true; // Show distance plot
    bool show_position  = true; // Show position plot

    State();
    ~State();
    void populate_users();
    bool user_register(User user);
    bool user_unregister(const User *user);
    bool user_login(std::string email, std::string password);
    void user_logout();
    bool user_change_email(std::string email);
    void user_change_password(std::string password);
    bool user_is_admin();
    void clear_tabs();
};
