//
// Created by Anurag on 14/08/2026.
//

#ifndef DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
#define DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
#include <array>
#include <cstddef>
#include <cstdint>
#include "../../core/include/trading_limits.h"
#include <../../core/include/order.h>

namespace engine {

    struct PriceLevel {
        uint64_t price{0};

        std::array<core::Order, core::MAX_ORDERS_PER_PRICE_LEVEL> orders{};

        std::size_t order_count{0};
        std::size_t head{0};
        std::size_t tail{0};

        uint64_t total_quantity{0};
    };

    class OrderBook {
    private:
        std::array<PriceLevel, core::MAX_PRICE_LEVELS> bids_{};
        std::array<PriceLevel, core::MAX_PRICE_LEVELS> asks_{};

        std::size_t bid_level_count_{0};
        std::size_t ask_level_count_{0};

    public:
        bool add_order(const core::Order& order);
        bool cancel_order(uint64_t order_id);
        
        const PriceLevel* best_bid() const; // Highest number
        const PriceLevel* best_ask() const; // Lowest number
    };
}
#endif //DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
