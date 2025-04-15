#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug.h"
#include <am.h>
<<<<<<< HEAD
#include <klib.h>
=======

#ifdef assert
#pragma push_macro("assert")
#undef assert
#endif

#include <klib.h>

#pragma pop_macro("assert")
>>>>>>> master

typedef char bool;
#define true 1
#define false 0

#endif
