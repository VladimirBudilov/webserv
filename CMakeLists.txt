cmake_minimum_required(VERSION 3.26)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv main.cpp
        webserv.hpp
        Server.cpp
        Server.hpp
        Location.cpp
        Location.hpp
)
