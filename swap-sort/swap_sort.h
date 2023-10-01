#pragma once

void Swap(int* a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void Sort3(int* a, int* b, int* c) {
    if (*a > *b) {
        Swap(a, b);
    }
    if (*b > *c) {
        if (*a > *c) {
            Swap(a, c);
        }
        Swap(b, c);
    }
}
