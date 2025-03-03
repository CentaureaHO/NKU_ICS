#include "monitor/expr.h"
#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin.
 */
char* rl_gets()
{
    static char* line_read = NULL;

    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(nemu) ");

    if (line_read && *line_read) {
        add_history(line_read);
    }

    return line_read;
}

static int cmd_c(char* args)
{
    cpu_exec(-1);
    return 0;
}

static int cmd_q(char* args) { return -1; }

static int cmd_si(char* args)
{
    if (args == NULL) {
        cpu_exec(1);
        return 0;
    }

    int n = 0;
    Assert((n = atoi(args)) >= 1, "Got argument \"%s\" for si, but it should be a positive integer", args);

    cpu_exec(n);

    return 0;
}

static int cmd_info(char* args)
{
    if (strcmp(args, "r") == 0) {
        printf("eax\t0x%08x\t%d\n", cpu.eax, cpu.eax);
        printf("ecx\t0x%08x\t%d\n", cpu.ecx, cpu.ecx);
        printf("edx\t0x%08x\t%d\n", cpu.edx, cpu.edx);
        printf("ebx\t0x%08x\t%d\n", cpu.ebx, cpu.ebx);
        printf("esp\t0x%08x\t0x%08x\n", cpu.esp, cpu.esp);
        printf("ebp\t0x%08x\t0x%08x\n", cpu.ebp, cpu.ebp);
        printf("esi\t0x%08x\t0x%08x\n", cpu.esi, cpu.esi);
        printf("edi\t0x%08x\t0x%08x\n", cpu.edi, cpu.edi);
        printf("eip\t0x%08x\t0x%08x\n", cpu.eip, cpu.eip);
    }
    else
        Assert(false, "Unknown argument \"%s\" for info", args);

    return 0;
}

static int cmd_x(char* args)
{
    int n = 0;
    char* endptr = NULL;

    n = strtol(args, &endptr, 10);

    Log("n = %d, endptr = %s", n, endptr);

    return 0;
}

static int cmd_p(char* args)
{
    bool     success = false;
    uint32_t result  = expr(args, &success);
    if (success) {
        printf("Result: %u\n", result);
    }
    else
    {
        printf("Failed to evaluate expression \"%s\"\n", args);
    }

    return 0;
}

static int cmd_help(char* args);

static struct
{
    char* name;
    char* description;
    int (*handler)(char*);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Execute the program step by step", cmd_si},
    {"info", "Print the information of registers", cmd_info},
    {"x", "Scan memory", cmd_x},
    {"p", "Evaluate expression", cmd_p},

    /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char* args)
{
    /* extract the first argument */
    char* arg = strtok(NULL, " ");
    int   i;

    if (arg == NULL) {
        /* no argument given */
        for (i = 0; i < NR_CMD; i++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else
    {
        for (i = 0; i < NR_CMD; i++) {
            if (strcmp(arg, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

void ui_mainloop(int is_batch_mode)
{
    if (is_batch_mode) {
        cmd_c(NULL);
        return;
    }

    while (1) {
        char* str     = rl_gets();
        char* str_end = str + strlen(str);

        /* extract the first token as the command */
        char* cmd = strtok(str, " ");
        if (cmd == NULL) {
            continue;
        }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char* args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }

#ifdef HAS_IOE
        extern void sdl_clear_event_queue(void);
        sdl_clear_event_queue();
#endif

        int i;
        for (i = 0; i < NR_CMD; i++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) {
                    return;
                }
                break;
            }
        }

        if (i == NR_CMD) {
            printf("Unknown command '%s'\n", cmd);
        }
    }
}
