#include "imgui/imgui.h"

#include "../common.h"
#include "../state.h"

static void role_combo(char *name, const UserRole *role)
{
    size_t roles_size;
    const char **roles = role_map(&roles_size);
    ImGui::Combo(name, (int *)role, roles, roles_size);
}

static void render_all_users(State *state)
{
    ImGui::SeparatorText("ALL USERS");

    std::vector<const User *> users;
    for (const auto &pair : state->users)
        users.push_back(&pair.second);

    static const ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
        | ImGuiTableFlags_Resizable
        | ImGuiTableFlags_BordersOuter
        | ImGuiTableFlags_BordersV
        | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("Users", 3, flags))
    {
        ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Role", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        char buf[BUF_SIZE];
        for (int row = 0; row < users.size(); row++)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", users[row]->email.data());

            bool disabled = state->user->eq(users[row]);
            if (disabled) ImGui::BeginDisabled();

            ImGui::TableSetColumnIndex(1);
            memset(buf, 0, BUF_SIZE);
            sprintf(buf, "##%d", row);
            role_combo(buf, &users[row]->role);

            ImGui::TableSetColumnIndex(2);
            memset(buf, 0, BUF_SIZE);
            sprintf(buf, "Remove##%d", row);
            if (ImGui::Button(buf)) {
                state->user_unregister(users[row]);
            }

            if (disabled) ImGui::EndDisabled();
        }

        ImGui::EndTable();
    }
}

void render_new_user(State *state)
{

    ImGui::SeparatorText("NEW USER");

    char *buf_email     = state->settings_tab.buf_email;
    char *buf_password1 = state->settings_tab.buf_password1;
    char *buf_password2 = state->settings_tab.buf_password2;
    UserRole &role      = state->settings_tab.role;
    int &message        = state->settings_tab.message;

    ImGui::InputText("email", buf_email, BUF_SIZE);
    ImGui::InputText("password", buf_password1, BUF_SIZE, ImGuiInputTextFlags_Password);
    ImGui::InputText("repeat password", buf_password2, BUF_SIZE, ImGuiInputTextFlags_Password);
    role_combo("Role", &role);

    if (ImGui::Button("Create")) {
        if (!buf_password1[0] && !buf_password2[1]) {
            message = 1;
        } else if (strcmp(buf_password1, buf_password2) == 0) {
            auto user = User(
                    std::string(buf_email),
                    std::string(buf_password1),
                    role);
            if (state->user_register(user)) {
                message = 2;
            } else {
                message = 3;
            }
        } else {
            message = 4;
        }
    }

    switch (message) {
    case 0: break;
    case 1: ImGui::Text("Passwords cannot be empty"); break;
    case 2: ImGui::Text("User created correctly"); break;
    case 3: ImGui::Text("Email already taken"); break;
    case 4: ImGui::Text("Passwords must correspond"); break;
    }
}

void render_show_plots(State *state)
{
    ImGui::SeparatorText("SHOW PLOTS");

    ImGui::Checkbox("Show path plot",     &state->show_path);
    ImGui::Checkbox("Show throttle plot", &state->show_throttle);
    ImGui::Checkbox("Show distance plot", &state->show_distance);
    ImGui::Checkbox("Show position plot", &state->show_position);
}

void render_settings(State *state)
{
    if (ImGui::BeginTabItem("Settings")) {
        render_all_users(state);
        render_new_user(state);
        render_show_plots(state);
        ImGui::EndTabItem();
    }
}

