cmake_minimum_required(VERSION 3.10)
project(distributed_trading_pipeline)

set(CMAKE_CXX_STANDARD 20)

include_directories(core/include)
include_directories(ipc/include)

add_subdirectory(core)
add_subdirectory(ipc)
add_subdirectory(feed_handler)
add_subdirectory(engine)