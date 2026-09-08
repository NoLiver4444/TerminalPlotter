#include "../include/plotter/renderer.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace plotter {

namespace {

void drawLine(std::vector<std::string>& canvas, ScreenPoint from, ScreenPoint to) {
    int x0 = static_cast<int>(from.column);
    int y0 = static_cast<int>(from.row);

    int x1 = static_cast<int>(to.column);
    int y1 = static_cast<int>(to.row);

    const int dx = std::abs(x1 - x0);
    const int dy = std::abs(y1 - y0);

    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;

    int error = dx - dy;

    while (true) {
        canvas[y0][x0] = '*';

        if (x0 == x1 && y0 == y1) {
            break;
        }

        const int error2 = 2 * error;

        if (error2 > -dy) {
            error -= dy;
            x0 += sx;
        }

        if (error2 < dx) {
            error += dx;
            y0 += sy;
        }
    }
}

void drawAxes(std::vector<std::string>& canvas, const std::vector<Point>& points,
              const PlotConfig& config) {
    if (points.empty()) {
        return;
    }

    const double xmin = points.front().x;
    const double xmax = points.back().x;

    double ymin = points.front().y;
    double ymax = points.front().y;

    for (const auto& point : points) {
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }

    bool hasXAxis = false;
    bool hasYAxis = false;

    std::size_t xAxisRow = 0;
    std::size_t yAxisColumn = 0;

    if (xmin <= 0.0 && xmax >= 0.0 && xmin != xmax) {
        const double normalizedX = (0.0 - xmin) / (xmax - xmin);

        yAxisColumn = static_cast<std::size_t>(std::lround(normalizedX * (config.width - 1)));

        for (std::size_t row = 0; row < config.height; ++row) {
            canvas[row][yAxisColumn] = '|';
        }

        hasYAxis = true;
    }

    if (ymin <= 0.0 && ymax >= 0.0 && ymin != ymax) {
        const double normalizedY = (0.0 - ymin) / (ymax - ymin);

        xAxisRow = static_cast<std::size_t>(std::lround((1.0 - normalizedY) * (config.height - 1)));

        for (std::size_t column = 0; column < config.width; ++column) {
            canvas[xAxisRow][column] = '-';
        }

        hasXAxis = true;
    }

    if (hasXAxis && hasYAxis) {
        canvas[xAxisRow][yAxisColumn] = '+';
    }
}

}  // namespace

std::vector<ScreenPoint> scalePoints(const std::vector<Point>& points, std::size_t height) {
    std::vector<ScreenPoint> result;

    if (points.empty()) {
        return result;
    }

    result.reserve(points.size());

    double ymin = points.front().y;
    double ymax = points.front().y;

    for (const auto& point : points) {
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }

    if (ymin == ymax) {
        const std::size_t row = height / 2;

        for (std::size_t i = 0; i < points.size(); ++i) {
            result.push_back({i, row});
        }

        return result;
    }

    for (std::size_t i = 0; i < points.size(); ++i) {
        const double normalizedY = (points[i].y - ymin) / (ymax - ymin);

        const std::size_t row =
            static_cast<std::size_t>(std::lround((1.0 - normalizedY) * (height - 1)));

        result.push_back({i, row});
    }

    return result;
}

void drawGraph(const std::vector<Point>& points, const PlotConfig& config) {
    if (points.empty()) {
        return;
    }

    std::vector<std::string> canvas(config.height, std::string(config.width, ' '));

    drawAxes(canvas, points, config);

    const auto screenPoints = scalePoints(points, config.height);

    for (std::size_t i = 1; i < screenPoints.size(); ++i) {
        drawLine(canvas, screenPoints[i - 1], screenPoints[i]);
    }

    std::cout << '\n';

    for (const auto& row : canvas) {
        std::cout << row << '\n';
    }

    std::cout << '\n';
}

}  // namespace plotter