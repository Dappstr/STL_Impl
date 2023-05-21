cmake_minimum_required(VERSION 3.10)

# Set the project name
project(STL_impl)

# Add the include directory
include_directories(include)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add an executable with the above sources
add_executable(binary ${SOURCES})

