#pragma once

#include <cmath>

int BinPow(int a, int64_t b, int c) {
    int64_t res = 1;
    int64_t cur = static_cast<int64_t>(a);

    while (b) {
        if (b & 1) {
            res = (res * cur) % c;
            res %= c;
        }
        cur = (cur * cur) % c;
        b >>= 1;
    }

    return static_cast<int>(res);
}
