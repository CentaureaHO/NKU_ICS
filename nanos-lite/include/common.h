#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug.h"
#include <am.h>

#ifdef assert
#pragma push_macro("assert")
#undef assert
#endif

#include <klib.h>

#pragma pop_macro("assert")

typedef char bool;
#define true 1
#define false 0

#define SCHEDULE_POLICY 3

#endif
