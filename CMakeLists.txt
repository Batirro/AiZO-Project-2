cmake_minimum_required(VERSION 3.10)
project(AizoProject2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(AizoProject2 ${SOURCES})
