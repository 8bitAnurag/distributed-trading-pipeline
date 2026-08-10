//
// Created by Anurag on 10/08/2026.
//

#ifndef DISTRIBUTED_TRADING_PIPELINE_SPSC_QUEUE_H
#define DISTRIBUTED_TRADING_PIPELINE_SPSC_QUEUE_H
#include "../../core/include/limits.h"
#include <cstddef>
#include "../../core/include/order.h"
#include <atomic>
namespace ipc {
    class SPSCQueue {
    private:
        static constexpr std::size_t CAPACITY = core::RING_BUFFER_CAPACITY;  //capcity

        core::Order buffer[CAPACITY];

        alignas(core::CACHE_LINE_SIZE) std::atomic<std::size_t> head{0}; //Head
        alignas(core::CACHE_LINE_SIZE) std::atomic<std::size_t> tail{0}; //Tail

    public:
        bool push(const core::Order& order) {
            const std::size_t current_head = head.load(std::memory_order_relaxed);
            const std::size_t next_head = (current_head + 1) & (CAPACITY -1 );

            if (next_head == tail.load(std::memory_order_acquire)) {
                return false;
            }

            buffer[current_head] = order; // store order
            head.store(next_head, std::memory_order_release);
            return true;
        }

        bool pop(core::Order& order) {
            const std::size_t current_tail = tail.load(std::memory_order_relaxed);
            if (current_tail == head.load(std::memory_order_acquire)) {
                return false;
            }
            std::size_t next_tail = (current_tail+1) & (CAPACITY-1);
            order = buffer[current_tail]; // read order
            tail.store(next_tail, std::memory_order_relaxed);
            return true;
        }
    };
}
#endif //DISTRIBUTED_TRADING_PIPELINE_SPSC_QUEUE_H
