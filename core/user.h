#pragma once

#include <string>
#include <vector>

enum UserRole {
    Admin   = 0,
    Viewer  = 1,
};

const char *role_string(UserRole role);
const char **role_map(size_t *size);

struct User {
    std::string email;
    std::string password;
    UserRole role;

    User();
    User(std::string email, std::string password, UserRole role);
    bool eq(const User *user);
};
