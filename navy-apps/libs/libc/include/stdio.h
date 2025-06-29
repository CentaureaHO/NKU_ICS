/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)stdio.h	5.3 (Berkeley) 3/15/86
 *	$Header: /rel/cvsfiles/devo/newlib/libc/include/stdio.h,v 1.21
 *1994/06/22 14:27:11 ian Exp $
 */

/*
 * NB: to fit things in six character monocase externals, the
 * stdio code uses the prefix `__s' for stdio objects, typically
 * followed by a three-character attempt at a mnemonic.
 */

#ifndef _STDIO_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _STDIO_H_

#include "_ansi.h"

#define _FSTDIO /* ``function stdio'' */

#define __need_size_t
#include <stddef.h>

#define __need___va_list
#include <stdarg.h>

/*
 * <sys/reent.h> defines __sFILE, _fpos_t.
 * They must be defined there because struct _reent needs them (and we don't
 * want reent.h to include this file.
 */

#include <sys/reent.h>

typedef _fpos_t fpos_t;

typedef struct __sFILE FILE;

#define __SLBF 0x0001 /* line buffered */
#define __SNBF 0x0002 /* unbuffered */
#define __SRD 0x0004  /* OK to read */
#define __SWR 0x0008  /* OK to write */
                      /* RD and WR are never simultaneously asserted */
#define __SRW 0x0010  /* open for reading & writing */
#define __SEOF 0x0020 /* found EOF */
#define __SERR 0x0040 /* found error */
#define __SMBF 0x0080 /* _buf is from malloc */
#define __SAPP 0x0100 /* fdopen()ed in append mode - so must  write to end */
#define __SSTR 0x0200 /* this is an sprintf/snprintf string */
#define __SOPT 0x0400 /* do fseek() optimisation */
#define __SNPT 0x0800 /* do not do fseek() optimisation */
#define __SOFF 0x1000 /* set iff _offset is in fact correct */
#define __SMOD 0x2000 /* true => fgetline modified _p text */

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which stupidly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although these happen to match their counterparts above, the
 * implementation does not rely on that (so these could be renumbered).
 */
#define _IOFBF 0 /* setvbuf should set fully buffered */
#define _IOLBF 1 /* setvbuf should set line buffered */
#define _IONBF 2 /* setvbuf should set unbuffered */

#ifndef NULL
#define NULL 0L
#endif

#define BUFSIZ 1024
#define EOF (-1)

#define FOPEN_MAX 20      /* must be <= OPEN_MAX <sys/syslimits.h> */
#define FILENAME_MAX 1024 /* must be <= PATH_MAX <sys/syslimits.h> */
#define L_tmpnam 1024     /* XXX must be == PATH_MAX */
#ifndef _STRICT_ANSI
#define P_tmpdir "/tmp"
#endif

#ifndef SEEK_SET
#define SEEK_SET 0 /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1 /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* set file offset to EOF plus offset */
#endif

#define TMP_MAX 26

#define stdin (_impure_ptr->_stdin)
#define stdout (_impure_ptr->_stdout)
#define stderr (_impure_ptr->_stderr)

#define _stdin_r(x) ((x)->_stdin)
#define _stdout_r(x) ((x)->_stdout)
#define _stderr_r(x) ((x)->_stderr)

/*
 * Functions defined in ANSI C standard.
 */

#ifdef __GNUC__
#define __VALIST __gnuc_va_list
#else
#define __VALIST char *
#endif

#ifndef _REENT_ONLY
<<<<<<< HEAD
int _EXFUN(remove, (const char*));
int _EXFUN(rename, (const char*, const char*));
#endif
FILE*  _EXFUN(tmpfile, (void));
char*  _EXFUN(tmpnam, (char*));
int    _EXFUN(fclose, (FILE*));
int    _EXFUN(fflush, (FILE*));
FILE*  _EXFUN(freopen, (const char*, const char*, FILE*));
void   _EXFUN(setbuf, (FILE*, char*));
int    _EXFUN(setvbuf, (FILE*, char*, int, size_t));
int    _EXFUN(fprintf, (FILE*, const char*, ...));
int    _EXFUN(fscanf, (FILE*, const char*, ...));
int    _EXFUN(printf, (const char*, ...));
int    _EXFUN(scanf, (const char*, ...));
int    _EXFUN(sscanf, (const char*, const char*, ...));
int    _EXFUN(vfprintf, (FILE*, const char*, __VALIST));
int    _EXFUN(vprintf, (const char*, __VALIST));
int    _EXFUN(vsprintf, (char*, const char*, __VALIST));
int    _EXFUN(fgetc, (FILE*));
char*  _EXFUN(fgets, (char*, int, FILE*));
int    _EXFUN(fputc, (int, FILE*));
int    _EXFUN(fputs, (const char*, FILE*));
int    _EXFUN(getc, (FILE*));
int    _EXFUN(getchar, (void));
char*  _EXFUN(gets, (char*));
int    _EXFUN(putc, (int, FILE*));
int    _EXFUN(putchar, (int));
int    _EXFUN(puts, (const char*));
int    _EXFUN(ungetc, (int, FILE*));
size_t _EXFUN(fread, (_PTR, size_t _size, size_t _n, FILE*));
size_t _EXFUN(fwrite, (const _PTR, size_t _size, size_t _n, FILE*));
int  _EXFUN(fgetpos, (FILE*, fpos_t*));
int  _EXFUN(fseek, (FILE*, long, int));
int  _EXFUN(fsetpos, (FILE*, const fpos_t*));
long _EXFUN(ftell, (FILE*));
void _EXFUN(rewind, (FILE*));
void _EXFUN(clearerr, (FILE*));
int  _EXFUN(feof, (FILE*));
int  _EXFUN(ferror, (FILE*));
void _EXFUN(perror, (const char*));
#ifndef _REENT_ONLY
FILE* _EXFUN(fopen, (const char* _name, const char* _type));
int _EXFUN(sprintf, (char*, const char*, ...));
#endif
#ifndef _STRICT_ANSI
int _EXFUN(vfiprintf, (FILE*, const char*, __VALIST));
int _EXFUN(iprintf, (const char*, ...));
int _EXFUN(fiprintf, (FILE*, const char*, ...));
=======
int _EXFUN(remove, (const char *));
int _EXFUN(rename, (const char *, const char *));
#endif
FILE *_EXFUN(tmpfile, (void));
char *_EXFUN(tmpnam, (char *));
int _EXFUN(fclose, (FILE *));
int _EXFUN(fflush, (FILE *));
FILE *_EXFUN(freopen, (const char *, const char *, FILE *));
void _EXFUN(setbuf, (FILE *, char *));
int _EXFUN(setvbuf, (FILE *, char *, int, size_t));
int _EXFUN(fprintf, (FILE *, const char *, ...));
int _EXFUN(fscanf, (FILE *, const char *, ...));
int _EXFUN(printf, (const char *, ...));
int _EXFUN(scanf, (const char *, ...));
int _EXFUN(sscanf, (const char *, const char *, ...));
int _EXFUN(vfprintf, (FILE *, const char *, __VALIST));
int _EXFUN(vprintf, (const char *, __VALIST));
int _EXFUN(vsprintf, (char *, const char *, __VALIST));
int _EXFUN(fgetc, (FILE *));
char *_EXFUN(fgets, (char *, int, FILE *));
int _EXFUN(fputc, (int, FILE *));
int _EXFUN(fputs, (const char *, FILE *));
int _EXFUN(getc, (FILE *));
int _EXFUN(getchar, (void));
char *_EXFUN(gets, (char *));
int _EXFUN(putc, (int, FILE *));
int _EXFUN(putchar, (int));
int _EXFUN(puts, (const char *));
int _EXFUN(ungetc, (int, FILE *));
size_t _EXFUN(fread, (_PTR, size_t _size, size_t _n, FILE *));
size_t _EXFUN(fwrite, (const _PTR, size_t _size, size_t _n, FILE *));
int _EXFUN(fgetpos, (FILE *, fpos_t *));
int _EXFUN(fseek, (FILE *, long, int));
int _EXFUN(fsetpos, (FILE *, const fpos_t *));
long _EXFUN(ftell, (FILE *));
void _EXFUN(rewind, (FILE *));
void _EXFUN(clearerr, (FILE *));
int _EXFUN(feof, (FILE *));
int _EXFUN(ferror, (FILE *));
void _EXFUN(perror, (const char *));
#ifndef _REENT_ONLY
FILE *_EXFUN(fopen, (const char *_name, const char *_type));
int _EXFUN(sprintf, (char *, const char *, ...));
#endif
#ifndef _STRICT_ANSI
int _EXFUN(vfiprintf, (FILE *, const char *, __VALIST));
int _EXFUN(iprintf, (const char *, ...));
int _EXFUN(fiprintf, (FILE *, const char *, ...));
>>>>>>> master
#endif

/*
 * Routines in POSIX 1003.1.
 */

#ifndef _STRICT_ANSI
#ifndef _REENT_ONLY
<<<<<<< HEAD
FILE* _EXFUN(fdopen, (int, const char*));
#endif
int   _EXFUN(fileno, (FILE*));
int   _EXFUN(getw, (FILE*));
int   _EXFUN(pclose, (FILE*));
FILE* _EXFUN(popen, (const char*, const char*));
int   _EXFUN(putw, (int, FILE*));
void  _EXFUN(setbuffer, (FILE*, char*, int));
int   _EXFUN(setlinebuf, (FILE*));
=======
FILE *_EXFUN(fdopen, (int, const char *));
#endif
int _EXFUN(fileno, (FILE *));
int _EXFUN(getw, (FILE *));
int _EXFUN(pclose, (FILE *));
FILE *_EXFUN(popen, (const char *, const char *));
int _EXFUN(putw, (int, FILE *));
void _EXFUN(setbuffer, (FILE *, char *, int));
int _EXFUN(setlinebuf, (FILE *));
>>>>>>> master
#endif

/*
 * Recursive versions of the above.
 */

<<<<<<< HEAD
FILE* _EXFUN(_fdopen_r, (struct _reent*, int, const char*));
FILE* _EXFUN(_fopen_r, (struct _reent*, const char*, const char*));
int   _EXFUN(_getchar_r, (struct _reent*));
char* _EXFUN(_gets_r, (struct _reent*, char*));
int   _EXFUN(_iprintf_r, (struct _reent*, const char*, ...));
int   _EXFUN(_mkstemp_r, (struct _reent*, char*));
char* _EXFUN(_mktemp_r, (struct _reent*, char*));
void  _EXFUN(_perror_r, (struct _reent*, const char*));
int   _EXFUN(_printf_r, (struct _reent*, const char*, ...));
int   _EXFUN(_putchar_r, (struct _reent*, int));
int   _EXFUN(_puts_r, (struct _reent*, const char*));
int   _EXFUN(_remove_r, (struct _reent*, const char*));
int _EXFUN(_rename_r, (struct _reent*, const char* _old, const char* _new));
int   _EXFUN(_scanf_r, (struct _reent*, const char*, ...));
int   _EXFUN(_sprintf_r, (struct _reent*, char*, const char*, ...));
char* _EXFUN(_tempnam_r, (struct _reent*, char*, char*));
FILE* _EXFUN(_tmpfile_r, (struct _reent*));
char* _EXFUN(_tmpnam_r, (struct _reent*, char*));
int   _EXFUN(_vfprintf_r, (struct _reent*, FILE*, const char*, __VALIST));
int   _EXFUN(_vprintf_r, (struct _reent*, const char*, __VALIST));
int   _EXFUN(_vsprintf_r, (struct _reent*, char*, const char*, __VALIST));
=======
FILE *_EXFUN(_fdopen_r, (struct _reent *, int, const char *));
FILE *_EXFUN(_fopen_r, (struct _reent *, const char *, const char *));
int _EXFUN(_getchar_r, (struct _reent *));
char *_EXFUN(_gets_r, (struct _reent *, char *));
int _EXFUN(_iprintf_r, (struct _reent *, const char *, ...));
int _EXFUN(_mkstemp_r, (struct _reent *, char *));
char *_EXFUN(_mktemp_r, (struct _reent *, char *));
void _EXFUN(_perror_r, (struct _reent *, const char *));
int _EXFUN(_printf_r, (struct _reent *, const char *, ...));
int _EXFUN(_putchar_r, (struct _reent *, int));
int _EXFUN(_puts_r, (struct _reent *, const char *));
int _EXFUN(_remove_r, (struct _reent *, const char *));
int _EXFUN(_rename_r, (struct _reent *, const char *_old, const char *_new));
int _EXFUN(_scanf_r, (struct _reent *, const char *, ...));
int _EXFUN(_sprintf_r, (struct _reent *, char *, const char *, ...));
char *_EXFUN(_tempnam_r, (struct _reent *, char *, char *));
FILE *_EXFUN(_tmpfile_r, (struct _reent *));
char *_EXFUN(_tmpnam_r, (struct _reent *, char *));
int _EXFUN(_vfprintf_r, (struct _reent *, FILE *, const char *, __VALIST));
int _EXFUN(_vprintf_r, (struct _reent *, const char *, __VALIST));
int _EXFUN(_vsprintf_r, (struct _reent *, char *, const char *, __VALIST));
>>>>>>> master

/*
 * Routines internal to the implementation.
 */

<<<<<<< HEAD
int _EXFUN(__srget, (FILE*));
int _EXFUN(__swbuf, (int, FILE*));
=======
int _EXFUN(__srget, (FILE *));
int _EXFUN(__swbuf, (int, FILE *));
>>>>>>> master

/*
 * Stdio function-access interface.
 */

#ifndef _STRICT_ANSI
<<<<<<< HEAD
FILE* _EXFUN(funopen, (const _PTR _cookie, int (*readfn)(_PTR _cookie, char* _buf, int _n),
                          int (*writefn)(_PTR _cookie, const char* _buf, int _n),
                          fpos_t (*seekfn)(_PTR _cookie, fpos_t _off, int _whence), int (*closefn)(_PTR _cookie)));

#define fropen(cookie, fn) funopen(cookie, fn, (int (*)())0, (fpos_t(*)())0, (int (*)())0)
#define fwopen(cookie, fn) funopen(cookie, (int (*)())0, fn, (fpos_t(*)())0, (int (*)())0)
=======
FILE *_EXFUN(funopen, (const _PTR _cookie,
                       int (*readfn)(_PTR _cookie, char *_buf, int _n),
                       int (*writefn)(_PTR _cookie, const char *_buf, int _n),
                       fpos_t (*seekfn)(_PTR _cookie, fpos_t _off, int _whence),
                       int (*closefn)(_PTR _cookie)));

#define fropen(cookie, fn)                                                     \
  funopen(cookie, fn, (int (*)())0, (fpos_t(*)())0, (int (*)())0)
#define fwopen(cookie, fn)                                                     \
  funopen(cookie, (int (*)())0, fn, (fpos_t(*)())0, (int (*)())0)
>>>>>>> master
#endif

/*
 * The __sfoo macros are here so that we can
 * define function versions in the C library.
 */
#define __sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
#ifdef _never /* __GNUC__ */
<<<<<<< HEAD
              /* If this inline is actually used, then systems using coff debugging
                 info get hopelessly confused.  21sept93 rich@cygnus.com.  */
static __inline int __sputc(int _c, FILE* _p)
{
    if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
        return (*_p->_p++ = _c);
    else
        return (__swbuf(_c, _p));
=======
/* If this inline is actually used, then systems using coff debugging
   info get hopelessly confused.  21sept93 rich@cygnus.com.  */
static __inline int __sputc(int _c, FILE *_p) {
  if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
    return (*_p->_p++ = _c);
  else
    return (__swbuf(_c, _p));
>>>>>>> master
}
#else
/*
 * This has been tuned to generate reasonable code on the vax using pcc
 */
<<<<<<< HEAD
#define __sputc(c, p)                                                \
    (--(p)->_w < 0 ? (p)->_w >= (p)->_lbfsize ? (*(p)->_p = (c)),    \
        *(p)->_p != '\n' ? (int)*(p)->_p++ : __swbuf('\n', p)        \
                                              : __swbuf((int)(c), p) \
                                              : (*(p)->_p = (c), (int)*(p)->_p++))
=======
#define __sputc(c, p)                                                          \
  (--(p)->_w < 0 ? (p)->_w >= (p)->_lbfsize                                    \
   ? (*(p)->_p = (c)),                                                         \
   *(p)->_p != '\n' ? (int)*(p)->_p++ : __swbuf('\n', p)                       \
   : __swbuf((int)(c), p)                                                      \
   : (*(p)->_p = (c), (int)*(p)->_p++))
>>>>>>> master
#endif

#define __sfeof(p) (((p)->_flags & __SEOF) != 0)
#define __sferror(p) (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR | __SEOF)))
#define __sfileno(p) ((p)->_file)

#define feof(p) __sfeof(p)
#define ferror(p) __sferror(p)
#define clearerr(p) __sclearerr(p)

#if 0 /*ndef _STRICT_ANSI - FIXME: must initialize stdio first, use fn */
#define fileno(p) __sfileno(p)
#endif

#ifndef lint
#define getc(fp) __sgetc(fp)
#define putc(x, fp) __sputc(x, fp)
#endif /* lint */

#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)

#ifndef _STRICT_ANSI
/* fast always-buffered version, true iff error */
<<<<<<< HEAD
#define fast_putc(x, p) (--(p)->_w < 0 ? __swbuf((int)(x), p) == EOF : (*(p)->_p = (x), (p)->_p++, 0))
=======
#define fast_putc(x, p)                                                        \
  (--(p)->_w < 0 ? __swbuf((int)(x), p) == EOF : (*(p)->_p = (x), (p)->_p++, 0))
>>>>>>> master

#define L_cuserid 9 /* posix says it goes in stdio.h :( */
#endif

#ifdef __cplusplus
}
#endif
#endif /* _STDIO_H_ */
