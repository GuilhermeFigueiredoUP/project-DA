#include "bitwise.h"
#include <stdarg.h>

uint8_t clear(uint8_t msk, int pos) {
    return msk & ~(1U << pos);
}

uint8_t set(uint8_t msk, int pos) {
    return msk | (1U << pos);
}

bool is_set(uint8_t msk, int pos) {
    return (msk & (1U << pos)) != 0;
}

uint8_t lsb(uint16_t wide_msk) {
    return (uint8_t)(wide_msk & 0xFF);
}


uint8_t msb(uint16_t wide_msk) {
    return (uint8_t)(wide_msk >> 8);
}

uint8_t mask(int pos, ...) {
    uint8_t res = 0;
    va_list args;
    
    va_start(args, pos);
    int current_pos = pos;

    while (current_pos != MSK_END) {
        res |= (1U << current_pos);
        current_pos = va_arg(args, int);
    }

    va_end(args);
    return res;
}