cmake_minimum_required(VERSION 3.16)
project(QuantEdge)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add QuantEdge library
add_library(QuantEdge ${SOURCES})

# Add main executable
add_executable(QuantEdgeRunner src/main.cpp)

# Link QuantEdge library to main executable
target_link_libraries(QuantEdgeRunner PRIVATE QuantEdge)

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

# Link test executable against QuantEdge library and gtest
target_link_libraries(TestPairsTrader PRIVATE QuantEdge gtest_main)

# Register individual Google Test cases with CTest
include(GoogleTest)
gtest_discover_tests(TestPairsTrader)

# Add the test to CTest (if discover tests does not automatically include them)
add_test(NAME TestPairsTrader COMMAND TestPairsTrader)

