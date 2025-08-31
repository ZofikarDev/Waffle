add_library(WaffleParser STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser/Parser.cpp
)

target_include_directories(WaffleParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(WaffleParser PRIVATE WaffleLexer)

add_subdirectory(test)