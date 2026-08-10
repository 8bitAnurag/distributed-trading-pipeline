add_library(ipc_lib STATIC)

target_sources(ipc_lib
    PRIVATE
        src/MemoryMappedFile.cpp
)

target_include_directories(ipc_lib
        PUBLIC
            include
)
