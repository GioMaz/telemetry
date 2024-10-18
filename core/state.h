#pragma once

#include <unordered_map>

#include "user.h"
#include "common.h"

enum Tab {
    None,
    App,
    Profile,
    Settings,
    Telemetry,
};

struct State {
    // Global
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

    /*// Settings tab*/
    /*char settings_buf_email[BUF_SIZE];*/
    /*char settings_buf_password[BUF_SIZE];*/
    /*UserRole settings_role;*/
    /**/
    /*// Profile tab*/
    /*char profile_buf_email[BUF_SIZE];*/
    /*char profile_buf_password1[BUF_SIZE];*/
    /*char profile_buf_password2[BUF_SIZE];*/
};
