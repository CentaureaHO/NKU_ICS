/* This header file provides the reentrancy.  */

/* WARNING: All identifiers here must begin with an underscore.  This file is
   included by stdio.h and others and we therefore must only use identifiers
   in the namespace allotted to us.  */

#ifndef _SYS_REENT_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _SYS_REENT_H_

#include <_ansi.h>

#define __need__MAX_SIGNALS
#include <sys/signal.h> /* for _MAX_SIGNALS */

<<<<<<< HEAD
struct _glue
{
    struct _glue*   _next;
    int             _niobs;
    struct __sFILE* _iobs;
};

struct _Bigint
{
    struct _Bigint* _next;
    int             _k, _maxwds, _sign, _wds;
    struct _Bigint* _reclaim;
    unsigned long   _x[1];
=======
struct _glue {
  struct _glue *_next;
  int _niobs;
  struct __sFILE *_iobs;
};

struct _Bigint {
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  struct _Bigint *_reclaim;
  unsigned long _x[1];
>>>>>>> master
};

/*
 * atexit() support
 */

<<<<<<< HEAD
#define _ATEXIT_SIZE                                        \
    32 /* must be at least 32 to guarantee ANSI conformance \
          */

struct _atexit
{
    struct _atexit* _next;        /* next in list */
    int             _ind;         /* next index in this table */
    void (*_fns[_ATEXIT_SIZE])(); /* the table itself */
=======
#define _ATEXIT_SIZE                                                           \
  32 /* must be at least 32 to guarantee ANSI conformance                      \
        */

struct _atexit {
  struct _atexit *_next;        /* next in list */
  int _ind;                     /* next index in this table */
  void (*_fns[_ATEXIT_SIZE])(); /* the table itself */
>>>>>>> master
};

/*
 * Stdio buffers.
 *
 * This and __sFILE are defined here because we need them for struct _reent,
 * but we don't want stdio.h included when stdlib.h is.
 */

<<<<<<< HEAD
struct __sbuf
{
    unsigned char* _base;
    int            _size;
=======
struct __sbuf {
  unsigned char *_base;
  int _size;
>>>>>>> master
};

/*
 * We need fpos_t for the following, but it doesn't have a leading "_",
 * so we use _fpos_t instead.
 */

typedef long _fpos_t; /* XXX must match off_t in <sys/types.h> */
                      /* (and must be `long' for now) */

/*
 * Stdio state variables.
 *
 * The following always hold:
 *
 *	if (_flags&(__SLBF|__SWR)) == (__SLBF|__SWR),
 *		_lbfsize is -_bf._size, else _lbfsize is 0
 *	if _flags&__SRD, _w is 0
 *	if _flags&__SWR, _r is 0
 *
 * This ensures that the getc and putc macros (or inline functions) never
 * try to write or read from a file that is in `read' or `write' mode.
 * (Moreover, they can, and do, automatically switch from read mode to
 * write mode, and back, on "r+" and "w+" files.)
 *
 * _lbfsize is used only to make the inline line-buffered output stream
 * code as compact as possible.
 *
 * _ub, _up, and _ur are used when ungetc() pushes back more characters
 * than fit in the current _bf, or when ungetc() pushes back a character
 * that does not match the previous one in _bf.  When this happens,
 * _ub._base becomes non-nil (i.e., a stream has ungetc() data iff
 * _ub._base!=NULL) and _up and _ur save the current values of _p and _r.
 */

<<<<<<< HEAD
struct __sFILE
{
    unsigned char* _p;       /* current position in (some) buffer */
    int            _r;       /* read space left for getc() */
    int            _w;       /* write space left for putc() */
    short          _flags;   /* flags, below; this FILE is free if 0 */
    short          _file;    /* fileno, if Unix descriptor, else -1 */
    struct __sbuf  _bf;      /* the buffer (at least 1 byte, if !NULL) */
    int            _lbfsize; /* 0 or -_bf._size, for inline putc */

    /* operations */
    _PTR _cookie; /* cookie passed to io functions */

    int _EXFUN((*_read), (_PTR _cookie, char* _buf, int _n));
    int _EXFUN((*_write), (_PTR _cookie, const char* _buf, int _n));
    _fpos_t _EXFUN((*_seek), (_PTR _cookie, _fpos_t _offset, int _whence));
    int _EXFUN((*_close), (_PTR _cookie));

    /* separate buffer for long sequences of ungetc() */
    struct __sbuf  _ub; /* ungetc buffer */
    unsigned char* _up; /* saved _p when _p is doing ungetc data */
    int            _ur; /* saved _r when _r is counting ungetc data */

    /* tricks to meet minimum requirements even when malloc() fails */
    unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
    unsigned char _nbuf[1]; /* guarantee a getc() buffer */

    /* separate buffer for fgetline() when line crosses buffer boundary */
    struct __sbuf _lb; /* buffer for fgetline() */

    /* Unix stdio files get aligned to block boundaries on fseek() */
    int _blksize; /* stat.st_blksize (may be != _bf._size) */
    int _offset;  /* current lseek offset */
=======
struct __sFILE {
  unsigned char *_p; /* current position in (some) buffer */
  int _r;            /* read space left for getc() */
  int _w;            /* write space left for putc() */
  short _flags;      /* flags, below; this FILE is free if 0 */
  short _file;       /* fileno, if Unix descriptor, else -1 */
  struct __sbuf _bf; /* the buffer (at least 1 byte, if !NULL) */
  int _lbfsize;      /* 0 or -_bf._size, for inline putc */

  /* operations */
  _PTR _cookie; /* cookie passed to io functions */

  int _EXFUN((*_read), (_PTR _cookie, char *_buf, int _n));
  int _EXFUN((*_write), (_PTR _cookie, const char *_buf, int _n));
  _fpos_t _EXFUN((*_seek), (_PTR _cookie, _fpos_t _offset, int _whence));
  int _EXFUN((*_close), (_PTR _cookie));

  /* separate buffer for long sequences of ungetc() */
  struct __sbuf _ub;  /* ungetc buffer */
  unsigned char *_up; /* saved _p when _p is doing ungetc data */
  int _ur;            /* saved _r when _r is counting ungetc data */

  /* tricks to meet minimum requirements even when malloc() fails */
  unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
  unsigned char _nbuf[1]; /* guarantee a getc() buffer */

  /* separate buffer for fgetline() when line crosses buffer boundary */
  struct __sbuf _lb; /* buffer for fgetline() */

  /* Unix stdio files get aligned to block boundaries on fseek() */
  int _blksize; /* stat.st_blksize (may be != _bf._size) */
  int _offset;  /* current lseek offset */
>>>>>>> master

    struct _reent* _data;
};

/*
 * struct _reent
 *
 * This structure contains *all* globals needed by the library.
 * It's raison d'etre is to facilitate threads by making all library routines
 * reentrant.  IE: All state information is contained here.
 */

<<<<<<< HEAD
struct _reent
{
    /* local copy of errno */
    int _errno;
=======
struct _reent {
  /* local copy of errno */
  int _errno;
>>>>>>> master

    /* FILE is a big struct and may change over time.  To try to achieve binary
       compatibility with future versions, put stdin,stdout,stderr here.
       These are pointers into member __sf defined below.  */
    struct __sFILE *_stdin, *_stdout, *_stderr;

<<<<<<< HEAD
    char* _scanpoint;   /* used by strtok */
    char  _asctime[26]; /* used by asctime */
    long  _next;        /* used by rand/srand */
    int   _inc;         /* used by tmpnam */
    char  _emergency[25];

    int          _current_category; /* used by setlocale */
    _CONST char* _current_locale;

    int __sdidinit; /* 1 means stdio has been init'd */
=======
  char *_scanpoint;  /* used by strtok */
  char _asctime[26]; /* used by asctime */
  long _next;        /* used by rand/srand */
  int _inc;          /* used by tmpnam */
  char _emergency[25];

  int _current_category; /* used by setlocale */
  _CONST char *_current_locale;

  int __sdidinit; /* 1 means stdio has been init'd */

  void _EXFUN((*__cleanup), (struct _reent *));
>>>>>>> master

    void _EXFUN((*__cleanup), (struct _reent*));

    /* used by mprec routines */
    struct _Bigint*  _result;
    int              _result_k;
    struct _Bigint*  _p5s;
    struct _Bigint** _freelist;

<<<<<<< HEAD
    /* used by gamma */
    int _signgam;

    /* used by some fp conversion routines */
    int   _cvtlen; /* should be size_t */
    char* _cvtbuf;

/* used by malloc */
#define _N_LISTS 30
    unsigned char* _nextf[_N_LISTS];
    unsigned int   _nmalloc[_N_LISTS];

    /* atexit stuff */
    struct _atexit* _atexit;  /* points to head of LIFO stack */
    struct _atexit  _atexit0; /* one guaranteed table, required by ANSI */

    /* These are here last so that __sFILE can grow without changing the offsets
       of the above members (on the off chance that future binary compatibility
       would be broken otherwise).  */
    struct _glue   __sglue; /* root of glue chain */
    struct __sFILE __sf[3]; /* first three file descriptors */

    /* signal info */
    void (**(_sig_func))();
};

#define _REENT_INIT(var)                                       \
    {                                                          \
        0, &var.__sf[0], &var.__sf[1], &var.__sf[2], 0, {0}, 1 \
    }
=======
  /* used by some fp conversion routines */
  int _cvtlen; /* should be size_t */
  char *_cvtbuf;

/* used by malloc */
#define _N_LISTS 30
  unsigned char *_nextf[_N_LISTS];
  unsigned int _nmalloc[_N_LISTS];

  /* atexit stuff */
  struct _atexit *_atexit; /* points to head of LIFO stack */
  struct _atexit _atexit0; /* one guaranteed table, required by ANSI */

  /* These are here last so that __sFILE can grow without changing the offsets
     of the above members (on the off chance that future binary compatibility
     would be broken otherwise).  */
  struct _glue __sglue;   /* root of glue chain */
  struct __sFILE __sf[3]; /* first three file descriptors */

  /* signal info */
  void (**(_sig_func))();
};

#define _REENT_INIT(var)                                                       \
  { 0, &var.__sf[0], &var.__sf[1], &var.__sf[2], 0, {0}, 1 }
>>>>>>> master

/*
 * All references to struct _reent are via this pointer.
 * Internally, newlib routines that need to reference it should use _REENT.
 */

extern struct _reent* _impure_ptr;

<<<<<<< HEAD
void _reclaim_reent _PARAMS((struct _reent*));
=======
void _reclaim_reent _PARAMS((struct _reent *));
>>>>>>> master

/* #define _REENT_ONLY define this to get only reentrant routines */

#ifndef _REENT_ONLY
#define _REENT _impure_ptr
#endif

#ifdef __cplusplus
}
#endif
#endif /* _SYS_REENT_H_ */
