cmake_minimum_required(VERSION 3.10.0)
project(SerializeXTests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(MainTest SerializeTests.cpp)

target_link_libraries(MainTest 
    PRIVATE 
        SerializeX
        Catch2::Catch2WithMain)
set_target_properties(MainTest PROPERTIES CXX_STANDARD 23)
