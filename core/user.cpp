#include <iostream>
#include <optional>

#include "user.h"
#include "state.h"

// Global variable does not belong to State object
// since is not mutable
static const char *map[] = { "Admin", "Viewer", };

const char *role_string(UserRole role)
{
    return map[role];
}

const char **role_map(size_t *size)
{
    *size = sizeof(map)/sizeof(map[0]);
    return map;
}

User::User() { }

User::User(std::string email, std::string password, UserRole role)
{
    this->email     = email;
    this->password  = password;
    this->role      = role;
}

bool User::eq(const User *user)
{
    return this->email == user->email;
}
