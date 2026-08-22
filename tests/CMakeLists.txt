add_executable(test_spsc_queue
        test_spsc_queue.cpp
)

target_link_libraries(test_spsc_queue
        PRIVATE ipc_lib
)
add_executable(test_order_book
        test_order_book.cpp
        ../engine/src/order_book.cpp
)