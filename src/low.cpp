#include "low.h"

int transitions(int a)
{
    int b = a >> 1;
    int c = a ^ b;
    return popcnt(c);
}

int transition_lbp(byte_t a)
{
    // shift in the sign bit as logical shift with pull in 0
    byte_t sign = (a & (1 << 7)) == 0x80 ? 0x80 : 0x00;
    byte_t b = (a >> 1) | sign;
    byte_t c = a ^ b;
/*
    bitstr(a);
    bitstr(b);
    bitstr(c);*/

    return popcnt((unsigned int)c);
}
