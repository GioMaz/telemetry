#include <iostream>

#include "imgui/imgui.h"
#include "implot/implot.h"

#include "../common.h"
#include "../state.h"

#include "profile.h"

void render_new_email(State *state)
{
    int &message = state->profile_tab.message_email;

    char *buf_email = state->profile_tab.buf_email;

    ImGui::SeparatorText("NEW EMAIL");
    ImGui::InputText("new email", buf_email, BUF_SIZE);
    if (ImGui::Button("Apply##email")) {
        if (buf_email[0]) {
            if (state->user_change_email(std::string(buf_email))) {
                memset(buf_email, 0, BUF_SIZE);
                message = 1;
            } else {
                message = 2;
            }
        } else {
            message = 3;
        }
    }

    switch (message) {
    case 0: break;
    case 1: ImGui::Text("Email updated correctly"); break;
    case 2: ImGui::Text("Email already taken"); break;
    case 3: ImGui::Text("Email cannot be empty"); break;
    }
}

void render_new_password(State *state)
{
    int &message = state->profile_tab.message_password;

    ImGui::SeparatorText("NEW PASSWORD");

    char *buf_password1 = state->profile_tab.buf_password1;
    char *buf_password2 = state->profile_tab.buf_password2;

    ImGui::InputText("new password", buf_password1, BUF_SIZE, ImGuiInputTextFlags_Password);
    ImGui::InputText("repeat new password", buf_password2, BUF_SIZE, ImGuiInputTextFlags_Password);
    if (ImGui::Button("Apply##password")) {
        if (!buf_password1[0] && !buf_password2[0]) {
            message = 1;
        } else if (strcmp(buf_password1, buf_password2) == 0) {
            // Set new password
            state->user_change_password(std::string(buf_password1));

            // Clear buffers
            memset(buf_password1, '\0', BUF_SIZE);
            memset(buf_password2, '\0', BUF_SIZE);

            message = 2;
        } else {
            message = 3;
        }
    }

    switch (message) {
    case 0: break;
    case 1: ImGui::Text("Password cannot be empty"); break;
    case 2: ImGui::Text("Password updated correctly"); break;
    case 3: ImGui::Text("Passwords must correspond"); break;
    }
}

void render_profile(State *state)
{
    if (ImGui::BeginTabItem("Profile")) {
        render_new_email(state);
        render_new_password(state);
        ImGui::EndTabItem();
    }
}
