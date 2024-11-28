cmake_minimum_required(VERSION 3.16)
project(QuantEdge)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add the main executable
add_executable(QuantEdge ${SOURCES})

# Enable testing
enable_testing()

# Download and configure Google Test
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(TestPairsTrader test/TestPairsTrader.cpp)

# Link test executable against the main sources and gtest libraries
target_sources(TestPairsTrader PRIVATE ${SOURCES})
target_link_libraries(TestPairsTrader PRIVATE gtest_main)

# Add the test to CTest
add_test(NAME TestPairsTrader COMMAND TestPairsTrader)

