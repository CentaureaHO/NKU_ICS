#include "trap.h"

int main()
{
    long long a = 0, b = 2;
    long long c = 0xfffffffffffffffeLL;

    nemu_assert((a - b) == c);
}