cmake_minimum_required(VERSION 3.10)
project(HFT_Order_Matching_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_executable(hft_engine
    src/Main.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
)

add_executable(hft_client
    client/Client.cpp
)

add_executable(hft_benchmark
    benchmark/Benchmark.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
)

target_link_libraries(hft_engine pthread)
target_link_libraries(hft_client)
target_link_libraries(hft_benchmark pthread)