cmake_minimum_required(VERSION 3.26)
project(CandleScript)
set(CMAKE_CXX_STANDARD 23)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.9 # or a later release
)
FetchContent_MakeAvailable(Catch2)

FILE(GLOB_RECURSE
    Sources
        src/*.cpp
    )

add_executable(CandleScript main.cpp
        ${Sources})

target_include_directories(
        CandleScript
        PUBLIC src
)



add_executable(tests
        tests/main.tests.cpp
        tests/lexer.tests.cpp
        ${Sources}
)
target_include_directories(
        tests
        PUBLIC src
)
target_link_libraries(tests
        Catch2::Catch2
)