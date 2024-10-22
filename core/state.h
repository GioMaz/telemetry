#pragma once

#include <map>

#include "user.h"
#include "common.h"

#include "parser/parser.h"

enum Tab {
    None        = 0,
    App         = 1,
    Profile     = 2,
    Settings    = 3,
    Telemetry   = 4,
};

// Profile tab's state
struct ProfileTab {
    char buf_email[BUF_SIZE];
    char buf_password1[BUF_SIZE];
    char buf_password2[BUF_SIZE];
    int message_email = 0;
    int message_password = 0;
};

// Settings tab's state
struct SettingsTab {
    char buf_email[BUF_SIZE];
    char buf_password1[BUF_SIZE];
    char buf_password2[BUF_SIZE];
    UserRole role = Admin;
    int message = 0;
};

// Telemetry tab's state
struct TelemetryTab {
    bool show_plots = false;
    bool show_error = false;
    char buf_path[BUF_SIZE];
    char err_buf_path[BUF_SIZE];
    Parser parser;
    std::map<std::string, std::vector<double>> map;
};

struct State {
    // Registered users
    std::unordered_map<std::string, User> users;

    // Logged user
    User *user      = NULL;
    bool logged_in  = false;

    // Tabs state
    ProfileTab   profile_tab;
    SettingsTab  settings_tab;
    TelemetryTab telemetry_tab;

    // Settings
    bool show_path      = true;
    bool show_throttle  = true;
    bool show_position  = true;
    bool show_distance  = true;

    State();
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
