#pragma once

#include <optional>

namespace plotter {

enum class FunctionType {
    Linear,
    Square,
    Sin,
    Cos,
    Sqrt,
    Log,
    Abs,
    Hyperbola
};

std::optional<double> evaluate(FunctionType function, double x);

}  // namespace plotter