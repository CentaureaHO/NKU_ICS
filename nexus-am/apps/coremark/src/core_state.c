/*
Author : Shay Gal-On, EEMBC

This file is part of  EEMBC(R) and CoreMark(TM), which are Copyright (C) 2009
All rights reserved.

EEMBC CoreMark Software is a product of EEMBC and is provided under the terms of
the
CoreMark License that is distributed with the official EEMBC COREMARK Software
release.
If you received this EEMBC CoreMark Software without the accompanying CoreMark
License,
you must discontinue use and download the official release from
www.coremark.org.

Also, if you are publicly displaying scores generated from the EEMBC CoreMark
software,
make sure that you are in compliance with Run and Reporting rules specified in
the accompanying readme.txt file.

EEMBC
4354 Town Center Blvd. Suite 114-200
El Dorado Hills, CA, 95762
*/
#include "coremark.h"
/* local functions */
<<<<<<< HEAD
enum CORE_STATE core_state_transition(ee_u8** instr, ee_u32* transition_count);
=======
enum CORE_STATE core_state_transition(ee_u8 **instr, ee_u32 *transition_count);
>>>>>>> master

/*
Topic: Description
        Simple state machines like this one are used in many embedded products.

        For more complex state machines, sometimes a state transition table
implementation is used instead,
        trading speed of direct coding for ease of maintenance.

        Since the main goal of using a state machine in CoreMark is to excercise
the switch/if behaviour,
        we are using a small moore machine.

        In particular, this machine tests type of string input,
        trying to determine whether the input is a number or something else.
        (see core_state.png).
*/

/* Function: core_bench_state
        Benchmark function

        Go over the input twice, once direct, and once after introducing some
   corruption.
*/
<<<<<<< HEAD
ee_u16 core_bench_state(ee_u32 blksize, ee_u8* memblock, ee_s16 seed1, ee_s16 seed2, ee_s16 step, ee_u16 crc)
{
    ee_u32 final_counts[NUM_CORE_STATES];
    ee_u32 track_counts[NUM_CORE_STATES];
    ee_u8* p = memblock;
    ee_u32 i;

#if CORE_DEBUG
    ee_printf("State Bench: %d,%d,%d,%04x\n", seed1, seed2, step, crc);
#endif
    for (i = 0; i < NUM_CORE_STATES; i++) {
        final_counts[i] = track_counts[i] = 0;
    }
    /* run the state machine over the input */
    while (*p != 0) {
        enum CORE_STATE fstate = core_state_transition(&p, track_counts);
        final_counts[fstate]++;
#if CORE_DEBUG
        ee_printf("%d,", fstate);
    }
    ee_printf("\n");
#else
    }
#endif
    p = memblock;
    while (p < (memblock + blksize)) { /* insert some corruption */
        if (*p != ',') *p ^= (ee_u8)seed1;
        p += step;
    }
    p = memblock;
    /* run the state machine over the input again */
    while (*p != 0) {
        enum CORE_STATE fstate = core_state_transition(&p, track_counts);
        final_counts[fstate]++;
#if CORE_DEBUG
        ee_printf("%d,", fstate);
    }
    ee_printf("\n");
#else
    }
#endif
    p = memblock;
    while (p < (memblock + blksize)) { /* undo corruption is seed1 and seed2 are equal */
        if (*p != ',') *p ^= (ee_u8)seed2;
        p += step;
    }
    /* end timing */
    for (i = 0; i < NUM_CORE_STATES; i++) {
        crc = crcu32(final_counts[i], crc);
        crc = crcu32(track_counts[i], crc);
    }
    return crc;
}

/* Default initialization patterns */
static ee_u8* intpat[4]   = {(ee_u8*)"5012", (ee_u8*)"1234", (ee_u8*)"-874", (ee_u8*)"+122"};
static ee_u8* floatpat[4] = {(ee_u8*)"35.54400", (ee_u8*)".1234500", (ee_u8*)"-110.700", (ee_u8*)"+0.64400"};
static ee_u8* scipat[4]   = {(ee_u8*)"5.500e+3", (ee_u8*)"-.123e-2", (ee_u8*)"-87e+832", (ee_u8*)"+0.6e-12"};
static ee_u8* errpat[4]   = {(ee_u8*)"T0.3e-1F", (ee_u8*)"-T.T++Tq", (ee_u8*)"1T3.4e4z", (ee_u8*)"34.0e-T^"};
=======
ee_u16 core_bench_state(ee_u32 blksize, ee_u8 *memblock, ee_s16 seed1,
                        ee_s16 seed2, ee_s16 step, ee_u16 crc) {
  ee_u32 final_counts[NUM_CORE_STATES];
  ee_u32 track_counts[NUM_CORE_STATES];
  ee_u8 *p = memblock;
  ee_u32 i;

#if CORE_DEBUG
  ee_printf("State Bench: %d,%d,%d,%04x\n", seed1, seed2, step, crc);
#endif
  for (i = 0; i < NUM_CORE_STATES; i++) {
    final_counts[i] = track_counts[i] = 0;
  }
  /* run the state machine over the input */
  while (*p != 0) {
    enum CORE_STATE fstate = core_state_transition(&p, track_counts);
    final_counts[fstate]++;
#if CORE_DEBUG
    ee_printf("%d,", fstate);
  }
  ee_printf("\n");
#else
  }
#endif
  p = memblock;
  while (p < (memblock + blksize)) { /* insert some corruption */
    if (*p != ',')
      *p ^= (ee_u8)seed1;
    p += step;
  }
  p = memblock;
  /* run the state machine over the input again */
  while (*p != 0) {
    enum CORE_STATE fstate = core_state_transition(&p, track_counts);
    final_counts[fstate]++;
#if CORE_DEBUG
    ee_printf("%d,", fstate);
  }
  ee_printf("\n");
#else
  }
#endif
  p = memblock;
  while (p < (memblock +
              blksize)) { /* undo corruption is seed1 and seed2 are equal */
    if (*p != ',')
      *p ^= (ee_u8)seed2;
    p += step;
  }
  /* end timing */
  for (i = 0; i < NUM_CORE_STATES; i++) {
    crc = crcu32(final_counts[i], crc);
    crc = crcu32(track_counts[i], crc);
  }
  return crc;
}

/* Default initialization patterns */
static ee_u8 *intpat[4] = {(ee_u8 *)"5012", (ee_u8 *)"1234", (ee_u8 *)"-874",
                           (ee_u8 *)"+122"};
static ee_u8 *floatpat[4] = {(ee_u8 *)"35.54400", (ee_u8 *)".1234500",
                             (ee_u8 *)"-110.700", (ee_u8 *)"+0.64400"};
static ee_u8 *scipat[4] = {(ee_u8 *)"5.500e+3", (ee_u8 *)"-.123e-2",
                           (ee_u8 *)"-87e+832", (ee_u8 *)"+0.6e-12"};
static ee_u8 *errpat[4] = {(ee_u8 *)"T0.3e-1F", (ee_u8 *)"-T.T++Tq",
                           (ee_u8 *)"1T3.4e4z", (ee_u8 *)"34.0e-T^"};
>>>>>>> master

/* Function: core_init_state
        Initialize the input data for the state machine.

        Populate the input with several predetermined strings, interspersed.
        Actual patterns chosen depend on the seed parameter.

        Note:
        The seed parameter MUST be supplied from a source that cannot be
   determined at compile time
*/
<<<<<<< HEAD
void core_init_state(ee_u32 size, ee_s16 seed, ee_u8* p)
{
    ee_u32 total = 0, next = 0, i;
    ee_u8* buf = 0;
#if CORE_DEBUG
    ee_u8* start = p;
    ee_printf("State: %d,%d\n", size, seed);
#endif
    size--;
    next = 0;
    while ((total + next + 1) < size) {
        if (next > 0) {
            for (i = 0; i < next; i++) *(p + total + i) = buf[i];
            *(p + total + i)                            = ',';
            total += next + 1;
        }
        seed++;
        switch (seed & 0x7)
        {
            case 0: /* int */
            case 1: /* int */
            case 2: /* int */
                buf  = intpat[(seed >> 3) & 0x3];
                next = 4;
                break;
            case 3: /* float */
            case 4: /* float */
                buf  = floatpat[(seed >> 3) & 0x3];
                next = 8;
                break;
            case 5: /* scientific */
            case 6: /* scientific */
                buf  = scipat[(seed >> 3) & 0x3];
                next = 8;
                break;
            case 7: /* invalid */
                buf  = errpat[(seed >> 3) & 0x3];
                next = 8;
                break;
            default: /* Never happen, just to make some compilers happy */ break;
        }
    }
    size++;
    while (total < size) { /* fill the rest with 0 */
        *(p + total) = 0;
        total++;
    }
#if CORE_DEBUG
    ee_printf("State Input: %s\n", start);
#endif
}

static ee_u8 ee_isdigit(ee_u8 c)
{
    ee_u8 retval;
    retval = ((c >= '0') & (c <= '9')) ? 1 : 0;
    return retval;
=======
void core_init_state(ee_u32 size, ee_s16 seed, ee_u8 *p) {
  ee_u32 total = 0, next = 0, i;
  ee_u8 *buf = 0;
#if CORE_DEBUG
  ee_u8 *start = p;
  ee_printf("State: %d,%d\n", size, seed);
#endif
  size--;
  next = 0;
  while ((total + next + 1) < size) {
    if (next > 0) {
      for (i = 0; i < next; i++)
        *(p + total + i) = buf[i];
      *(p + total + i) = ',';
      total += next + 1;
    }
    seed++;
    switch (seed & 0x7) {
    case 0: /* int */
    case 1: /* int */
    case 2: /* int */
      buf = intpat[(seed >> 3) & 0x3];
      next = 4;
      break;
    case 3: /* float */
    case 4: /* float */
      buf = floatpat[(seed >> 3) & 0x3];
      next = 8;
      break;
    case 5: /* scientific */
    case 6: /* scientific */
      buf = scipat[(seed >> 3) & 0x3];
      next = 8;
      break;
    case 7: /* invalid */
      buf = errpat[(seed >> 3) & 0x3];
      next = 8;
      break;
    default: /* Never happen, just to make some compilers happy */
      break;
    }
  }
  size++;
  while (total < size) { /* fill the rest with 0 */
    *(p + total) = 0;
    total++;
  }
#if CORE_DEBUG
  ee_printf("State Input: %s\n", start);
#endif
}

static ee_u8 ee_isdigit(ee_u8 c) {
  ee_u8 retval;
  retval = ((c >= '0') & (c <= '9')) ? 1 : 0;
  return retval;
>>>>>>> master
}

/* Function: core_state_transition
        Actual state machine.

        The state machine will continue scanning until either:
        1 - an invalid input is detcted.
        2 - a valid number has been detected.

        The input pointer is updated to point to the end of the token, and the
   end state is returned (either specific format determined or invalid).
*/

<<<<<<< HEAD
enum CORE_STATE core_state_transition(ee_u8** instr, ee_u32* transition_count)
{
    ee_u8*          str = *instr;
    ee_u8           NEXT_SYMBOL;
    enum CORE_STATE state = CORE_START;
    for (; *str && state != CORE_INVALID; str++) {
        NEXT_SYMBOL = *str;
        if (NEXT_SYMBOL == ',') /* end of this input */
        {
            str++;
            break;
        }
        switch (state)
        {
            case CORE_START:
                if (ee_isdigit(NEXT_SYMBOL)) {
                    state = CORE_INT;
                }
                else if (NEXT_SYMBOL == '+' || NEXT_SYMBOL == '-')
                {
                    state = CORE_S1;
                }
                else if (NEXT_SYMBOL == '.')
                {
                    state = CORE_FLOAT;
                }
                else
                {
                    state = CORE_INVALID;
                    transition_count[CORE_INVALID]++;
                }
                transition_count[CORE_START]++;
                break;
            case CORE_S1:
                if (ee_isdigit(NEXT_SYMBOL)) {
                    state = CORE_INT;
                    transition_count[CORE_S1]++;
                }
                else if (NEXT_SYMBOL == '.')
                {
                    state = CORE_FLOAT;
                    transition_count[CORE_S1]++;
                }
                else
                {
                    state = CORE_INVALID;
                    transition_count[CORE_S1]++;
                }
                break;
            case CORE_INT:
                if (NEXT_SYMBOL == '.') {
                    state = CORE_FLOAT;
                    transition_count[CORE_INT]++;
                }
                else if (!ee_isdigit(NEXT_SYMBOL))
                {
                    state = CORE_INVALID;
                    transition_count[CORE_INT]++;
                }
                break;
            case CORE_FLOAT:
                if (NEXT_SYMBOL == 'E' || NEXT_SYMBOL == 'e') {
                    state = CORE_S2;
                    transition_count[CORE_FLOAT]++;
                }
                else if (!ee_isdigit(NEXT_SYMBOL))
                {
                    state = CORE_INVALID;
                    transition_count[CORE_FLOAT]++;
                }
                break;
            case CORE_S2:
                if (NEXT_SYMBOL == '+' || NEXT_SYMBOL == '-') {
                    state = CORE_EXPONENT;
                    transition_count[CORE_S2]++;
                }
                else
                {
                    state = CORE_INVALID;
                    transition_count[CORE_S2]++;
                }
                break;
            case CORE_EXPONENT:
                if (ee_isdigit(NEXT_SYMBOL)) {
                    state = CORE_SCIENTIFIC;
                    transition_count[CORE_EXPONENT]++;
                }
                else
                {
                    state = CORE_INVALID;
                    transition_count[CORE_EXPONENT]++;
                }
                break;
            case CORE_SCIENTIFIC:
                if (!ee_isdigit(NEXT_SYMBOL)) {
                    state = CORE_INVALID;
                    transition_count[CORE_INVALID]++;
                }
                break;
            default: break;
        }
    }
    *instr = str;
    return state;
=======
enum CORE_STATE core_state_transition(ee_u8 **instr, ee_u32 *transition_count) {
  ee_u8 *str = *instr;
  ee_u8 NEXT_SYMBOL;
  enum CORE_STATE state = CORE_START;
  for (; *str && state != CORE_INVALID; str++) {
    NEXT_SYMBOL = *str;
    if (NEXT_SYMBOL == ',') /* end of this input */
    {
      str++;
      break;
    }
    switch (state) {
    case CORE_START:
      if (ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_INT;
      } else if (NEXT_SYMBOL == '+' || NEXT_SYMBOL == '-') {
        state = CORE_S1;
      } else if (NEXT_SYMBOL == '.') {
        state = CORE_FLOAT;
      } else {
        state = CORE_INVALID;
        transition_count[CORE_INVALID]++;
      }
      transition_count[CORE_START]++;
      break;
    case CORE_S1:
      if (ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_INT;
        transition_count[CORE_S1]++;
      } else if (NEXT_SYMBOL == '.') {
        state = CORE_FLOAT;
        transition_count[CORE_S1]++;
      } else {
        state = CORE_INVALID;
        transition_count[CORE_S1]++;
      }
      break;
    case CORE_INT:
      if (NEXT_SYMBOL == '.') {
        state = CORE_FLOAT;
        transition_count[CORE_INT]++;
      } else if (!ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_INVALID;
        transition_count[CORE_INT]++;
      }
      break;
    case CORE_FLOAT:
      if (NEXT_SYMBOL == 'E' || NEXT_SYMBOL == 'e') {
        state = CORE_S2;
        transition_count[CORE_FLOAT]++;
      } else if (!ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_INVALID;
        transition_count[CORE_FLOAT]++;
      }
      break;
    case CORE_S2:
      if (NEXT_SYMBOL == '+' || NEXT_SYMBOL == '-') {
        state = CORE_EXPONENT;
        transition_count[CORE_S2]++;
      } else {
        state = CORE_INVALID;
        transition_count[CORE_S2]++;
      }
      break;
    case CORE_EXPONENT:
      if (ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_SCIENTIFIC;
        transition_count[CORE_EXPONENT]++;
      } else {
        state = CORE_INVALID;
        transition_count[CORE_EXPONENT]++;
      }
      break;
    case CORE_SCIENTIFIC:
      if (!ee_isdigit(NEXT_SYMBOL)) {
        state = CORE_INVALID;
        transition_count[CORE_INVALID]++;
      }
      break;
    default:
      break;
    }
  }
  *instr = str;
  return state;
>>>>>>> master
}
