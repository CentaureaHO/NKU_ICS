#include "trap.h"

char to_lower_case(char c) { return (c >= 'A' && c <= 'Z' ? (c + 32) : c); }

<<<<<<< HEAD
char ans[] = {0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    117,
    118,
    119,
    120,
    121,
    122,
    91,
    92,
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    117,
    118,
    119,
    120,
    121,
    122,
    123,
    124,
    125,
    126,
    127};

int main()
{
    int i;
    for (i = 0; i < 128; i++) {
        nemu_assert(to_lower_case(i) == ans[i]);
    }

    return 0;
=======
char ans[] = {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,
              13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,
              26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
              39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,
              52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,
              97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
              110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
              91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103,
              104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
              117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

int main() {
  int i;
  for (i = 0; i < 128; i++) {
    nemu_assert(to_lower_case(i) == ans[i]);
  }

  return 0;
>>>>>>> master
}
