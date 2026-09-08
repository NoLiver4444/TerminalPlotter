cmake_minimum_required(VERSION 3.16)

project(TerminalPlotter
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(terminal_plotter
    src/main.cpp
    src/function.cpp
    src/plot.cpp
    src/renderer.cpp
)

target_include_directories(terminal_plotter
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(terminal_plotter PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# =========================
# clang-format
# =========================

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
    file(GLOB_RECURSE FORMAT_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*.hpp
        ${PROJECT_SOURCE_DIR}/tests/*.cpp
        ${PROJECT_SOURCE_DIR}/tests/*.hpp
    )

    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT} -i ${FORMAT_FILES}
        COMMENT "Formatting source files with clang-format"
    )
else()
    message(WARNING "clang-format not found")
endif()