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
        printf("reg\tvalue(hex)\tvalue(dec)\n");
        printf("eax\t0x%08x\t%d\n", cpu.eax, cpu.eax);
        printf("ecx\t0x%08x\t%d\n", cpu.ecx, cpu.ecx);
        printf("edx\t0x%08x\t%d\n", cpu.edx, cpu.edx);
        printf("ebx\t0x%08x\t%d\n", cpu.ebx, cpu.ebx);
        printf("esp\t0x%08x\t%d\n", cpu.esp, cpu.esp);
        printf("ebp\t0x%08x\t%d\n", cpu.ebp, cpu.ebp);
        printf("esi\t0x%08x\t%d\n", cpu.esi, cpu.esi);
        printf("edi\t0x%08x\t%d\n", cpu.edi, cpu.edi);
        printf("eip\t0x%08x\t%d\n", cpu.eip, cpu.eip);
        printf("eflags\t0x%08x\t%d\n", cpu.eflags, cpu.eflags);
        printf("\tCF: %d\tPF: %d\tAF: %d\tZF: %d\tSF: %d\tTF: %d\tIF: %d\tDF: %d\tOF: %d\n",
            cpu.CF,
            cpu.PF,
            cpu.AF,
            cpu.ZF,
            cpu.SF,
            cpu.TF,
            cpu.IF,
            cpu.DF,
            cpu.OF);
#ifdef DEBUG
        printf("EXEC CNT: %d\n", cpu.exec_cnt);
#endif
    }
    else if (strcmp(args, "w") == 0)
        print_wp();
    else
        Assert(false, "Unknown argument \"%s\" for info", args);

    return 0;
}

static int cmd_x(char* args)
{
    char* expr_ptr = NULL;
    int   n        = strtol(args, &expr_ptr, 0);

    Assert(n > 0, "Got argument \"%s\" for x, but it should be a positive integer", args);
    Assert(*expr_ptr == ' ', "Got argument \"%s\" for x, but it should be followed by a space", args);

    bool     success = false;
    uint32_t addr    = expr(expr_ptr + 1, &success);
    Assert(success, "Failed to evaluate expression \"%s\"", expr_ptr + 1);

    for (int i = 0; i < n; i++) {
        printf("0x%08x: 0x%08x\n", addr, vaddr_read(addr, 4));
        addr += 4;
    }

    return 0;
}

static int cmd_p(char* args)
{
    bool     success = false;
    uint32_t result  = expr(args, &success);
    if (success)
        printf("Result: 0x%08x %d\n", result, result);
    else
        printf("Failed to evaluate expression \"%s\"\n", args);

    return 0;
}

static int cmd_w(char* args)
{
    WP* wp = create_wp(args);
    if (wp == NULL) {
        Log("Failed to create watchpoint for expression \"%s\"", args);
        return 0;
    }

    printf("Watchpoint %d: %s 0x%08x\n", wp->NO, wp->expr_str, wp->prev_val);
    return 0;
}

static int cmd_b(char* args)
{
    bool     success = false;
    uint32_t addr    = expr(args, &success);
    if (!success) {
        printf("Failed to evaluate expression \"%s\"\n", args);
        return 0;
    }

    WP* wp = create_bp(addr);

    if (wp == NULL) {
        Log("Failed to create breakpoint for address 0x%08x", addr);
        return 0;
    }

    printf("Breakpoint %d: %s 0x%08x\n", wp->NO, wp->expr_str, wp->prev_val);
    return 0;
}

static int cmd_d(char* args)
{
    int n = atoi(args);
    destroy_wp(n);

    return 0;
}

static int cmd_wr(char* args)
{
    if (args == NULL) {
        printf("Usage: wr $REG, EXPR\n");
        return 0;
    }

    char* comma = strchr(args, ',');
    if (comma == NULL) {
        printf("Missing comma separator. Usage: wr $REG, EXPR\n");
        return 0;
    }

    *comma         = '\0';
    char* reg_ptr  = args;
    char* expr_str = comma + 1;

    while (*reg_ptr && (*reg_ptr == ' ' || *reg_ptr == '\t')) ++reg_ptr;

    char* reg_end                                                                 = reg_ptr + strlen(reg_ptr) - 1;
    while (reg_end > reg_ptr && (*reg_end == ' ' || *reg_end == '\t')) *reg_end-- = '\0';

    if (*reg_ptr == '$') ++reg_ptr;
    if (*reg_ptr == '\0') {
        printf("Missing register name. Usage: wr $REG, EXPR\n");
        return 0;
    }

    while (*expr_str && (*expr_str == ' ' || *expr_str == '\t')) ++expr_str;

    if (*expr_str == '\0') {
        printf("Missing expression. Usage: wr $REG, EXPR\n");
        return 0;
    }

    bool     success = false;
    uint32_t val     = expr(expr_str, &success);
    if (!success) {
        printf("Failed to evaluate expression \"%s\"\n", expr_str);
        return 0;
    }

#define X(name, width)                                                              \
    if (strcmp(reg_ptr, #name) == 0) {                                              \
        cpu.name = val;                                                             \
        Log("Set register \"%s\" to 0x%08x from expr: %s", reg_ptr, val, expr_str); \
        return 0;                                                                   \
    }
    X86_REGS
#undef X

    Log("Unknown register \"%s\"", reg_ptr);
    return 0;
}

static int cmd_wm(char* args)
{
    if (args == NULL) {
        printf("Usage: wm ADDR_EXPR, VALUE_EXPR\n");
        return 0;
    }

    char* comma = strchr(args, ',');
    if (comma == NULL) {
        printf("Missing comma separator. Usage: wm ADDR_EXPR, VALUE_EXPR\n");
        return 0;
    }

    *comma          = '\0';
    char* addr_expr = args;
    char* val_expr  = comma + 1;

    while (*addr_expr && (*addr_expr == ' ' || *addr_expr == '\t')) ++addr_expr;

    char* addr_end = addr_expr + strlen(addr_expr) - 1;
    while (addr_end > addr_expr && (*addr_end == ' ' || *addr_end == '\t')) *addr_end-- = '\0';

    if (*addr_expr == '\0') {
        printf("Missing address expression. Usage: wm ADDR_EXPR, VALUE_EXPR\n");
        return 0;
    }

    while (*val_expr && (*val_expr == ' ' || *val_expr == '\t')) ++val_expr;

    if (*val_expr == '\0') {
        printf("Missing value expression. Usage: wm ADDR_EXPR, VALUE_EXPR\n");
        return 0;
    }

    bool     success = false;
    uint32_t addr    = expr(addr_expr, &success);
    if (!success) {
        printf("Failed to evaluate address expression \"%s\"\n", addr_expr);
        return 0;
    }

    success      = false;
    uint32_t val = expr(val_expr, &success);
    if (!success) {
        printf("Failed to evaluate value expression \"%s\"\n", val_expr);
        return 0;
    }

    vaddr_write(addr, 4, val);

    Log("Write 0x%08x to 0x%08x (addr_expr: \"%s\", val_expr: \"%s\")", val, addr, addr_expr, val_expr);
    return 0;
}

#ifdef DIFF_TEST

bool gdb_memcpy_from_qemu(uint32_t src, void* dest, int len);

static int cmd_ri(char* args)
{
    // show register from gdb
    // args: none

    return 0;
}

static int cmd_rx(char* args)
{
    // show memory from gdb
    // args: num of memory, expr of start address

    char* expr_ptr = NULL;
    int   n        = strtol(args, &expr_ptr, 0);

    Assert(n > 0, "Got argument \"%s\" for x, but it should be a positive integer", args);
    Assert(*expr_ptr == ' ', "Got argument \"%s\" for x, but it should be followed by a space", args);

    bool     success = false;
    uint32_t addr    = expr(expr_ptr + 1, &success);
    Assert(success, "Failed to evaluate expression \"%s\"", expr_ptr + 1);

    uint8_t qemu_mem[4];
    bool    ok = false;

    for (int i = 0; i < n; i++) {
        // printf("0x%08x: 0x%08x\n", addr, vaddr_read(addr, 4));
        ok = gdb_memcpy_from_qemu(addr, qemu_mem, 4);
        if (!ok) {
            printf("Failed to read memory from qemu\n");
            return 0;
        }
        printf("0x%08x: 0x%02x%02x%02x%02x\n", addr, qemu_mem[3], qemu_mem[2], qemu_mem[1], qemu_mem[0]);
        addr += 4;
    }

    return 0;
}

#endif

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
    {"si", "Execute the program step by step: si [N]", cmd_si},
    {"info", "Print the information of registers or watchpoints: info [r/w]", cmd_info},
    {"x", "Scan memory: x [N] [EXPR]", cmd_x},
    {"p", "Evaluate expression: p [EXPR]", cmd_p},
    {"w", "Set watchpoint: w [EXPR]", cmd_w},
    {"d", "Delete watchpoint: d [N]", cmd_d},
    {"wr", "Write target register with expression: wr $REG, EXPR", cmd_wr},
    {"wm", "Write memory at address: wm ADDR_EXPR, VALUE_EXPR", cmd_wm},
    {"b", "Set breakpoint at address: b [EXPR]", cmd_b},
#ifdef DIFF_TEST
    {"ri", "Show register from gdb", cmd_ri},
    {"rx", "Show memory from gdb", cmd_rx},
#endif

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