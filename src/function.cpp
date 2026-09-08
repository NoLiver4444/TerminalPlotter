#include "../include/plotter/function.hpp"

#include "math.h"
#include <cmath>

namespace plotter {

double evaluate(FunctionType function, double x) {
    switch (function) {
        case plotter::FunctionType::Linear:
            return x;

        case plotter::FunctionType::Square:
            return x * x;

        case plotter::FunctionType::Cos:
            return std::cos(x);

        case plotter::FunctionType::Sin:
            return std::sin(x);
    }

    return 0.0;
}

}  // namespace plotter