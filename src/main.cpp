#include "../include/plotter/config.hpp"
#include "../include/plotter/function.hpp"
#include "../include/plotter/plot.hpp"
#include "../include/plotter/renderer.hpp"

int main() {
    plotter::PlotConfig config;

    auto points = plotter::calculatePlot(plotter::FunctionType::Cos, -10.0, 10.0, config.width);

    plotter::drawGraph(points, config);

    return 0;
}