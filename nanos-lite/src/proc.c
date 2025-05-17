#include "proc.h"
#include "list.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC];
static int nr_proc = 0;
PCB*       current = NULL;

int curgame_idx = 0;

static list_entry_t proc_list;

typedef struct
{
    PCB*         pcb;
    list_entry_t list;
} proc_node_t;

static proc_node_t proc_nodes[MAX_NR_PROC];

void init_proc() { list_init(&proc_list); }

uintptr_t loader(_Protect* as, const char* filename);

void load_prog(const char* filename)
{
    int i = nr_proc++;
    _protect(&pcb[i].as);

    uintptr_t entry = loader(&pcb[i].as, filename);

    // TODO: remove the following three lines after you have implemented _umake()
    /*
    _switch(&pcb[i].as);
    current = &pcb[i];
    ((void (*)(void))entry)();
    */

    _Area stack;
    stack.start = pcb[i].stack;
    stack.end   = stack.start + sizeof(pcb[i].stack);

    pcb[i].tf = _umake(&pcb[i].as, stack, stack, (void*)entry, NULL, NULL);

    proc_nodes[i].pcb = &pcb[i];
    list_add(&proc_list, &proc_nodes[i].list);
}

#if SCHEDULE_POLICY == 1
_RegSet* schedule(_RegSet* prev)
{
    if (current) current->tf = prev;

    if (nr_proc < 2) {
        if (nr_proc == 1)
            current = &pcb[0];
        else
            return NULL;
    }
    else
    {
        static size_t i = 0;
        current         = (i++ % 2 == 0) ? &pcb[0] : &pcb[1];
    }

    if (current) {
        _switch(&current->as);
        return current->tf;
    }
    return NULL;
}
#elif SCHEDULE_POLICY == 2
_RegSet* schedule(_RegSet* prev)
{
    if (current) current->tf = prev;

    if (list_empty(&proc_list)) {
        current = NULL;
        return NULL;
    }

    if (current == NULL) {
        list_entry_t* entry = list_next(&proc_list);
        proc_node_t*  node  = (proc_node_t*)((char*)entry - offsetof(proc_node_t, list));
        current             = node->pcb;
    }
    else
    {
        list_entry_t* entry = NULL;
        proc_node_t*  node  = NULL;
        bool          found = false;

        list_for_each(entry, &proc_list)
        {
            node = (proc_node_t*)((char*)entry - offsetof(proc_node_t, list));
            if (node->pcb == current) {
                found = true;
                break;
            }
        }

        if (found) {
            entry                          = list_next(entry);
            if (entry == &proc_list) entry = list_next(&proc_list);

            node    = (proc_node_t*)((char*)entry - offsetof(proc_node_t, list));
            current = node->pcb;
        }
        else
        {
            entry   = list_next(&proc_list);
            node    = (proc_node_t*)((char*)entry - offsetof(proc_node_t, list));
            current = node->pcb;
        }
    }

    if (current) {
        _switch(&current->as);
        return current->tf;
    }
    return NULL;
}
#elif SCHEDULE_POLICY == 3
_RegSet* schedule(_RegSet* prev)
{
    if (current) current->tf = prev;

    if (nr_proc < 2) {
        if (nr_proc == 1)
            current = &pcb[0];
        else
            return NULL;
    }
    else
    {
        PCB* game_pcb = NULL;
        if (curgame_idx == 0)
            game_pcb = &pcb[0];
        else
        {
            if (nr_proc > 2)
                game_pcb = &pcb[2];
            else
            {
                game_pcb    = &pcb[0];
                curgame_idx = 0;
            }
        }

        PCB* hello_pcb = &pcb[1];

        if (current == game_pcb)
            current = hello_pcb;
        else
            current = game_pcb;
    }

    if (current) {
        _switch(&current->as);
        return current->tf;
    }
    return NULL;
}
#endif
