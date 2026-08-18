//
// Created by Anurag on 15/08/2026.
//

#include "../include/order_book.h"

namespace engine {
     bool OrderBook::add_order(const core::Order &order) {

         //-------------Order validation , BUY/SELL , free node creation ---------------//
         if (order.price >= core::MAX_PRICE_TICKS || order.quantity == 0 ) {
             return false;
         }
         const std::size_t price_index =
             static_cast<std::size_t>(order.price);

         const bool isBuy = order.side == core::Side::BUY;

         PriceLevel& level = isBuy? bids_[price_index] : asks_[price_index];
         const bool new_price_level = level.order_count == 0;

         const std::size_t free_node_index = allocate_node();
         if (free_node_index == INVALID_INDEX) return false;

         //------------------Node Ops-------------------------//
         OrderNode& node = orders_pool_[free_node_index];
         node.order = order;
         node.next = INVALID_INDEX;
         node.prev = level.tail;

         if (level.tail != INVALID_INDEX) {
             orders_pool_[level.tail].next = free_node_index;
         } else {
             level.head = free_node_index;
         }
         level.tail = free_node_index;
         level.order_count++;
         level.total_quantity += order.quantity;

         if (new_price_level) {
             const std::size_t word_index = price_index >> 6;
             const uint64_t bit = uint64_t{1} << (price_index & 63);

             if (isBuy) {
                 active_bids_[word_index] |= bit;
             } else {
                 active_asks_[word_index] |= bit;
             }
         }

         if (isBuy) {
             if (best_bid_index_ == INVALID_INDEX || price_index > best_bid_index_) {
                 best_bid_index_ = price_index;
             }
         } else {
             if (best_ask_index_ == INVALID_INDEX || price_index < best_ask_index_) {
                 best_ask_index_ = price_index;
             }
         }
         return true;
    }

}