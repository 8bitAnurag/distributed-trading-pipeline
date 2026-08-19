//
// Created by Anurag on 15/08/2026.
//

#include "../include/order_book.h"

namespace engine {

    void OrderBook::free_node(std::size_t node_index) {

        OrderNode& node = orders_pool_[node_index];

        ++node.generation;

        node.next = INVALID_INDEX;
        node.prev = INVALID_INDEX;

        free_indices_[free_indices_count_++] = node_index;
    }

    OrderHandle OrderBook::add_order(const core::Order &order) {

        //-------------Order validation , BUY/SELL , free node creation ---------------//
        if (order.price >= core::MAX_PRICE_TICKS || order.quantity == 0 ) {
            return INVALID_HANDLE;
        }

        const std::size_t price_index =
            static_cast<std::size_t>(order.price);

        const bool isBuy = order.side == core::Side::BUY;

        PriceLevel& level = isBuy ? bids_[price_index] : asks_[price_index];
        const bool new_price_level = level.order_count == 0;

        const std::size_t node_index = allocate_node();

        if (node_index == INVALID_INDEX) {
            return INVALID_HANDLE;
        }

        //------------------Node Ops-------------------------//
        OrderNode& node = orders_pool_[node_index];

        const OrderHandle handle =
            (node.generation << SLOT_BITS) |
            static_cast<OrderHandle>(node_index);

        node.order = order;
        node.next = INVALID_INDEX;
        node.prev = level.tail;

        if (level.tail != INVALID_INDEX) {
            orders_pool_[level.tail].next = node_index;
        } else {
            level.head = node_index;
        }

        level.tail = node_index;
        level.order_count++;
        level.total_quantity += order.quantity;

        //--------------------Price activation-----------------//
        if (new_price_level) {
            const std::size_t word_index = price_index >> 6;
            const uint64_t bit = uint64_t{1} << (price_index & 63);

            if (isBuy) {
                active_bids_[word_index] |= bit;
            } else {
                active_asks_[word_index] |= bit;
            }
        }

        //-------------Best bid/ask update logic-----------------//
        if (isBuy) {
            if (best_bid_index_ == INVALID_INDEX || price_index > best_bid_index_) {
                best_bid_index_ = price_index;
            }
        } else {
            if (best_ask_index_ == INVALID_INDEX || price_index < best_ask_index_) {
                best_ask_index_ = price_index;
            }
        }

        return handle;
    }

    bool OrderBook::cancel_order(OrderHandle handle) {

        const std::size_t node_index = static_cast<std::size_t>(handle & SLOT_MASK);

        const uint64_t generation = handle >> SLOT_BITS;

        OrderNode& node = orders_pool_[node_index];

        if (node.generation != generation) {
            return false;
        }

        const std::size_t price_index = static_cast<std::size_t>(node.order.price);

        const bool is_buy = node.order.side == core::Side::BUY;

        PriceLevel& level = is_buy ? bids_[price_index] : asks_[price_index];

        if (node.prev != INVALID_INDEX) {
            orders_pool_[node.prev].next = node.next;
        } else {
            level.head = node.next;
        }

        if (node.next != INVALID_INDEX) {
            orders_pool_[node.next].prev = node.prev;
        } else {
            level.tail = node.prev;
        }

        level.order_count--;
        level.total_quantity -= node.order.quantity;

        if (level.order_count == 0) {
            const std::size_t word_index = price_index >> 6;

            const uint64_t bit = uint64_t{1} << (price_index & 63);

            if (is_buy) {
                active_bids_[word_index] &= ~bit;
            } else {
                active_asks_[word_index] &= ~bit;
            }

            free_node(node_index);

            return true;
        }
    }
}