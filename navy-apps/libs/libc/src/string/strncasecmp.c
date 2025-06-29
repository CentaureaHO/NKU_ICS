/*
FUNCTION
        <<strncasecmp>>---case insensitive character string compare

INDEX
        strncasecmp

ANSI_SYNOPSIS
        #include <string.h>
        int strncasecmp(const char *<[a]>, const char * <[b]>, size_t
<[length]>);

TRAD_SYNOPSIS
        #include <string.h>
        int strncasecmp(<[a]>, <[b]>, <[length]>)
        char *<[a]>;
        char *<[b]>;
        size_t <[length]>

DESCRIPTION
        <<strncasecmp>> compares up to <[length]> characters
        from the string at <[a]> to the string at <[b]> in a
        case-insensitive manner.

RETURNS

        If <<*<[a]>>> sorts lexicographically after <<*<[b]>>> (after
        both are converted to upper case), <<strncasecmp>> returns a
        number greater than zero.  If the two strings are equivalent,
        <<strncasecmp>> returns zero.  If <<*<[a]>>> sorts
        lexicographically before <<*<[b]>>>, <<strncasecmp>> returns a
        number less than zero.

PORTABILITY
<<strncasecmp>> is in the Berkeley Software Distribution.

<<strncasecmp>> requires no supporting OS subroutines. It uses
toupper() from elsewhere in this library.

QUICKREF
        strncasecmp
*/

#include <ctype.h>
#include <string.h>

<<<<<<< HEAD
int _DEFUN(strncasecmp, (s1, s2, n), _CONST char* s1 _AND _CONST char* s2 _AND size_t n)
{
    if (n == 0) return 0;

    while (n-- != 0 && toupper(*s1) == toupper(*s2)) {
        if (n == 0 || *s1 == '\0' || *s2 == '\0') break;
        s1++;
        s2++;
    }

    return toupper(*(unsigned char*)s1) - toupper(*(unsigned char*)s2);
=======
int _DEFUN(strncasecmp, (s1, s2, n),
           _CONST char *s1 _AND _CONST char *s2 _AND size_t n) {
  if (n == 0)
    return 0;

  while (n-- != 0 && toupper(*s1) == toupper(*s2)) {
    if (n == 0 || *s1 == '\0' || *s2 == '\0')
      break;
    s1++;
    s2++;
  }

  return toupper(*(unsigned char *)s1) - toupper(*(unsigned char *)s2);
>>>>>>> master
}
