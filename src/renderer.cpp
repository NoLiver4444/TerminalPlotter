#include "../include/plotter/renderer.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace plotter {

namespace {

struct PlotBounds {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

bool isValidY(const Point& point) {
    return point.y.has_value();  
}

std::optional<PlotBounds> calculateBounds(
    const std::vector<Point>& points) {

    if (points.empty()) {
        return std::nullopt;
    }

    const auto firstValid =
        std::find_if(
            points.begin(),
            points.end(),
            [](const Point& point) {
                return isValidY(point);
            });

    if (firstValid == points.end()) {
        return std::nullopt;
    }

    PlotBounds bounds{
        points.front().x,
        points.back().x,
        *firstValid->y,
        *firstValid->y
    };

    for (const auto& point : points) {
        if (!isValidY(point)) {
            continue;
        }

        bounds.ymin =
            std::min(bounds.ymin, *point.y);

        bounds.ymax =
            std::max(bounds.ymax, *point.y);
    }

    return bounds;
}

std::string formatValue(double value) {
    std::ostringstream stream;

    stream << std::fixed
           << std::setprecision(2)
           << value;

    return stream.str();
}

void drawLine(
    std::vector<std::string>& canvas,
    ScreenPoint from,
    ScreenPoint to) {

    int x0 = static_cast<int>(from.column);
    int y0 = static_cast<int>(from.row);

    const int x1 = static_cast<int>(to.column);
    const int y1 = static_cast<int>(to.row);

    const int dx = std::abs(x1 - x0);
    const int dy = std::abs(y1 - y0);

    const int sx =
        x0 < x1 ? 1 : -1;

    const int sy =
        y0 < y1 ? 1 : -1;

    int error = dx - dy;

    while (true) {
        if (y0 >= 0 &&
            y0 < static_cast<int>(canvas.size()) &&
            x0 >= 0 &&
            x0 < static_cast<int>(canvas[y0].size())) {

            canvas[y0][x0] = '*';
        }

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

void drawAxes(
    std::vector<std::string>& canvas,
    const PlotBounds& bounds,
    const PlotConfig& config) {

    bool hasXAxis = false;
    bool hasYAxis = false;

    std::size_t xAxisRow = 0;
    std::size_t yAxisColumn = 0;

    // Y-axis: x = 0
    if (bounds.xmin <= 0.0 &&
        bounds.xmax >= 0.0 &&
        bounds.xmin != bounds.xmax) {

        double normalizedX =
            (0.0 - bounds.xmin) /
            (bounds.xmax - bounds.xmin);

        normalizedX =
            std::clamp(
                normalizedX,
                0.0,
                1.0);

        yAxisColumn = config.width / 2;  

        for (std::size_t row = 0;
             row < config.height;
             ++row) {

            canvas[row][yAxisColumn] = '|';
        }

        hasYAxis = true;
    }

    // X-axis: y = 0
    if (bounds.ymin <= 0.0 &&
        bounds.ymax >= 0.0 &&
        bounds.ymin != bounds.ymax) {

        double normalizedY =
            (0.0 - bounds.ymin) /
            (bounds.ymax - bounds.ymin);

        normalizedY =
            std::clamp(
                normalizedY,
                0.0,
                1.0);

        xAxisRow =
            static_cast<std::size_t>(
                std::lround(
                    (1.0 - normalizedY) *
                    static_cast<double>(
                        config.height - 1)));

        for (std::size_t column = 0;
             column < config.width;
             ++column) {

            canvas[xAxisRow][column] = '-';
        }

        hasXAxis = true;
    }

    if (hasXAxis && hasYAxis) {
        canvas[xAxisRow][yAxisColumn] = '+';
    }
}

void printGraph(
    const std::vector<std::string>& canvas,
    const PlotBounds& bounds,
    const PlotConfig& config) {

    constexpr int labelWidth = 10;

    const std::string ymaxLabel =
        formatValue(bounds.ymax);

    const std::string yminLabel =
        formatValue(bounds.ymin);

    std::cout << '\n';

    for (std::size_t row = 0;
         row < canvas.size();
         ++row) {

        if (row == 0) {
            std::cout
                << std::setw(labelWidth)
                << ymaxLabel;
        } else if (row == canvas.size() - 1) {
            std::cout
                << std::setw(labelWidth)
                << yminLabel;
        } else {
            std::cout
                << std::setw(labelWidth)
                << "";
        }

        std::cout
            << " | "
            << canvas[row]
            << '\n';
    }

    std::string xLabels(
        config.width,
        ' ');

    const std::string xminLabel =
        formatValue(bounds.xmin);

    const std::string xmaxLabel =
        formatValue(bounds.xmax);

    for (std::size_t i = 0;
         i < xminLabel.size() &&
         i < xLabels.size();
         ++i) {

        xLabels[i] = xminLabel[i];
    }

    if (xmaxLabel.size() <=
        xLabels.size()) {

        const std::size_t start =
            xLabels.size() -
            xmaxLabel.size();

        for (std::size_t i = 0;
             i < xmaxLabel.size();
             ++i) {

            xLabels[start + i] =
                xmaxLabel[i];
        }
    }

    std::cout
        << std::string(
               labelWidth + 3,
               ' ')
        << xLabels
        << "\n\n";
}

}  // namespace

std::vector<ScreenPoint> scalePoints(
    const std::vector<Point>& points,
    std::size_t width,
    std::size_t height) {

    std::vector<ScreenPoint> result;

    if (points.empty() ||
        width < 2 ||
        height < 2) {

        return result;
    }

    const auto bounds =
        calculateBounds(points);

    if (!bounds.has_value()) {
        return result;
    }

    result.reserve(points.size());

    for (const auto& point : points) {
        double normalizedX =
            (point.x - bounds->xmin) /
            (bounds->xmax - bounds->xmin);

        normalizedX =
            std::clamp(
                normalizedX,
                0.0,
                1.0);

        const std::size_t column =
            static_cast<std::size_t>(
                std::lround(
                    normalizedX *
                    static_cast<double>(
                        width - 1)));

        if (!isValidY(point)) {
            result.push_back({
                column,
                0,
                false
            });

            continue;
        }

        if (bounds->ymin ==
            bounds->ymax) {

            result.push_back({
                column,
                height / 2,
                true
            });

            continue;
        }

        double normalizedY =
            (*point.y - bounds->ymin) /
            (bounds->ymax -
             bounds->ymin);

        normalizedY =
            std::clamp(
                normalizedY,
                0.0,
                1.0);

        const std::size_t row =
            static_cast<std::size_t>(
                std::lround(
                    (1.0 - normalizedY) *
                    static_cast<double>(
                        height - 1)));

        result.push_back({
            column,
            row,
            true
        });
    }

    return result;
}

void drawGraph(
    const std::vector<Point>& points,
    const PlotConfig& config) {

    if (points.empty()) {
        std::cout
            << "\nNo points to draw.\n\n";
        return;
    }

    if (config.width < 2 ||
        config.height < 2) {

        std::cout
            << "\nInvalid plot size.\n\n";

        return;
    }

    const auto bounds =
        calculateBounds(points);

    if (!bounds.has_value()) {
        std::cout
            << "\nNo valid points "
               "in selected range.\n\n";

        return;
    }

    std::vector<std::string> canvas(
        config.height,
        std::string(
            config.width,
            ' ')
    );

    // Оси сначала, график поверх них.
    drawAxes(
        canvas,
        *bounds,
        config
    );

    const auto screenPoints =
        scalePoints(
            points,
            config.width,
            config.height
        );

    for (std::size_t i = 1;
         i < screenPoints.size();
         ++i) {

        const ScreenPoint& previous =
            screenPoints[i - 1];

        const ScreenPoint& current =
            screenPoints[i];

        // Разрыв области определения.
        if (!previous.valid ||
            !current.valid) {

            continue;
        }

        // const std::size_t rowDifference =
        //     previous.row > current.row
        //         ? previous.row - current.row
        //         : current.row - previous.row;

        if (!previous.valid || !current.valid) { //BUG
            continue;
        }

        drawLine(
            canvas,
            previous,
            current
        );
    }

    printGraph(
        canvas,
        *bounds,
        config
    );
}

}  // namespace plotter