#include <iostream>

#include "imgui/imgui.h"
#include "implot/implot.h"

#include "../parser/parser.h"
#include "../common.h"
#include "../state.h"
#include "../user.h"

static bool render_values(
        std::unordered_map<std::string, std::vector<double>> &lines)
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
        ImPlot::EndPlot();
    }
}

static bool render_path(
        std::unordered_map<std::string, std::vector<double>> &lines,
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
                    if (line.first != "x" && line.first != "y") {
                        char buf[BUF_SIZE];
                        sprintf(buf, "%s: %lf",
                                line.first.data(),
                                line.second[mini]);
                        ImGui::Text(buf);
                    }
                }
                ImGui::EndTooltip();
            }
        }

        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }
}

void render_telemetry(State *state)
{
    static bool show_plots = false;
    static char buf_path[BUF_SIZE];
    static Parser parser;
    static std::unordered_map<std::string, std::vector<double>> map;

    if (ImGui::BeginTabItem("Telemetry")) {

        if (state->previous_tab != Telemetry) {
            state->previous_tab = Telemetry;
            show_plots = false;
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
        }

        ImGui::EndTabItem();
    }
}
