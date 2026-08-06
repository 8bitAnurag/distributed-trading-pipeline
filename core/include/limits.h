//
// Created by Anurag on 06/08/2026.
//
#include <cstddef>
#ifndef DISTRIBUTED_TRADING_PIPELINE_LIMITS_H
#define DISTRIBUTED_TRADING_PIPELINE_LIMITS_H

namespace core {
    constexpr std::size_t CACHE_LINE_SIZE = 64;

    constexpr  std::size_t MAX_SYMBOL_LENGTH = 8;

    constexpr std::size_t RING_BUFFER_CAPACITY = 1 << 10;

    constexpr std::size_t MAX_PRICE_LEVELS = 65536;
}
#endif //DISTRIBUTED_TRADING_PIPELINE_LIMITS_H
