//
// Created by Anurag on 11/08/2026.
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

        std::cout << "[Feed Handler] starting up...." << std::endl;

        //--------Creating Shared mem of size of SPSCQueue----------
        const std::size_t queue_size = sizeof(ipc::SPSCQueue);
        ipc::MemoryMappedFile shared_mem("Local\\TradingQueue", queue_size);
        //-----------------------------------------------------------------------------

        std::cout << "[Feed Handler] Shared memory allocated ("
        << shared_mem.size() << " bytes)" << std::endl;

        //---------Storing SPSCQueue in that shared mem-----------
        ipc::SPSCQueue* queue = new(shared_mem.data()) ipc::SPSCQueue();
        //---------------------------------------------------------------

        std::cout << "[Feed Handler] Queue initialized in shared RAM. Generating dummy orders..." << std::endl;

        //---------Generating random orders and pushing in queue--------
        for (uint64_t i = 1; i <= 1000; ++i) {
            core::Order dummy_order;
            dummy_order.id = i;
            dummy_order.price = 100 + (i % 10);
            dummy_order.quantity = 10;
            dummy_order.side = (i % 2 == 0) ? core::Side::BUY : core::Side::SELL;
            strcpy_s(dummy_order.symbol, "EURINR");
            while (!queue->push(dummy_order)) {
                std::this_thread::yield();
            }
            if (i%100 == 0) {
                std::cout << "[Feed Handler] Successfully pushed order ID: " << i <<  std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "[Feed Handler] Finished pushing 1000 dummy orders. Exiting..." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[Feed Handler] Fatal Error: "<< e.what() << std::endl;
        return 1;
    }
    return 0;
}
