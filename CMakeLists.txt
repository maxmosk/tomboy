cmake_minimum_required(VERSION 3.25)
project(ParaCL LANGUAGES CXX)

set(SOURCE_DIR     "src")
set(LANGUAGE_DIR  "lang")

find_package(FLEX 2.6 REQUIRED)

flex_target(SCANNER "${LANGUAGE_DIR}/paracl.l" "${CMAKE_CURRENT_BINARY_DIR}/paracl.yy.cpp")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(tomboy "${SOURCE_DIR}/main.cpp" ${FLEX_SCANNER_OUTPUTS})
