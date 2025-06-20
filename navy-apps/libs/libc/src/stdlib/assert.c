/*
FUNCTION
<<assert>>---Macro for Debugging Diagnostics

INDEX
        assert

ANSI_SYNOPSIS
        #include <assert.h>
        #include <stdlib.h>
        void assert(int <[expression]>);

TRAD_SYNOPSIS
        #include <assert.h>
        #include <stdlib.h>
        assert(<[expression]>)
        int <[expression]>;

DESCRIPTION
        Use this macro to embed debuggging diagnostic statements in
        your programs.  The argument <[expression]> should be an
        expression which evaluates to true (nonzero) when your program
        is working as you intended.

        When <[expression]> evaluates to false (zero), <<assert>>
        calls <<abort>>, after first printing a message showing what
        failed and where:

. Assertion failed: <[expression]>, file <[filename]>, line <[lineno]>

        The macro is defined to permit you to turn off all uses of
        <<assert>> at compile time by defining <<NDEBUG>> as a
        preprocessor variable.   If you do this, the <<assert>> macro
        expands to

. (void(0))

RETURNS
        <<assert>> does not return a value.

PORTABILITY
        The <<assert>> macro is required by ANSI, as is the behavior
        when <<NDEBUG>> is defined.

Supporting OS subroutines required (only if enabled): <<close>>, <<fstat>>,
<<getpid>>, <<isatty>>, <<kill>>, <<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
int _DEFUN(__assertfail, (string, cond, file, line), char* string _AND char* cond _AND char* file _AND int line)
{
    fprintf(stderr, string, cond, file, line);
    abort();
=======
int _DEFUN(__assertfail, (string, cond, file, line),
           char *string _AND char *cond _AND char *file _AND int line) {
  fprintf(stderr, string, cond, file, line);
  abort();
>>>>>>> master
}
