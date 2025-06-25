#include <iostream>
#include <math.h>

#include "imgui/imgui.h"
#include "implot/implot.h"

#include "../parser/parser.h"
#include "../common.h"
#include "../state.h"
#include "../user.h"

static int binary_search_aux(double *a, int i, int j, double val)
{
    if (i == j) {
        return i;
    } else {
        int k = (i+j)/2;
        if (val < a[k]) {
            return binary_search_aux(a, i, k, val);
        } else {
            return binary_search_aux(a, k+1, j, val);
        }
    }
}

static int binary_search(std::vector<double> &a, double val)
{
    return binary_search_aux(a.data(), 0, a.size()-1, val);
}

static void render_line_tooltip(
        std::map<std::string, std::vector<double>> &lines,
        std::vector<double> &time)
{
    // Draw line
    double x  = ImPlot::GetPlotMousePos().x;
    int xi = binary_search(time, x);

    double xn = time[xi];
    double xp = ImPlot::PlotToPixels(xn, 0).x;
    double t  = ImPlot::GetPlotPos().y;
    double b  = t + ImPlot::GetPlotSize().y;
    auto color = IM_COL32(128,128,128,255);
    ImPlot::GetPlotDrawList()->AddLine(ImVec2(xp, b), ImVec2(xp, t), color);

    // Draw tooltip
    if (ImGui::BeginItemTooltip()) {
        for (auto &line : lines) {
            ImGui::Text("%s: %lf", line.first.data(), line.second[xi]);
        }
        ImGui::EndTooltip();
    }
}

static bool render_path(
        std::map<std::string, std::vector<double>> &lines,
        bool fit_next)
{
    auto itx = lines.find("x");
    auto ity = lines.find("y");
    if (itx == lines.end() || ity == lines.end())
        return false;

    auto &xs = itx->second;
    auto &ys = ity->second;

    if (xs.size() != ys.size())
        return false;

    if (fit_next)
        ImPlot::SetNextAxesToFit();

    if (ImPlot::BeginPlot("Path", ImVec2(-1, 0), ImPlotFlags_Equal)) {
        const static auto color = IM_COL32(66, 135, 245, 255);
        ImPlot::SetupAxes("X", "Y");
        ImPlot::PushStyleColor(ImPlotCol_Line, color);
        ImPlot::PlotLine("Path",
                xs.data(),
                ys.data(),
                ys.size());

        if (ImPlot::IsPlotHovered()) {

            // Calculate cursor's closest point
            auto point  = ImPlot::GetPlotMousePos();
            double x    = point.x;
            double y    = point.y;
            int mini    = -1;
            double dmin = DBL_MAX;
            for (int i = 0; i < xs.size(); i++) {
                double dx = x - xs[i];
                double dy = y - ys[i];
                double d = sqrtf((dx*dx) + (dy*dy));
                if (d < dmin) {
                    dmin = d;
                    mini = i;
                }
            }

            // Draw circle
            auto center = ImPlot::PlotToPixels(ImPlotPoint(xs[mini], ys[mini]));
            ImPlot::GetPlotDrawList()->AddCircleFilled(center, 5, color, 20);

            // Draw tooltip
            if (mini > 0 && ImGui::BeginItemTooltip())
            {
                for (auto &line : lines) {
                    ImGui::Text("%s: %lf", line.first.data(), line.second[mini]);
                }
                ImGui::EndTooltip();
            }
        }

        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }

    return true;
}

static bool render_throttle(
        std::map<std::string, std::vector<double>> &lines,
        bool fit_next)
{
    auto ittime = lines.find("time");
    if (ittime == lines.end())
        return false;
    auto itspeed = lines.find("speed");
    if (itspeed == lines.end())
        return false;
    auto itthrottle = lines.find("throttle");
    if (itthrottle == lines.end())
        return false;
    auto itbrake = lines.find("brake");
    if (itbrake == lines.end())
        return false;

    auto &time      = ittime->second;
    auto &speed     = itspeed->second;
    auto &throttle  = itthrottle->second;
    auto &brake     = itbrake->second;

    if (fit_next)
        ImPlot::SetNextAxesToFit();

    if (ImPlot::BeginPlot("Throttle & brake")) {
        ImPlot::SetupAxes("Time", "Speed");
        ImPlot::SetupAxis(ImAxis_Y2, "Throttle", ImPlotAxisFlags_AuxDefault);
        ImPlot::SetupAxis(ImAxis_Y3, "Brake", ImPlotAxisFlags_AuxDefault);

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
        ImPlot::PlotLine("Speed",
                time.data(),
                speed.data(),
                speed.size());

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
        ImPlot::PlotLine("Throttle",
                time.data(),
                throttle.data(),
                throttle.size());

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y3);
        ImPlot::PlotLine("Brake",
                time.data(),
                brake.data(),
                brake.size());

        if (ImPlot::IsPlotHovered()) {
            render_line_tooltip(lines, time);
        }

        ImPlot::EndPlot();
    }

    return true;
}

static bool render_distance(
        std::map<std::string, std::vector<double>> &lines,
        bool fit_next)
{
    auto itt = lines.find("time");
    auto its = lines.find("speed");
    if (itt == lines.end() || its == lines.end())
        return false;

    auto &time = itt->second;
    auto &speed = its->second;

    std::vector<double> space(speed.size() - 1);
    for (int i = 1; i < space.size(); i++) {
        space[i] = space[i-1] + speed[i];
    }

    if (fit_next)
        ImPlot::SetNextAxesToFit();

    if (ImPlot::BeginPlot("Distance")) {
        ImPlot::SetupAxes("Time", "Distance");
        ImPlot::SetupAxis(ImAxis_Y2, "Speed", ImPlotAxisFlags_AuxDefault);

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
        ImPlot::PlotLine("Distance",
                time.data(),
                space.data(),
                space.size());

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
        ImPlot::PlotLine("Speed",
                time.data(),
                speed.data(),
                speed.size());

        if (ImPlot::IsPlotHovered()) {
            render_line_tooltip(lines, time);
        }

        ImPlot::EndPlot();
    }

    return true;
}

static bool render_position(
        std::map<std::string, std::vector<double>> &lines,
        bool fit_next)
{
    auto itt = lines.find("time");
    if (itt == lines.end())
        return false;
    auto itx = lines.find("x");
    if (itx == lines.end())
        return false;
    auto ity = lines.find("y");
    if (ity == lines.end())
        return false;

    auto &ts = itt->second;
    auto &xs = itx->second;
    auto &ys = ity->second;

    // TODO: this does not work since it isn't displayed
    if (fit_next)
        ImPlot::SetNextAxesToFit();

    if (ImPlot::BeginPlot("Position")) {
        ImPlot::SetupAxes("Time", "Space");

        ImPlot::PlotLine("X",
                ts.data(),
                xs.data(),
                xs.size());

        ImPlot::PlotLine("Y",
                ts.data(),
                ys.data(),
                ys.size());

        if (ImPlot::IsPlotHovered()) {
            render_line_tooltip(lines, ts);
        }

        ImPlot::EndPlot();
    }

    return true;
}

void render_plots(State *state)
{
    bool &show_plots    = state->plots_tab.show_plots;
    bool &show_error    = state->plots_tab.show_error;
    bool &fit_next      = state->plots_tab.fit_next;
    char *buf_path      = state->plots_tab.buf_path;
    char *err_buf_path  = state->plots_tab.err_buf_path;
    Parser &parser      = state->plots_tab.parser;
    auto &map           = state->plots_tab.map;

    if (ImGui::BeginTabItem("Plots")) {

        ImGui::InputText("filepath", buf_path, BUF_SIZE);

        if (ImGui::Button("Load CSV")) {
            // Open file
            if (parser.open(buf_path)) {
                // Read file content
                parser.read_all();
                map = parser.to_mapd();

                show_plots = true;
                show_error = false;
                fit_next   = true;
            } else {
                strcpy(err_buf_path, buf_path);
                show_plots = false;
                show_error = true;
            }
        }

        if (show_error) {

            // Show error message
            ImGui::Text("Could not open %s\n", err_buf_path);

        } else if (show_plots) {

            // Show veichle path
            if (state->show_path) {
                if (!render_path(map, fit_next)) {
                    ImGui::Text("Could not show path plot");
                }
            }

            // Show all the csv values based on time
            if (state->show_throttle) {
                if (!render_throttle(map, fit_next)) {
                    ImGui::Text("Could not show throttle plot");
                }
            }

            // Show distance travelled
            if (state->show_distance) {
                if (!render_distance(map, fit_next)) {
                    ImGui::Text("Could not show distance plot");
                }
            }

            // Show veichle position
            if (state->show_position) {
                if (!render_position(map, fit_next)) {
                    ImGui::Text("Could not show position plot");
                }
            }

        }

        ImGui::EndTabItem();
    }

    if (fit_next)
        fit_next = false;
}
