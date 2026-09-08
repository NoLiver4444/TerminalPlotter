#pragma once

namespace plotter {

enum class FunctionType { Linear, Square, Sin, Cos };

double evaluate(FunctionType function, double x);

}  // namespace plotter