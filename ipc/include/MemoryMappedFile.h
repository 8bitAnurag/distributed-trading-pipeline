//
// Created by Anurag on 10/08/2026.
//

#ifndef DISTRIBUTED_TRADING_PIPELINE_MEMORY_MAPPED_H
#define DISTRIBUTED_TRADING_PIPELINE_MEMORY_MAPPED_H
#pragma once

#include <climits>
#include <cstdint>
#include <windows.h>
#include <string>
#include <stdexcept>

namespace ipc {
    class MemoryMappedFile {
    public:
        MemoryMappedFile(const std::string& filepath, std::size_t size);
        ~MemoryMappedFile();

        MemoryMappedFile(const MemoryMappedFile&) = delete;
        MemoryMappedFile& operator=(const MemoryMappedFile&) = delete;

        void* data() const;
        std::size_t size() const;

    private:
        std::string filepath_;
        std::size_t size_;

        void* mapped_data_;
        void* handle_;
    };
}
#endif //DISTRIBUTED_TRADING_PIPELINE_MEMORY_MAPPED_H
