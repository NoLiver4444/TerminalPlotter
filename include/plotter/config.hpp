#pragma once

#include <cstddef>

namespace plotter {

struct PlotConfig {
    std::size_t width = 80;
    std::size_t height = 25;
};

}  // namespace plotter