cmake_minimum_required(VERSION 3.10)
project(distributed_trading_pipeline)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

add_subdirectory(core)
add_subdirectory(feed_handler)
add_subdirectory(ipc)
add_subdirectory(engine)
