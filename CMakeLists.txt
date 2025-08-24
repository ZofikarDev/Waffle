cmake_minimum_required(VERSION 3.30.0)
project(Waffle)

set(CMAKE_CXX_STANDARD 23)

include(CTest)
include(Thirdparty.cmake)

add_executable(WaffleCompiler
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_subdirectory(Libs)

target_link_libraries(WaffleCompiler PRIVATE WaffleLexer)