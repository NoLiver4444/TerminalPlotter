#pragma once

#include "function.hpp"

#include <optional>
#include <string>

namespace plotter {

std::optional<FunctionType> parseFunction(
    const std::string& expression);

}  // namespace plotter