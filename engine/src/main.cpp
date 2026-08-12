//
// Created by Anurag on 12/08/2026.
//

#include <climits>
#include <cstdint>

#include <iostream>
#include <thread>
#include <chrono>

#include "../../ipc/include/MemoryMappedFile.h"
#include "../../ipc/include/SPSCQueue.h"
#include "../../core/include/order.h"
int main() {
    try {
        std::cout << "[Matching Engine] Starting up..." << std::endl;
        //------------Connecting to shared mem--------------------
        const std::size_t queue_size = sizeof(ipc::SPSCQueue);
        ipc::MemoryMappedFile shared_mem("Local\\TradingQueue", queue_size);

        std::cout << "[Matching Engine] Connected to shared memory." << std::endl;
        //-------------void* of shared_mem.data() to queue* conversion---------------
        ipc::SPSCQueue* queue = reinterpret_cast<ipc::SPSCQueue*>(shared_mem.data());
        std::cout << "[Matching Engine] Queue located. Listening for orders...\n" << std::endl;

        uint64_t orders_processed = 0;
        //---------------Processing Orders---------------------
        while (orders_processed < 1000) {
            core::Order received_order;

            while (!queue->pop(received_order)) {
                std::this_thread::yield();
            }
            orders_processed++;
            if (orders_processed % 100 == 0) {
                std::cout << "[Matching Engine] Processed Order ID: " << received_order.id
                          << " | Price: " << received_order.price
                          << " | Qty: " << received_order.quantity
                          << " | Type: " << (received_order.side == core::Side::BUY ? "Buy" : "Sell") << std::endl;
            }
        }
        std::cout << "[Matching Engine] Finished processing 1000 orders. Exiting..." << std::endl;

            } catch(const std::exception& e) {
                std::cerr << "[Matching Engine] Fatal Error: " << e.what() << std::endl;
                return 1;
            }

            return 0;
        }