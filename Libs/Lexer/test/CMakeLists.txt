add_executable(WaffleLexerTestSuite ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(WaffleLexerTestSuite PRIVATE
        WaffleLexer
        GTest::gtest_main
)

include(GoogleTest)

if (CMAKE_CROSSCOMPILING)
    # Can't run test exe at configure time, just register them by regex
    gtest_add_tests(TARGET WaffleLexerTestSuite TEST_SUFFIX .no_discovery)
else ()
    # Normal host build → discover tests automatically
    gtest_discover_tests(WaffleLexerTestSuite)
endif ()
