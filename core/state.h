#pragma once

#include <unordered_map>

#include "user.h"
#include "common.h"

enum Tab {
    None        = 0,
    App         = 1,
    Profile     = 2,
    Settings    = 3,
    Telemetry   = 4,
};

struct State {
    // Users
    std::unordered_map<std::string, User> users;
    User *user      = NULL;
    bool logged_in  = false;

    // Tabs
    Tab previous_tab = None;

    // Telemetry
    bool show_values    = true;
    bool show_path      = true;
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
};
