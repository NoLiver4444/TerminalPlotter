#include "../include/plotter/plot.hpp"
#include <cstdlib>
#include <vector>

namespace plotter {

std::vector<plotter::Point> calculatePlot(plotter::FunctionType function, double xmin, double xmax,
                                          std::size_t width) {
    double step = (xmax - xmin) / (width - 1);

    std::vector<plotter::Point> result;
    result.reserve(width + 1);

    double x = xmin, y = 0;

    for (std::size_t i = 0; i < width + 1; ++i) {
        y = plotter::evaluate(function, x);

        result.push_back({x, y});

        x += step;
    }

    return result;
}

}  // namespace plotter