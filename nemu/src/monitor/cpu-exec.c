#include "monitor/monitor.h"
#include "nemu.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = NEMU_STOP;

void exec_wrapper(bool);
bool check_wp();

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n)
{
    if (nemu_state == NEMU_END) {
        printf("Program execution has ended. To restart the program, exit NEMU and "
               "run again.\n");
        return;
    }
    nemu_state = NEMU_RUNNING;

    bool print_flag = n < MAX_INSTR_TO_PRINT;

    for (; n > 0; n--) {
        /* Execute one instruction, including instruction fetch,
         * instruction decode, and the actual execution. */
        exec_wrapper(print_flag);
<<<<<<< HEAD

#ifdef DEBUG
/* TODO: check watchpoints here. */
=======

        if (check_wp()) {
            printf("Current eip: 0x%08x\n", cpu.eip);
            nemu_state = NEMU_STOP;
        }

#ifdef DEBUG
        if (check_wp()) {
            printf("Current eip: 0x%08x\n", cpu.eip);
            nemu_state = NEMU_STOP;
        }
>>>>>>> master

#endif

#ifdef HAS_IOE
        extern void device_update();
        device_update();
#endif

        if (nemu_state != NEMU_RUNNING) {
            return;
        }
    }

    if (nemu_state == NEMU_RUNNING) {
        nemu_state = NEMU_STOP;
    }
}
