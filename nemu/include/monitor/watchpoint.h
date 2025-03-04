#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#include "monitor/expr.h"

typedef struct watchpoint
{
    int                NO;
    struct watchpoint* next;

    /* TODO: Add more members if necessary */
    uint32_t prev_val;
    char*    expr_str;
    ASTNode* ast;
} WP;

WP* create_wp(char* es);
void destroy_wp(int n);
void print_wp();
bool check_wp();
void init_wp_pool();
void destroy_wp_pool();

#endif
