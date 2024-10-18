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
    auto it = this->users.find(email);
    if (it != this->users.end()) {
        return false;
    }

    this->user->email = email;
    return true;
}

void State::user_change_password(std::string password)
{
    this->user->password = password;
}
