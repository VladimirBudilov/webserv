cmake_minimum_required(VERSION 3.12)
project(webserv CXX)

# Set C++ standard to C++98
set(CMAKE_CXX_STANDARD 98)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -I./webserv/includes")

# Include directories
include_directories(webserv_github/includes)

# Link directories
link_directories(${PROJECT_NAME}/webserv_github/includes/*.hpp)

# Get source files from subdirectories
file(GLOB SRCS webserv_github/sources/dimka/*.cpp webserv_github/sources/core/*.cpp webserv_github/sources/main/main.cpp webserv_github/sources/HTTP/*.cpp)

# Create the executable
add_executable(webserv_github ${SRCS})

# Link libraries
target_include_directories(${PROJECT_NAME} PRIVATE webserv_github/includes/*.hpp)
