cmake_minimum_required(VERSION 3.16)
project(engine)
set(CMAKE_CXX_STANDARD 20)

add_executable(engine src/main.cpp src/Loader.cpp src/Simulator.cpp src/Writer.cpp src/Metrics.cpp src/strategies/BuyAndHold.cpp src/strategies/MovingAverageCrossover.cpp src/AssetRegistry.cpp src/strategies/MeanReversion.cpp)

# --- Tests ---
enable_testing()

add_executable(tests
    tests/catch_amalgamated.cpp        # Catch2's machinery + main, compiled once
    tests/test_metrics.cpp             # test files VVV
    tests/test_integration.cpp  
    tests/test_loader.cpp          
    src/Metrics.cpp                    # code under test VVV
    src/Loader.cpp                     
    src/Simulator.cpp
    src/Writer.cpp 
    src/strategies/BuyAndHold.cpp 
    src/strategies/MovingAverageCrossover.cpp
    src/AssetRegistry.cpp
)

target_include_directories(tests PRIVATE src tests)

add_test(NAME all_tests COMMAND tests)