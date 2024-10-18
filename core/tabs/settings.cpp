#include "imgui/imgui.h"

#include "../common.h"
#include "../state.h"

static void role_combo(char *combo_name, const UserRole *role)
{
    size_t roles_size;
    const char **roles = role_map(&roles_size);
    ImGui::Combo(combo_name, (int *)role, roles, roles_size);
}

void render_settings(State *state)
{
    if (ImGui::BeginTabItem("Settings")) {
        ImGui::SeparatorText("ALL USERS");

        std::vector<const User *> users;
        for (const auto &pair : state->users)
            users.push_back(&pair.second);

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
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

                ImGui::TableSetColumnIndex(1);
                memset(buf, 0, BUF_SIZE);
                sprintf(buf, "##%d", row);
                role_combo(buf, &users[row]->role);

                ImGui::TableSetColumnIndex(2);
                memset(buf, 0, BUF_SIZE);
                sprintf(buf, "Remove##%d", row);
                if (ImGui::Button(buf)) {
                    if (!state->user->eq(users[row])) {
                        state->user_unregister(users[row]);
                    }
                }
            }

            ImGui::EndTable();
        }

        ImGui::SeparatorText("NEW USER");

        static char buf_email[BUF_SIZE];
        static char buf_password[BUF_SIZE];
        static UserRole role;
        if (state->previous_tab != Settings) {
            state->previous_tab = Settings;
            memset(buf_email, 0, BUF_SIZE);
            memset(buf_password, 0, BUF_SIZE);
            role = Admin;
        }

        ImGui::InputText("email", buf_email, BUF_SIZE);
        ImGui::InputText("password", buf_password, BUF_SIZE, ImGuiInputTextFlags_Password);
        role_combo("Role", &role);

        if (ImGui::Button("Create")) {
            auto user = User(
                    std::string(buf_email),
                    std::string(buf_password),
                    role);
            state->user_register(user);
        }
        ImGui::EndTabItem();
    }
}

