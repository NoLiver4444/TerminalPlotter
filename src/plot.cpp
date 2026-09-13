#include "../include/plotter/plot.hpp"

namespace plotter {

std::vector<Point> calculatePlot(
    FunctionType function,
    double xmin,
    double xmax,
    std::size_t width) {

    if (width < 2 || xmin >= xmax) {
        return {};
    }

    const double step =
        (xmax - xmin) /
        static_cast<double>(width); // BUG

    std::vector<Point> result;
    result.reserve(width);

    for (std::size_t i = 0; i < width; ++i) {
        const double x =
            xmin + static_cast<double>(i) * step;

        result.push_back({
            x,
            evaluate(function, x)
        });
    }

    return result;
}

}  // namespace plotter