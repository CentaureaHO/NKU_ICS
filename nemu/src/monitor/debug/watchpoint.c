#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include <stdlib.h>

#define NR_WP 32

static WP  wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; ++i) {
        wp_pool[i].NO   = i;
        wp_pool[i].next = &wp_pool[i + 1];
    }
    wp_pool[NR_WP - 1].next = NULL;

    head  = NULL;
    free_ = wp_pool;
}

void destroy_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; ++i) {
        if (wp_pool[i].expr_str != NULL) free(wp_pool[i].expr_str);
    }
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp()
{
    if (free_ == NULL) {
        Log("No more watchpoint available");
        return NULL;
    }

    WP* wp = free_;
    free_ = free_->next;
    wp->next = head;
    head = wp;

    return wp;
}

void free_wp(WP* wp)
{
    if (wp == NULL) {
        Log("Invalid watchpoint");
        return;
    }

    WP* p = head;
    if (p == wp) {
        head = head->next;
        wp->next = free_;
        free_ = wp;
        return;
    }

    while (p != NULL)
    {
        if (p->next != wp)
        {
            p = p->next;
            continue;
        }

        p->next = wp->next;
        wp->next = free_;
        free_ = wp;
        return;
    }

    Log("Target watchpoint not found");
}

WP* create_wp(char* es)
{
    bool success = true;
    uint32_t val = expr(es, &success);
    if (!success) {
        Log("Failed to evaluate expression");
        return NULL;
    }

    WP* wp = new_wp();
    if (wp == NULL) {
        Log("Failed to create watchpoint");
        return NULL;
    }

    wp->expr_str = strdup(es);
    wp->prev_val = val;
    
    return wp;
}

void destroy_wp(int n)
{
    if (n < 0 || n >= NR_WP) {
        Log("Invalid watchpoint number");
        return;
    }

    free_wp(&wp_pool[n]);

    if (wp_pool[n].expr_str != NULL) free(wp_pool[n].expr_str);
    wp_pool[n].prev_val = 0;
}

void print_wp()
{
    WP* p = head;
    if (p == NULL) {
        Log("No watchpoint");
        return;
    }

    printf("NO\tEXPR\tVALUE\n");
    while (p != NULL) {
        printf("%d\t%s\t0x%08x\n", p->NO, p->expr_str, p->prev_val);
        p = p->next;
    }
}

bool check_wp()
{
    WP* p = head;
    bool changed = false;
    bool success = true;
    while (p != NULL)
    {
        uint32_t val = expr(p->expr_str, &success);

        if (val != p->prev_val) {
            changed = true;
            printf("Watchpoint %d: %s\n", p->NO, p->expr_str);
            printf("Old value = 0x%08x\n", p->prev_val);
            printf("New value = 0x%08x\n", val);
            p->prev_val = val;
        }

        p = p->next;
    }

    return changed;
}