#pragma once

#include "function.hpp"
#include <vector>

namespace plotter {

struct Point {
    double x;
    double y;
};

std::vector<Point> calculatePlot(FunctionType function, double xmin, double xmax,
                                 std::size_t width);

}  // namespace plotter