cmake_minimum_required(VERSION 3.12)

project(webserv)

set(CMAKE_CXX_STANDARD 98)

# Compiled directories
set(SRC webserv_github/sources)
set(OBJ webserv_github/objects)
set(SUBDIRS dimka)

# Folder directions
foreach(dir ${SUBDIRS})
    list(APPEND SRC_DIR ${SRC}/${dir})
    list(APPEND OBJ_DIR ${OBJ}/${dir})
endforeach()

# File directions
file(GLOB SRCS ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Add any additional flags or libraries as needed
# For example, if you need ncurses, you can uncomment the following line:
# target_link_libraries(${PROJECT_NAME} ncurses)
