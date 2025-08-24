add_executable(WaffleLexerTestSuite ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(WaffleLexerTestSuite PRIVATE
        WaffleLexer
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(WaffleLexerTestSuite)
