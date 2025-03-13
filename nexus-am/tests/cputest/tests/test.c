#include "trap.h"

long long sub(long long a, long long b)
{
    long long c = a - b;
    return c;
}

long long test_data[] = {0,
    1,
    2,
    0x7fffffffffffffffLL,
    0x8000000000000000LL,
    0x8000000000000001LL,
    0xfffffffffffffffeLL,
    0xffffffffffffffffLL};
long long ans[] = {0LL,
    0xffffffffffffffffLL,
    0xfffffffffffffffeLL,
    0x8000000000000001LL,
    0x8000000000000000LL,
    0x7fffffffffffffffLL,
    0x2LL,
    0x1LL,
    0x1LL,
    0LL,
    0xffffffffffffffffLL,
    0x8000000000000002LL,
    0x8000000000000001LL,
    0x8000000000000000LL,
    0x3LL,
    0x2LL,
    0x2LL,
    0x1LL,
    0LL,
    0x8000000000000003LL,
    0x8000000000000002LL,
    0x8000000000000001LL,
    0x4LL,
    0x3LL,
    0x7fffffffffffffffLL,
    0x7ffffffffffffffeLL,
    0x7ffffffffffffffdLL,
    0LL,
    0xffffffffffffffffLL,
    0xfffffffffffffffeLL,
    0x8000000000000001LL,
    0x8000000000000000LL,
    0x8000000000000000LL,
    0x7fffffffffffffffLL,
    0x7ffffffffffffffeLL,
    0x1LL,
    0LL,
    0xffffffffffffffffLL,
    0x8000000000000002LL,
    0x8000000000000001LL,
    0x8000000000000001LL,
    0x8000000000000000LL,
    0x7fffffffffffffffLL,
    0x2LL,
    0x1LL,
    0LL,
    0x8000000000000003LL,
    0x8000000000000002LL,
    0xfffffffffffffffeLL,
    0xfffffffffffffffdLL,
    0xfffffffffffffffcLL,
    0x7fffffffffffffffLL,
    0x7ffffffffffffffeLL,
    0x7ffffffffffffffdLL,
    0LL,
    0xffffffffffffffffLL,
    0xffffffffffffffffLL,
    0xfffffffffffffffeLL,
    0xfffffffffffffffdLL,
    0x8000000000000000LL,
    0x7fffffffffffffffLL,
    0x7ffffffffffffffeLL,
    0x1LL,
    0LL};

#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))

int main()
{

    nemu_assert(sub(0, 1) == 1);
}