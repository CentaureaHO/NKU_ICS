#ifndef _SYS_TIMES_H
#ifdef __cplusplus
extern "C" {
#endif
#define _SYS_TIMES_H

#include <_ansi.h>
#include <machine/types.h>

#ifndef __clock_t_defined
typedef _CLOCK_T_ clock_t;
#define __clock_t_defined
#endif

<<<<<<< HEAD
struct tms
{
    clock_t tms_utime;  /* user time */
    clock_t tms_stime;  /* system time */
    clock_t tms_cutime; /* user time, children */
    clock_t tms_cstime; /* system time, children */
};

clock_t _EXFUN(times, (struct tms*));
=======
struct tms {
  clock_t tms_utime;  /* user time */
  clock_t tms_stime;  /* system time */
  clock_t tms_cutime; /* user time, children */
  clock_t tms_cstime; /* system time, children */
};

clock_t _EXFUN(times, (struct tms *));
>>>>>>> master

#ifdef __cplusplus
}
#endif
#endif /* !_SYS_TIMES_H */
