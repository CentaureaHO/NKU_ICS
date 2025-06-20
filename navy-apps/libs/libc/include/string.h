/*
 * string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _STRING_H_
#define _STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_ansi.h"

#define __need_size_t
#include <stddef.h>

#ifndef NULL
#define NULL 0L
#endif

<<<<<<< HEAD
_PTR   _EXFUN(memchr, (const _PTR, int, size_t));
int    _EXFUN(memcmp, (const _PTR, const _PTR, size_t));
_PTR   _EXFUN(memcpy, (_PTR, const _PTR, size_t));
_PTR   _EXFUN(memmove, (_PTR, const _PTR, size_t));
_PTR   _EXFUN(memset, (_PTR, int, size_t));
char*  _EXFUN(strcat, (char*, const char*));
char*  _EXFUN(strchr, (const char*, int));
int    _EXFUN(strcmp, (const char*, const char*));
int    _EXFUN(strcoll, (const char*, const char*));
char*  _EXFUN(strcpy, (char*, const char*));
size_t _EXFUN(strcspn, (const char*, const char*));
char*  _EXFUN(strerror, (int));
size_t _EXFUN(strlen, (const char*));
char*  _EXFUN(strncat, (char*, const char*, size_t));
int    _EXFUN(strncmp, (const char*, const char*, size_t));
char*  _EXFUN(strncpy, (char*, const char*, size_t));
char*  _EXFUN(strpbrk, (const char*, const char*));
char*  _EXFUN(strrchr, (const char*, int));
size_t _EXFUN(strspn, (const char*, const char*));
char*  _EXFUN(strstr, (const char*, const char*));

#ifndef _REENT_ONLY
char* _EXFUN(strtok, (char*, const char*));
#endif

size_t _EXFUN(strxfrm, (char*, const char*, size_t));

#ifndef _STRICT_ANSI
struct _reent;
char* _EXFUN(_strtok_r, (struct _reent*, char*, const char*));

int   _EXFUN(bcmp, (const char*, const char*, size_t));
void  _EXFUN(bcopy, (const char*, char*, size_t));
void  _EXFUN(bzero, (char*, size_t));
int   _EXFUN(ffs, (int));
char* _EXFUN(index, (const char*, int));
_PTR  _EXFUN(memccpy, (_PTR, const _PTR, int, size_t));
char* _EXFUN(rindex, (const char*, int));
int   _EXFUN(strcasecmp, (const char*, const char*));
char* _EXFUN(strdup, (const char*));
int   _EXFUN(strncasecmp, (const char*, const char*, size_t));
char* _EXFUN(strsep, (char*, const char*));
void  _EXFUN(swab, (const char*, char*, size_t));
=======
_PTR _EXFUN(memchr, (const _PTR, int, size_t));
int _EXFUN(memcmp, (const _PTR, const _PTR, size_t));
_PTR _EXFUN(memcpy, (_PTR, const _PTR, size_t));
_PTR _EXFUN(memmove, (_PTR, const _PTR, size_t));
_PTR _EXFUN(memset, (_PTR, int, size_t));
char *_EXFUN(strcat, (char *, const char *));
char *_EXFUN(strchr, (const char *, int));
int _EXFUN(strcmp, (const char *, const char *));
int _EXFUN(strcoll, (const char *, const char *));
char *_EXFUN(strcpy, (char *, const char *));
size_t _EXFUN(strcspn, (const char *, const char *));
char *_EXFUN(strerror, (int));
size_t _EXFUN(strlen, (const char *));
char *_EXFUN(strncat, (char *, const char *, size_t));
int _EXFUN(strncmp, (const char *, const char *, size_t));
char *_EXFUN(strncpy, (char *, const char *, size_t));
char *_EXFUN(strpbrk, (const char *, const char *));
char *_EXFUN(strrchr, (const char *, int));
size_t _EXFUN(strspn, (const char *, const char *));
char *_EXFUN(strstr, (const char *, const char *));

#ifndef _REENT_ONLY
char *_EXFUN(strtok, (char *, const char *));
#endif

size_t _EXFUN(strxfrm, (char *, const char *, size_t));

#ifndef _STRICT_ANSI
struct _reent;
char *_EXFUN(_strtok_r, (struct _reent *, char *, const char *));

int _EXFUN(bcmp, (const char *, const char *, size_t));
void _EXFUN(bcopy, (const char *, char *, size_t));
void _EXFUN(bzero, (char *, size_t));
int _EXFUN(ffs, (int));
char *_EXFUN(index, (const char *, int));
_PTR _EXFUN(memccpy, (_PTR, const _PTR, int, size_t));
char *_EXFUN(rindex, (const char *, int));
int _EXFUN(strcasecmp, (const char *, const char *));
char *_EXFUN(strdup, (const char *));
int _EXFUN(strncasecmp, (const char *, const char *, size_t));
char *_EXFUN(strsep, (char *, const char *));
void _EXFUN(swab, (const char *, char *, size_t));
>>>>>>> master
#endif /* ! _STRICT_ANSI */

#ifdef __cplusplus
}
#endif
#endif /* _STRING_H_ */
