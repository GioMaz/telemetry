#include <iostream>

#include "implot/implot.h"

#include "app.h"
#include "user.h"
#include "state.h"
#include "common.h"

#include "tabs/profile.h"
#include "tabs/settings.h"
#include "tabs/telemetry.h"

static void render_login(State *state)
{
    ImGui::SeparatorText("USER LOGIN");

    static char buf_email[BUF_SIZE];
    static char buf_password[BUF_SIZE];

    if (state->previous_tab != App) {
        state->previous_tab = App;
        memset(buf_email, 0, BUF_SIZE);
        memset(buf_password, 0, BUF_SIZE);
    }

    ImGui::InputText("email", buf_email, BUF_SIZE);
    ImGui::InputText("password", buf_password, BUF_SIZE, ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        state->user_login(std::string(buf_email), std::string(buf_password));

        /*// TODO: remove this*/
        state->logged_in = true;
        state->user = &state->users["a@gmail.com"];
    }
}

static void render_topbar(State *state)
{
    char buf[BUF_SIZE];

    memset(buf, '\0', BUF_SIZE);
    sprintf(buf, "User: %s", state->user->email.data());
    ImGui::Text(buf);
    ImGui::SameLine();

    memset(buf, '\0', BUF_SIZE);
    sprintf(buf, "(Role: %s)", role_string(state->user->role));
    ImGui::Text(buf);
    ImGui::SameLine();

    if (ImGui::Button("Logout")) {
        state->user_logout();
    }
}

static void render_admin_tabs(State *state)
{
    // Tab 0 (Telemetry)
    render_telemetry(state);

    // Tab 1 (Profile)
    render_profile(state);

    // Tab 2 (Settings)
    render_settings(state);
}

static void render_viewer_tabs(State *state)
{
    // Tab 0 (Telemetry)
    render_telemetry(state);

    // Tab 1 (Profile)
    render_profile(state);
}

static void render_tabs(State *state) {
    render_topbar(state);

    auto tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("TabBar", tab_bar_flags)) {
        switch (state->user->role) {
        case Admin:     render_admin_tabs(state);   break;
        case Viewer:    render_viewer_tabs(state);  break;
        default:
            std::cerr << "Unexpected user role\n";
        }

        ImGui::EndTabBar();
    }
}

void app_render(State *state)
{
    // DEMO
    /*ImGui::ShowDemoWindow();*/
    /*ImPlot::ShowDemoWindow();*/
    /*ImGui::Begin("Project 1");*/
    /*ImGui::Text("Project 1");*/
    /*ImGui::End();*/
    /*return;*/

    static int flags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("App", NULL, flags);
    auto viewport = ImGui::GetMainViewport();
    ImGui::SetWindowSize(viewport->Size);
    ImGui::SetWindowPos(viewport->Pos);

    if (state->logged_in)
        render_tabs(state);
    else
        render_login(state);

    ImGui::End();
}
