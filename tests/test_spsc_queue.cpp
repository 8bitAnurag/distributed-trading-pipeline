//
// Created by Anurag on 13/08/2026.
//

#include "test_spsc_queue.h"

#include "SPSCQueue.h"
#include <cassert>
#include <thread>
#include <cstring>
#include <iostream>
int main() {
    ipc::SPSCQueue queue;
    core::Order order{};
    const bool result = queue.pop(order);
    assert(result == false);
    std::cout << "Test 1 passed: empty queue cannot be popped.\n";
////////////////////////------------------------//////////////////////////////////////

    core::Order sent_order{};
    sent_order.id = 67;
    sent_order.price = 420;
    sent_order.quantity = 10;
    sent_order.side = core::Side::BUY;
    std::strncpy(sent_order.symbol, "AAPL", core::MAX_SYMBOL_LENGTH - 1);
    sent_order.symbol[core::MAX_SYMBOL_LENGTH - 1] = '\0';
    bool push_result = queue.push(sent_order);
    assert(push_result);
    core::Order received_order{};
    bool pop_result = queue.pop(received_order);
    assert(pop_result);
    assert(received_order.id == sent_order.id);
    assert(received_order.price == sent_order.price);
    assert(received_order.quantity == sent_order.quantity);
    assert(received_order.side == sent_order.side);
    assert(std::strcmp(received_order.symbol, sent_order.symbol) == 0);
    std::cout << "Test 2 passed: push/pop preserves order data.\n";
    ////////////////////////------------------------//////////////////////////////////////

    for (uint64_t i = 1; i <= 5; i++) {
        core::Order order{};
        order.id = i;
        assert(queue.push(order));
    }
    for (uint64_t expected_id = 1; expected_id <= 5; expected_id++) {
        core::Order received_order{};
        assert(queue.pop(received_order));
        assert(received_order.id == expected_id);
    } std::cout << "Test 3 passed: FIFO ordering is correct.\n";
    ////////////////////////------------------------//////////////////////////////////////

    std::size_t push_count = 0;
    while (true) {
        core::Order sent_order{};
        sent_order.id = push_count+1;
        if (!queue.push(sent_order)) { break;}
        push_count++;
    }
    core::Order extra_order{};
    extra_order.id = 69420;
    assert(!queue.push(extra_order));
    for (uint64_t expected_id = 1; expected_id <= push_count; expected_id++) {
        core::Order received_order{};
        assert(queue.pop(received_order));
        assert(received_order.id == expected_id);
    } std::cout << "Test 4 passed: full queue does not overwrite unread orders.\n";
    ////////////////////////------------------------//////////////////////////////////////

    ipc::SPSCQueue wrap_queue;
    push_count =0;
    while(true) {
        core::Order push_order{};
        push_order.id = push_count+1;
        if (!wrap_queue.push(push_order)){ break;}
        push_count++;
    }
    const std::size_t remove_count = push_count / 2;

    for (uint64_t expected_id = 1;expected_id <= remove_count;expected_id++) {
        core::Order received_order{};
        assert(wrap_queue.pop(received_order));
        assert(received_order.id == expected_id);
    }
    for (std::size_t i = 1; i <= remove_count; i++) {
        core::Order test_order{};
        test_order.id = push_count + i;
        assert(wrap_queue.push(test_order));
    }
    for (uint64_t expected_id = remove_count + 1;expected_id <= push_count + remove_count;expected_id++) {
        core::Order received_order{};
        assert(wrap_queue.pop(received_order));
        assert(received_order.id == expected_id);
     }
    core::Order final_order{};
    assert(!wrap_queue.pop(final_order));
    std::cout << "Test 5 passed: ring-buffer wrap-around works correctly.\n";
    ////////////////////////------------------------//////////////////////////////////////

    ipc::SPSCQueue concurrent_queue;
    constexpr uint64_t Test_Order_Count = 1'000'000'000;

    std::thread producer([&concurrent_queue]() {
        for (uint64_t order_id = 1; order_id <= Test_Order_Count; order_id++) {
            core::Order sent_order{};
            sent_order.id = order_id;
            while (!concurrent_queue.push(sent_order)) { // loops when queue is full
                std::this_thread::yield();
            }
        }
    });
    std::thread consumer([&concurrent_queue]() {
        for (uint64_t expected_id = 1; expected_id <= Test_Order_Count; expected_id++) {
            core::Order received_order{};
            while (!concurrent_queue.pop(received_order)) { // loops when queue is empty
                std::this_thread::yield();
            }
            assert(received_order.id == expected_id);
        }
    });
    producer.join();
    consumer.join();
    std::cout << "Test 6 passed: concurrent producer/consumer stress test.\n";
    return 0;

}
