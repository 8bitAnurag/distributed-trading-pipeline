//
// Created by Anurag on 06/08/2026.
//
#pragma once
#include <cstddef>
#ifndef DISTRIBUTED_TRADING_PIPELINE_LIMITS_H
#define DISTRIBUTED_TRADING_PIPELINE_LIMITS_H

namespace core {
    constexpr std::size_t CACHE_LINE_SIZE = 64;

    constexpr  std::size_t MAX_SYMBOL_LENGTH = 8;

    constexpr std::size_t RING_BUFFER_CAPACITY = 1 << 10;

    constexpr std::size_t MAX_PRICE_LEVELS = 100'000;

    constexpr std::size_t MAX_ORDERS_PER_PRICE_LEVEL = 1024;

    constexpr std::size_t MAX_PRICE_TICKS = 100'000;

    constexpr std::size_t MAX_ACTIVE_ORDERS = 262'144;
}
#endif //DISTRIBUTED_TRADING_PIPELINE_LIMITS_H
