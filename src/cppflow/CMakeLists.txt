cmake_minimum_required(VERSION 3.10)

project(model)

add_executable(model main.cpp)

find_package(cppflow REQUIRED)

target_link_libraries(model
                      PUBLIC
                      cppflow::cppflow)
