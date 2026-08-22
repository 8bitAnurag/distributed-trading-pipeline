//
// Created by Anurag on 22/08/2026.
//

#include <cassert>
#include <iostream>

#include "../engine/include/order_book.h"


core::Order create_order(
    uint64_t id,
    uint64_t price,
    uint32_t quantity,
    core::Side side
) {
    core::Order order{};

    order.id = id;
    order.price = price;
    order.quantity = quantity;
    order.side = side;

    return order;
}


void test_single_order_add() {

    engine::OrderBook book;

    auto order = create_order(
        1,
        10000,
        50,
        core::Side::BUY
    );

    auto handle = book.add_order(order);

    assert(handle != engine::INVALID_HANDLE);

    auto best_bid = book.best_bid();

    assert(best_bid != nullptr);
    assert(best_bid->order_count == 1);
    assert(best_bid->total_quantity == 50);

    std::cout << "Test 1 passed: single order add.\n";
}


void test_multiple_orders_same_price() {

    engine::OrderBook book;

    book.add_order(create_order(1,10000,10,core::Side::BUY));
    book.add_order(create_order(2,10000,20,core::Side::BUY));
    book.add_order(create_order(3,10000,30,core::Side::BUY));


    auto best_bid = book.best_bid();

    assert(best_bid != nullptr);
    assert(best_bid->order_count == 3);
    assert(best_bid->total_quantity == 60);


    std::cout << "Test 2 passed: multiple orders same price.\n";
}


void test_cancel_middle_order() {

    engine::OrderBook book;


    auto h1 =
        book.add_order(create_order(1,10000,10,core::Side::BUY));

    auto h2 =
        book.add_order(create_order(2,10000,20,core::Side::BUY));

    auto h3 =
        book.add_order(create_order(3,10000,30,core::Side::BUY));


    bool result = book.cancel_order(h2);

    assert(result);


    auto best_bid = book.best_bid();

    assert(best_bid != nullptr);
    assert(best_bid->order_count == 2);
    assert(best_bid->total_quantity == 40);


    std::cout << "Test 3 passed: middle order cancellation.\n";
}


void test_cancel_all_orders_price_level_empty() {

    engine::OrderBook book;


    auto handle =
        book.add_order(create_order(
            1,
            10000,
            50,
            core::Side::BUY
        ));


    assert(book.cancel_order(handle));


    auto best_bid = book.best_bid();


    // After removing the only bid,
    // book should not contain this price anymore
    assert(best_bid == nullptr ||
           best_bid->order_count == 0);


    std::cout << "Test 4 passed: empty price level cleanup.\n";
}


void test_generation_protection() {

    engine::OrderBook book;


    auto old_handle =
        book.add_order(create_order(
            1,
            10000,
            10,
            core::Side::BUY
        ));


    assert(book.cancel_order(old_handle));


    auto new_handle =
        book.add_order(create_order(
            2,
            10000,
            20,
            core::Side::BUY
        ));


    // Old handle should not cancel new order
    bool result =
        book.cancel_order(old_handle);


    assert(result == false);


    // New handle should still work
    assert(book.cancel_order(new_handle));


    std::cout << "Test 5 passed: generation protection.\n";
}


void test_best_bid_ask() {

    engine::OrderBook book;


    book.add_order(create_order(
        1,
        10000,
        10,
        core::Side::BUY
    ));

    book.add_order(create_order(
        2,
        10500,
        20,
        core::Side::BUY
    ));


    book.add_order(create_order(
        3,
        11000,
        30,
        core::Side::SELL
    ));

    book.add_order(create_order(
        4,
        10800,
        40,
        core::Side::SELL
    ));


    auto bid = book.best_bid();
    auto ask = book.best_ask();


    assert(bid != nullptr);
    assert(ask != nullptr);


    assert(bid->total_quantity == 20);
    assert(ask->total_quantity == 40);


    std::cout << "Test 6 passed: best bid/ask separation.\n";
}


int main() {

    test_single_order_add();

    test_multiple_orders_same_price();

    test_cancel_middle_order();

    test_cancel_all_orders_price_level_empty();

    test_generation_protection();

    test_best_bid_ask();


    std::cout << "\nAll OrderBook tests passed.\n";

    return 0;
}