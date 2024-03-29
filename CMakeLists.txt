cmake_minimum_required(VERSION 3.0)
project(ParaCL LANGUAGES CXX)

set(SOURCE_DIR     "src")
set(LANGUAGE_DIR  "lang")

find_package(BISON 3.8 REQUIRED)
find_package(FLEX  2.6 REQUIRED)

flex_target(scanner
    "${LANGUAGE_DIR}/paracl.l"
    "${CMAKE_CURRENT_BINARY_DIR}/paracl.yy.cpp")
bison_target(parser
    lang/paracl.y
    ${CMAKE_CURRENT_BINARY_DIR}/paracl.tab.cpp
    COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/paracl.tab.hh")

add_flex_bison_dependency(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${SOURCE_DIR})

add_executable(tomboy
    "${SOURCE_DIR}/main.cpp"
    "${SOURCE_DIR}/nodes.cpp"
    ${FLEX_scanner_OUTPUTS}
    ${BISON_parser_OUTPUTS}
            )

target_compile_features(tomboy PRIVATE cxx_std_20)
