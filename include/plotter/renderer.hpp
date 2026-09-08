#pragma once

#include "config.hpp"
#include "plot.hpp"

#include <cstddef>
#include <vector>
#include <string>

namespace plotter {

struct ScreenPoint {
    std::size_t column;
    std::size_t row;
};

std::vector<ScreenPoint> scalePoints(const std::vector<Point>& points, std::size_t height);

void drawGraph(const std::vector<Point>& points, const PlotConfig& config);

}  // namespace plotter