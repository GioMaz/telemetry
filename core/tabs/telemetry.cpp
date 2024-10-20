#include <iostream>

#include "imgui/imgui.h"
#include "implot/implot.h"

#include "../parser/parser.h"
#include "../common.h"
#include "../state.h"
#include "../user.h"

static int binary_search(double *a, int i, int j, double val)
{
    if (i == j) {
        return i;
    } else {
        int k = (i+j)/2;
        if (val < a[k]) {
            return binary_search(a, i, k, val);
        } else {
            return binary_search(a, k+1, j, val);
        }
    }
}

static bool render_values(
        std::map<std::string, std::vector<double>> &lines)
{
    auto it = lines.find("time");
    if (it == lines.end())
        return false;
    auto &time = it->second;

    if (ImPlot::BeginPlot("Values")) {
        ImPlot::SetupAxes("Time", "Values");

        for (auto &line : lines) {
            if (line.first != "time") {
                ImPlot::PlotLine(line.first.data(),
                        time.data(),
                        line.second.data(),
                        line.second.size());
            }
        }

        if (ImPlot::IsPlotHovered()) {

            // Draw line
            auto point  = ImPlot::GetPlotMousePos();
            double x    = point.x;
            double y    = point.y;
            double half_width = (time[1] - time[0]) / 2;
            double l = ImPlot::PlotToPixels(x - half_width * 1.5, 0).x;
            double r = ImPlot::PlotToPixels(x + half_width * 1.5, 0).x;
            double t = ImPlot::GetPlotPos().y;
            double b = t + ImPlot::GetPlotSize().y;
            auto color    = IM_COL32(128,128,128,128);
            ImPlot::GetPlotDrawList()->AddLine(ImVec2(l, t), ImVec2(r, b), color);

            // Draw tooltip
            if (x >= 0 && x < time.size() && ImGui::BeginItemTooltip()) {
                int xi = binary_search(time.data(), 0, time.size(), x);
                for (auto &line : lines) {
                    ImGui::Text("%s: %lf", line.first.data(), line.second[xi]);
                }
                ImGui::EndTooltip();
            }
        }

        ImPlot::EndPlot();
    }
}

static bool render_path(
        std::map<std::string, std::vector<double>> &lines,
        bool tooltip = true)
{
    auto itx = lines.find("x");
    auto ity = lines.find("y");
    if (itx == lines.end() || ity == lines.end())
        return false;

    auto &xs = itx->second;
    auto &ys = ity->second;

    if (xs.size() != ys.size())
        return false;

    if (ImPlot::BeginPlot("Position", ImVec2(-1, 0), ImPlotFlags_Equal)) {
        const static auto color = IM_COL32(66, 135, 245, 255);
        ImPlot::SetupAxes("x", "y");
        ImPlot::PushStyleColor(ImPlotCol_Line, color);
        ImPlot::PlotLine("Postion",
                xs.data(),
                ys.data(),
                ys.size());

        if (ImPlot::IsPlotHovered() && tooltip) {

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

static bool render_space(
        std::map<std::string, std::vector<double>> &lines)
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

    if (ImPlot::BeginPlot("Space")) {
        ImPlot::SetupAxes("Time", "Space");

        ImPlot::PlotLine("Space",
                time.data(),
                space.data(),
                space.size());

        ImPlot::EndPlot();
    }
}

void render_telemetry(State *state)
{
    static bool show_plots = false;
    static char buf_path[BUF_SIZE];
    static Parser parser;
    static std::map<std::string, std::vector<double>> map;

    if (ImGui::BeginTabItem("Telemetry")) {

        if (state->previous_tab != Telemetry) {
            state->previous_tab = Telemetry;
            show_plots  = false;
            strcpy(buf_path, "../csv_samples/skidpad.csv");
        }

        ImGui::InputText("filepath", buf_path, BUF_SIZE);

        if (ImGui::Button("Load CSV")) {
            // Open file
            if (!parser.open(buf_path)) {
                std::cerr << "Failed to open file " << buf_path << "\n";
                return;
            }

            // Read all file
            parser.read_all();
            map = parser.to_mapd();

            show_plots = true;
        }

        if (show_plots) {
            // Show all the csv values based on time
            if (state->show_values || state->user_is_admin())
                render_values(map);

            // Show driver path
            if (state->show_path || state->user_is_admin())
                render_path(map);

            // Show space travelled
            if (state->show_space || state->user_is_admin())
                render_space(map);
        }

        ImGui::EndTabItem();
    }
}
