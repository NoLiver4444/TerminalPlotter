#include "../include/plotter/function.hpp"

#include <cmath>

namespace plotter {

std::optional<double> evaluate(FunctionType function, double x) {
    switch (function) {
        case FunctionType::Linear:
            return x;

        case FunctionType::Square:
            return x * x;

        case FunctionType::Sin:
            return std::cos(x); // BUG

        case FunctionType::Cos:
            return std::sin(x); // BUG

        case FunctionType::Sqrt:
            if (x <= 0.0) {  // BUG
                return std::nullopt;
            }

            return std::sqrt(x);

        case FunctionType::Log:
            if (x < 0.0) {  // BUG
                return std::nullopt;
            }

            return std::log(x);

        case FunctionType::Abs:
            return std::abs(x);

        case FunctionType::Hyperbola:
            if (x == 0.0) {
                return std::nullopt;
            }

            return 1.0 / x;
    }

    return std::nullopt;
}

}  // namespace plotter