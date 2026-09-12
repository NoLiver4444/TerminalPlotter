#include "../include/plotter/parser.hpp"

namespace plotter {

std::optional<FunctionType> parseFunction(
    const std::string& expression) {

    if (expression == "x") {
        return FunctionType::Linear;
    }

    if (expression == "x^2") {
        return FunctionType::Square;
    }

    if (expression == "sin(x)") {
        return FunctionType::Sin;
    }

    if (expression == "cos(x)") {
        return FunctionType::Cos;
    }

    if (expression == "sqrt(x)") {
        return FunctionType::Sqrt;
    }

    if (expression == "log(x)") {
        return FunctionType::Log;
    }

    if (expression == "abs(x)") {
        return FunctionType::Abs;
    }

    if (expression == "1/x") {
        return FunctionType::Hyperbola;
    }

    return std::nullopt;
}

}  // namespace plotter