add_executable(engine src/main.cpp)

target_link_libraries(engine PRIVATE core_lib ipc_lib)