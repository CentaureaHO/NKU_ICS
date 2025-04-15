#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#include "monitor/expr.h"

typedef struct watchpoint
{
    int                NO;
    struct watchpoint* next;

    /* TODO: Add more members if necessary */
<<<<<<< HEAD
=======
    uint32_t prev_val;
    char*    expr_str;
    ASTNode* ast;
>>>>>>> master

    bool is_bp;
} WP;

WP* create_wp(char* es);
WP* create_bp(uint32_t addr);
void destroy_wp(int n);
void print_wp();
bool check_wp();
void init_wp_pool();
void destroy_wp_pool();

#endif
