cmake_minimum_required(VERSION  3.10)
project(CambioMinimax CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD  20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(CambioMinimax ${SOURCES})

# Specify the include directories
include_directories(src)

# Set the build type to Release for x64
# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
