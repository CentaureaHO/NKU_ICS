#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common.h"

<<<<<<< HEAD
#define Log(format, ...) printk("\33[1;35m[%s,%d,%s] " format "\33[0m\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define panic(format, ...)                                    \
    do                                                        \
    {                                                         \
        Log("\33[1;31msystem panic: " format, ##__VA_ARGS__); \
        _halt(1);                                             \
    } while (0)
=======
#define NDEBUG
#ifndef NDEBUG
#define Log(format, ...)                                                       \
  printk("\33[1;35m[%s,%d,%s] " format "\33[0m\n", __FILE__, __LINE__,         \
         __func__, ##__VA_ARGS__)

#define panic(format, ...)                                                     \
  do {                                                                         \
    Log("\33[1;31msystem panic: " format, ##__VA_ARGS__);                      \
    _halt(1);                                                                  \
  } while (0)
>>>>>>> master

#ifdef assert
#undef assert
#endif

<<<<<<< HEAD
#define assert(cond)                              \
    do                                            \
    {                                             \
        if (!(cond)) {                            \
            panic("Assertion failed: %s", #cond); \
        }                                         \
    } while (0)
=======
#define assert(cond)                                                           \
  do {                                                                         \
    if (!(cond)) {                                                             \
      panic("Assertion failed: %s", #cond);                                    \
    }                                                                          \
  } while (0)
>>>>>>> master

#define TODO() panic("please implement me")
#else

#define Log(format, ...)                                                       \
  do {                                                                         \
  } while (0)
#define panic(format, ...)                                                     \
  do {                                                                         \
  } while (0)
#define assert(cond)                                                           \
  do {                                                                         \
  } while (0)
#define TODO()                                                                 \
  do {                                                                         \
  } while (0)

#endif

#endif
