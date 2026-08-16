//
// Created by Anurag on 14/08/2026.
//

#ifndef DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
#define DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
#include <array>
#include <cstddef>
#include <limits>
#include <cstdint>
#include "../../core/include/trading_limits.h"
#include "../../core/include/order.h"

namespace engine {
    constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

    struct PriceLevel {
        std::size_t head{INVALID_INDEX};
        std::size_t tail{INVALID_INDEX};

        uint64_t total_quantity{0};
        uint32_t order_count{0};
    };
    struct OrderNode {
        core::Order order{};

        std::size_t next{INVALID_INDEX};
        std::size_t prev{INVALID_INDEX};
    };

    class OrderBook {
    private:
        static constexpr std::size_t BITMAP_WORDS = (core::MAX_PRICE_TICKS + 63) / 64;

        std::array<PriceLevel, core::MAX_PRICE_TICKS> bids_{};
        std::array<PriceLevel, core::MAX_PRICE_TICKS> asks_{};

        std::array<OrderNode, core::MAX_ACTIVE_ORDERS> orders_pool_{};
        std::array<std::size_t, core::MAX_ACTIVE_ORDERS> free_indices_{};
        std::size_t free_indices_count_{core::MAX_ACTIVE_ORDERS};

        std::array<uint64_t, BITMAP_WORDS> active_bids_{};
        std::array<uint64_t, BITMAP_WORDS> active_asks_{};

        std::size_t best_bid_index_{INVALID_INDEX};
        std::size_t best_ask_index_{INVALID_INDEX};

        std::size_t allocate_node();

    public:
        OrderBook() {
            for (std::size_t i = 0; i < core::MAX_ACTIVE_ORDERS; ++i) {
                free_indices_[i] = core::MAX_ACTIVE_ORDERS - 1 - i;
            }
        }
        bool add_order(const core::Order& order);
        bool cancel_order(uint64_t order_id);

        const PriceLevel* best_bid() const; // Highest number
        const PriceLevel* best_ask() const; // Lowest number
    };
}
#endif //DISTRIBUTED_TRADING_PIPELINE_ORDER_BOOK_H
