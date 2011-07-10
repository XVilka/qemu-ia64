#include "qemu/osdep.h"
#include "qemu-common.h"
#include "qemu.h"
#include "cpu_loop-common.h"

void target_cpu_copy_regs(CPUArchState *env, struct target_pt_regs *regs)
{
        // Nothing special TODO
}

void cpu_loop(CPUIA64State *env)
{
    int trapnr;
    // XXX needed later: target_siginfo_t info;

    while (1) {
        trapnr = cpu_ia64_exec (env);

        switch (trapnr) {
            // TODO: handle traps
        default:
            printf ("Unhandled trap: 0x%x\n", trapnr);
            cpu_dump_state(env, stderr, fprintf, 0);
            exit (1);
        }
        process_pending_signals (env);
    }
}


