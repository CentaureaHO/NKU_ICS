#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <stdio.h>

typedef uint8_t bool;

#define true 1
#define false 0

<<<<<<< HEAD
#define Log(args...)           \
    do                         \
    {                          \
        fprintf(stderr, args); \
        fprintf(stderr, "\n"); \
    } while (0)
=======
#define Log(args...)                                                           \
  do {                                                                         \
    fprintf(stderr, args);                                                     \
    fprintf(stderr, "\n");                                                     \
  } while (0)
>>>>>>> master

void SDL_WaitUntil(uint32_t tick);

#endif
