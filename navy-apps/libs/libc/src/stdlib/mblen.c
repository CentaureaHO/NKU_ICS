#include <stdlib.h>

<<<<<<< HEAD
int _DEFUN(mblen, (s, n), const char* s _AND size_t n)
{
    if (s == NULL || *s == '\0') return 0;
    if (n == 0) return -1;
    return 1;
=======
int _DEFUN(mblen, (s, n), const char *s _AND size_t n) {
  if (s == NULL || *s == '\0')
    return 0;
  if (n == 0)
    return -1;
  return 1;
>>>>>>> master
}
