add_library(WaffleLexer STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer/Lexer.cpp
        src/Lexer/Types.cpp
)

target_include_directories(WaffleLexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_subdirectory(test)