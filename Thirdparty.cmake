include(FetchContent)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.7
)

FetchContent_MakeAvailable(magic_enum)

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

include_directories(
        ${magic_enum_SOURCE_DIR}/include
        ${frozen_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/googlemock/include
        ${gtest_SOURCE_DIR}/googletest/include
)