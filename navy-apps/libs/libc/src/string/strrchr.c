/*
FUNCTION
        <<strrchr>>---reverse search for character in string

INDEX
        strrchr

ANSI_SYNOPSIS
        #include <string.h>
        char * strrchr(const char *<[string]>, int <[c]>);

TRAD_SYNOPSIS
        #include <string.h>
        char * strrchr(<[string]>, <[c]>);
        char *<[string]>;
        int *<[c]>;

DESCRIPTION
        This function finds the last occurence of <[c]> (converted to
        a char) in the string pointed to by <[string]> (including the
        terminating null character).

RETURNS
        Returns a pointer to the located character, or a null pointer
        if <[c]> does not occur in <[string]>.

PORTABILITY
<<strrchr>> is ANSI C.

<<strrchr>> requires no supporting OS subroutines.

QUICKREF
        strrchr ansi pure
*/

#include <string.h>

<<<<<<< HEAD
char* _DEFUN(strrchr, (s, i), _CONST char* s _AND int i)
{
    _CONST char* last = NULL;
    char         c    = i;

    while (*s) {
        if (*s == c) {
            last = s;
        }
        s++;
    }

    if (*s == c) {
        last = s;
=======
char *_DEFUN(strrchr, (s, i), _CONST char *s _AND int i) {
  _CONST char *last = NULL;
  char c = i;

  while (*s) {
    if (*s == c) {
      last = s;
>>>>>>> master
    }
    s++;
  }

<<<<<<< HEAD
    return (char*)last;
=======
  if (*s == c) {
    last = s;
  }

  return (char *)last;
>>>>>>> master
}
