#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint
{
    int                NO;
    struct watchpoint* next;

    /* TODO: Add more members if necessary */
    uint32_t prev_val;
    char*    expr_str;
} WP;

WP*  create_wp(char* es);
void destroy_wp(int n);
void print_wp();

#endif
