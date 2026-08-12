add_executable(feed_handler src/main.cpp)
target_link_libraries(feed_handler PRIVATE ipc_lib)