cmake_minimum_required(VERSION 3.10)
project(DA2026_PRJ1_T16_G1)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers in src/
include_directories(${PROJECT_SOURCE_DIR}/include)

# List source files
set(SOURCES
    main.cpp
    src/cli_parser.cpp
)

# Build executable
add_executable(${PROJECT_NAME} ${SOURCES})