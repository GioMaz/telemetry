#pragma once

#include <string>
#include <vector>

// +------------+
// | User roles |
// +------------+

enum UserRole {
    Admin   = 0,
    Viewer  = 1,
};

#define ROLES_BEGIN 0
#define ROLES_END   2

#define FOREACH_ROLE(block)                                     \
    for (int role = ROLES_BEGIN; role < ROLES_END; role++) {    \
        block                                                   \
    }

const char *role_string(UserRole role);
const char **role_map(size_t *size);

// +-------+
// | Users |
// +-------+

struct User {
    std::string email;
    std::string password;
    UserRole role;

    User();
    User(std::string email, std::string password, UserRole role);
    bool eq(const User *user);
};
