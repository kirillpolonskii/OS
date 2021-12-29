cmake_minimum_required(VERSION 3.20)
project(os_cp_clion)

set(CMAKE_CXX_STANDARD 17)

add_executable(progA prog_a.cpp zmq_helper.hpp)
add_executable(progB prog_b.cpp zmq_helper.hpp)
add_executable(progC prog_c.cpp zmq_helper.hpp)


target_compile_options(progA PUBLIC -g)
target_compile_options(progB PUBLIC -g)
target_compile_options(progC PUBLIC -g)

target_link_libraries(progA zmq)
target_link_libraries(progB zmq)
target_link_libraries(progC zmq)
