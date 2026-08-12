add_library(core_lib INTERFACE
        ../tests/test_spsc_queue.cpp
        ../tests/test_spsc_queue.h
        ../tests/test_order_book.cpp
        ../tests/test_order_book.h)

target_include_directories(core_lib INTERFACE include)