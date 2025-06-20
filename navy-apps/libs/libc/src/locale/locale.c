/*
FUNCTION
<<setlocale>>, <<localeconv>>---select or query locale

INDEX
        setlocale
INDEX
        localeconv
INDEX
        _setlocale_r
INDEX
        _localeconv_r

ANSI_SYNOPSIS
        #include <locale.h>
        char *setlocale(int <[category]>, const char *<[locale]>);
        lconv *localeconv(void);

        char *_setlocale_r(void *<[reent]>,
                        int <[category]>, const char *<[locale]>);
        lconv *_localeconv_r(void *<[reent]>);

TRAD_SYNOPSIS
        #include <locale.h>
        char *setlocale(<[category]>, <[locale]>)
        int <[category]>;
        char *<[locale]>;

        lconv *localeconv();

        char *_setlocale_r(<[reent]>, <[category]>, <[locale]>)
        char *<[reent]>;
        int <[category]>;
        char *<[locale]>;

        lconv *_localeconv_r(<[reent]>);
        char *<[reent]>;

DESCRIPTION
<<setlocale>> is the facility defined by ANSI C to condition the
execution environment for international collating and formatting
information; <<localeconv>> reports on the settings of the current
locale.

This is a minimal implementation, supporting only the required <<``C''>>
value for <[locale]>; strings representing other locales are not
honored.  (<<``''>> is also accepted; it represents the default locale
for an implementation, here equivalent to <<``C''>>.)

If you use <<NULL>> as the <[locale]> argument, <<setlocale>> returns
a pointer to the string representing the current locale (always
<<``C''>> in this implementation).  The acceptable values for
<[category]> are defined in `<<locale.h>>' as macros beginning with
<<"LC_">>, but this implementation does not check the values you pass
in the <[category]> argument.

<<localeconv>> returns a pointer to a structure (also defined in
`<<locale.h>>') describing the locale-specific conventions currently
in effect.

<<_localeconv_r>> and <<_setlocale_r>> are reentrant versions of
<<localeconv>> and <<setlocale>> respectively.  The extra argument
<[reent]> is a pointer to a reentrancy structure.

RETURNS
<<setlocale>> returns either a pointer to a string naming the locale
currently in effect (always <<``C''>> for this implementation), or, if
the locale request cannot be honored, <<NULL>>.

<<localeconv>> returns a pointer to a structure of type <<lconv>>,
which describes the formatting and collating conventions in effect (in
this implementation, always those of the C locale).

PORTABILITY
ANSI C requires <<setlocale>>, but the only locale required across all
implementations is the C locale.

No supporting OS subroutines are required.
*/

/*
 * setlocale, localeconv : internationalize your locale.
 *                         (Only "C" or null supported).
 */

#include <limits.h>
#include <locale.h>
#include <reent.h>
#include <string.h>

static _CONST struct lconv lconv = {
<<<<<<< HEAD
    ".",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
};

char* _DEFUN(_setlocale_r, (p, category, locale), struct _reent* p _AND int category _AND _CONST char* locale)
{
    if (locale && strcmp(locale, "C") && strcmp(locale, "")) return 0;

    p->_current_category = category;

    p->_current_locale = locale;

    return "C";
}

struct lconv* _DEFUN(_localeconv_r, (data), struct _reent* data) { return (struct lconv*)&lconv; }

#ifndef _REENT_ONLY

char* _DEFUN(setlocale, (category, locale), int category _AND _CONST char* locale)
{
    return _setlocale_r(_REENT, category, locale);
}

struct lconv* _DEFUN_VOID(localeconv) { return _localeconv_r(_REENT); }
=======
    ".",      "",       "",       "",       "",       "",
    "",       "",       "",       "",       CHAR_MAX, CHAR_MAX,
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
};

char *_DEFUN(_setlocale_r, (p, category, locale),
             struct _reent *p _AND int category _AND _CONST char *locale) {
  if (locale && strcmp(locale, "C") && strcmp(locale, ""))
    return 0;

  p->_current_category = category;

  p->_current_locale = locale;

  return "C";
}

struct lconv *_DEFUN(_localeconv_r, (data), struct _reent *data) {
  return (struct lconv *)&lconv;
}

#ifndef _REENT_ONLY

char *_DEFUN(setlocale, (category, locale),
             int category _AND _CONST char *locale) {
  return _setlocale_r(_REENT, category, locale);
}

struct lconv *_DEFUN_VOID(localeconv) {
  return _localeconv_r(_REENT);
}
>>>>>>> master

#endif
