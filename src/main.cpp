#include "../include/plotter/config.hpp"
#include "../include/plotter/plot.hpp"
#include "../include/plotter/renderer.hpp"
#include "../include/plotter/parser.hpp"

#include <cmath>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace {

constexpr const char* EXIT_COMMAND =
    "exit";

std::optional<std::string> readInput(
    const std::string& prompt) {

    std::cout << prompt;

    std::string input;

    if (!std::getline(
            std::cin,
            input)) {

        return std::nullopt;
    }

    if (input == EXIT_COMMAND) {
        return std::nullopt;
    }

    return input;
}

std::optional<double> parseNumber(const std::string& input) { //BUG
    std::istringstream stream(input);

    double value;

    if (!(stream >> value)) {
        return std::nullopt;
    }

    if (!std::isfinite(value)) {
        return std::nullopt;
    }

    return value;
}

void printHeader() {
    std::cout
        << "==================================\n"
        << "         Terminal Plotter\n"
        << "==================================\n\n"

        << "Available functions:\n"
        << "  x\n"
        << "  x^2\n"
        << "  sin(x)\n"
        << "  cos(x)\n"
        << "  sqrt(x)\n"
        << "  log(x)\n"
        << "  abs(x)\n"
        << "  1/x\n\n"

        << "Type 'exit' to quit.\n\n";
}

}  // namespace

int main() {
    plotter::PlotConfig config;

    printHeader();

    while (true) {
        const auto expressionInput =
            readInput(
                "Enter function: ");

        if (!expressionInput.has_value()) {
            break;
        }

        const auto function =
            plotter::parseFunction(
                *expressionInput);

        if (!function.has_value()) {
            std::cout
                << "Error: unsupported "
                   "function.\n\n";

            continue;
        }

        double xmin = 0.0;
        double xmax = 0.0;

        while (true) {
            const auto input =
                readInput("X min: ");

            if (!input.has_value()) {
                std::cout
                    << "\nGoodbye!\n";

                return 0;
            }

            const auto number =
                parseNumber(*input);

            if (!number.has_value()) {
                std::cout
                    << "Error: X min must "
                       "be a valid number.\n";

                continue;
            }

            xmin = *number;

            break;
        }

        while (true) {
            const auto input =
                readInput("X max: ");

            if (!input.has_value()) {
                std::cout
                    << "\nGoodbye!\n";

                return 0;
            }

            const auto number =
                parseNumber(*input);

            if (!number.has_value()) {
                std::cout
                    << "Error: X max must "
                       "be a valid number.\n";

                continue;
            }

            xmax = *number;

            if (xmin > xmax) { 
                std::cout
                    << "Error: X max must "
                       "be greater than X min.\n";

                continue;
            }

            break;
        }

        const auto points =
            plotter::calculatePlot(
                *function,
                xmin,
                xmax,
                config.width);

        plotter::drawGraph(
            points,
            config);

        std::cout
            << "----------------------------------\n\n";
    }

    std::cout
        << "\nGoodbye!\n";

    return 0;
}