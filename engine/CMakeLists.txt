add_executable(engine src/main.cpp
        include/order_book.h
        src/order_book.cpp
)

target_link_libraries(engine PRIVATE core_lib ipc_lib)