cmake_minimum_required(VERSION 3.16)
project(point_cloud_viewer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PCL 1.14 REQUIRED COMPONENTS common io visualization)

add_executable(point_cloud_viewer src/main.cpp)

target_include_directories(point_cloud_viewer PRIVATE ${PCL_INCLUDE_DIRS})
target_link_directories(point_cloud_viewer PRIVATE ${PCL_LIBRARY_DIRS})
target_link_libraries(point_cloud_viewer ${PCL_LIBRARIES})
target_compile_definitions(point_cloud_viewer PRIVATE ${PCL_DEFINITIONS})
