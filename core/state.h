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
    std::unordered_map<std::string, User> users;
    User *user      = NULL;
    bool logged_in  = false;

    Tab previous_tab = None;

    State();
    void populate_users();
    bool user_register(User user);
    bool user_unregister(const User *user);
    bool user_login(std::string email, std::string password);
    void user_logout();
    bool user_change_email(std::string email);
    void user_change_password(std::string password);
};
