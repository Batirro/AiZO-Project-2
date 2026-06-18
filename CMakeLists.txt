cmake_minimum_required(VERSION 3.10)
project(AizoProject2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(include)
include_directories(lib) # Add lib directory for Parameters.h

file(GLOB SOURCES "src/*.cpp")

add_executable(AizoProject2 ${SOURCES})

# Link the pre-compiled library from the professor
target_link_directories(AizoProject2 PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(AizoProject2 parameters)
add_compile_definitions(PROJECT_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
