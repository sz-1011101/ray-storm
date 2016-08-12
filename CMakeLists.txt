cmake_minimum_required(VERSION 3.0)
project(ray_storm)

set(CMAKE_CXX_FLAGS " -g -O3 -std=c++11 -march=native -Wall -fopenmp")

include(FindPkgConfig)

# opencv
find_package(OpenCV REQUIRED)

# glm includes
include_directories(lib/glm/glm)

# includes
include_directories(include)

# sources
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(ray_storm ${SOURCES})
target_link_libraries(ray_storm ${OpenCV_LIBRARIES})