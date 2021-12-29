cmake_minimum_required(VERSION 3.20)
project(os_lab6_clion)

set(CMAKE_CXX_STANDARD 17)

add_executable(control_node control_node.cpp topology_tree.h topology_node.h)
add_executable(calc_node calc_node.cpp)

target_compile_options(control_node PUBLIC -g)
target_compile_options(calc_node PUBLIC -g)

target_link_libraries(control_node zmq)
target_link_libraries(calc_node zmq)