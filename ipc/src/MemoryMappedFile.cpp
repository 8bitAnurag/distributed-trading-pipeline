//
// Created by Anurag on 10/08/2026.
//

#include "../include/MemoryMappedFile.h"
#include <windows.h>
#include <stdexcept>

namespace ipc {
    // --- The Constructor ---
    MemoryMappedFile::MemoryMappedFile(const std::string &filepath, std::size_t size)
        : filepath_(filepath), size_(size), mapped_data_(nullptr), handle_(nullptr) {

        HANDLE hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            static_cast<DWORD>(size_),
            filepath_.c_str());

        if (hMapFile == NULL) {
            throw std::runtime_error("Windows Error: Failed to create file mapping.");
        }

        handle_ = hMapFile;
        mapped_data_ = MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            size_);

        if (mapped_data_ == NULL) {
            throw std::runtime_error("Windows Error: Failed to map view of file.");
        }
    }

    // --- The Destructor ---
    MemoryMappedFile::~MemoryMappedFile() {
        if (mapped_data_ != nullptr) {
            UnmapViewOfFile(mapped_data_);
        }
        if (handle_ != nullptr) {
            CloseHandle(static_cast<HANDLE>(handle_));
        }
    }

    // --- The Getters ---
    void* MemoryMappedFile::data() const {
        return mapped_data_;
    }
    std::size_t MemoryMappedFile::size() const {
        return size_;
    }

};
