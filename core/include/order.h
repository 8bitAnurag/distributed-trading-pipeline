//
// Created by Anurag on 07/08/2026.
//
#include <cstdint>
#include "trading_limits.h"
#include "types.h"

#ifndef DISTRIBUTED_TRADING_PIPELINE_ORDER_H
#define DISTRIBUTED_TRADING_PIPELINE_ORDER_H
namespace core {
    struct Order {
        uint64_t id;
        uint64_t price;

        char symbol[MAX_SYMBOL_LENGTH];

        uint32_t quantity;

        Side side;

        uint8_t padding[3];
    };
}
#endif //DISTRIBUTED_TRADING_PIPELINE_ORDER_H
