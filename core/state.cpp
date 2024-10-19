#include <iostream>

#include "state.h"
#include "user.h"

State::State()
{
    populate_users();
}

void State::populate_users()
{
    static User users[] = {
        { "a@gmail.com", "a", Admin },
        { "v@gmail.com", "v", Viewer },
    };
    size_t size = sizeof(users)/sizeof(users[0]);
    for (int i = 0; i < size; i++) {
        user_register(users[i]);
    }
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
