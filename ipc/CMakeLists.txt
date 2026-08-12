add_library(ipc_lib STATIC
    src/MemoryMappedFile.cpp
)

target_include_directories(ipc_lib
    PUBLIC
        include
)