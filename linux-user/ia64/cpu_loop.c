#include "qemu/osdep.h"
#include "qemu-common.h"
#include "qemu.h"
#include "cpu_loop-common.h"

void target_cpu_copy_regs(CPUArchState *env, struct target_pt_regs *regs)
{
        env->ip = regs->pc;
        env->code_ends_at = regs->code_ends_at;
}

void cpu_loop(CPUIA64State *env)
{
    int trapnr;
    // XXX needed later: target_siginfo_t info;

    while (1) {
        trapnr = cpu_ia64_exec (env);

        switch (trapnr) {
            case EXCP_SYSCALL_BREAK:
                trapnr = env->gr[15];
                // FIXME: use a better method to map target -> host syscall
                switch (trapnr) {
                    case 1027:
                        // write
                        trapnr = 4;
                        break;
                    case 1025:
                        // exit
                        trapnr = 1;
                        break;
                };
                do_syscall(env, trapnr,
                                    env->gr[36], env->gr[37],
                                    env->gr[38], 0,
                                    0, 0,
                                    0, 0);
                /* Handle syscall via break.m insn */
                break;
        default:
            printf ("Unhandled trap: 0x%x\n", trapnr);
            cpu_dump_state(env, stderr, fprintf, 0);
            exit (1);
        }
        process_pending_signals (env);
    }
}


