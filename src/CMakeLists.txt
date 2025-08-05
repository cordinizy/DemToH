cmake_minimum_required(VERSION 3.27)
project(DemToH)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/3rdparty/Eigen/include/eigen3)
include_directories(/usr/local/3rdparty/gdal/include)
include_directories(/usr/local/3rdparty/boost/include)

link_directories(/usr/local/3rdparty/gdal/lib)
link_directories(/usr/local/3rdparty/boost/lib)

add_executable(DemToH main.cpp)
target_link_libraries(DemToH gdal boost_filesystem)
