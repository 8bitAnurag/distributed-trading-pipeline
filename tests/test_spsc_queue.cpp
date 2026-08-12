//
// Created by Anurag on 13/08/2026.
//

#include "test_spsc_queue.h"

#include "SPSCQueue.h"
#include <cassert>
#include <iostream>
int main() {
    ipc::SPSCQueue queue;
    core::Order order{};
    const bool result = queue.pop(order);
    assert(result == false);
    std::cout << "Test 1 passed: empty queue cannot be popped.\n";

    return 0;

}
