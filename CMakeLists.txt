cmake_minimum_required(VERSION 3.25)
project(ParaCL LANGUAGES CXX)

set(SOURCE_DIR src)

add_executable(tomboy ${SOURCE_DIR}/main.cpp)