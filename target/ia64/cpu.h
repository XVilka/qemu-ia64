/*
 * IA64 virtual CPU header
 *
 *  Copyright (c) 2011 Prashant Vaibhav
 *  Copyright (c) 2009 Ulrich Hecht
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */
#ifndef CPU_IA64_H
#define CPU_IA64_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

#define ELF_MACHINE	EM_IA_64

#define MMU_USER_IDX 0 // guess

typedef struct CPUIA64State {
    uint64_t    gr[128];    // general registers gr0 - gr127
    uint64_t    pr;         // predicate registers, pr[qp] = 1 & (pr >> qp)
#define IA64_BIT_IS_SET(p,x) (1 & ((p) >> (x)))
#define IA64_SET_BIT(p,x)    ((p) = (p) | (1 << (x)))

    uint64_t    br[8];      // branch registers br0 - br7
    uint64_t    ar[128];    // application registers ar0 - ar127
#define ar_pfs  64

    uint64_t    ip;         // instruction pointer (PC)
    struct {                // current frame marker (has various fields)
        uint32_t sof;
        uint32_t sol;
        uint32_t sor;
        struct {
            uint32_t gr;
            uint32_t fr;
            uint32_t pr;
        } rrb;
    } cfm;

    uint64_t code_ends_at;
    uint32_t hflags;
#define CPU_PAL_HALT 1
#define HF_HALTED_MASK       (1 << CPU_PAL_HALT)
    // TODO: cpuid, pmd, user mask and alat registers

} CPUIA64State;

/**
 * IA64CCPU:
 * @env: #CPUIA64State
 *
 * An IA64 CPU.
 */
struct IA64CPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/

    CPUNegativeOffsetState neg;
    CPUIA64State env;
};

typedef CPUIA64State CPUArchState;
typedef IA64CPU ArchCPU;

#include "exec/cpu-all.h"

#if defined(CONFIG_USER_ONLY)
static inline void cpu_clone_regs(CPUState *env, target_ulong newsp)
{
    // TODO
}
#endif

#define CPU_RESOLVING_TYPE TYPE_IA64_CPU

void ia64_cpu_do_interrupt (CPUState *env);
bool ia64_cpu_exec_interrupt(CPUState *cpu, int int_req);
void ia64_cpu_dump_state(CPUState *cs, FILE *f, int flags);

CPUState *ia64_cpu_init(const char *cpu_model);
int ia64_cpu_exec(CPUState *s);
void ia64_cpu_close(CPUState *s);

static inline int cpu_mmu_index (CPUIA64State *env, bool ifetch)
{
        return 0;
}

void ia64_tcg_init(void);

/* you can call this signal handler from your SIGBUS and SIGSEGV
   signal handlers to inform the virtual CPU of exceptions. non zero
   is returned if the signal was handled by the virtual CPU.  */
int cpu_ia64_signal_handler(int host_signum, void *pinfo,
                           void *puc);
int cpu_ia64_handle_mmu_fault (CPUIA64State *env, target_ulong address, int rw,
                              int mmu_idx, int is_softmuu);

#define cpu_handle_mmu_fault cpu_ia64_handle_mmu_fault
#define cpu_init ia64_cpu_init
#define cpu_exec ia64_cpu_exec
#define cpu_gen_code cpu_ia64_gen_code
#define cpu_signal_handler cpu_ia64_signal_handler

enum {
    EXCP_SYSCALL_BREAK, /* syscall via break.m 0x10000  */
    EXCP_MMFAULT
};

static inline void cpu_get_tb_cpu_state(CPUIA64State* env, target_ulong *pc,
                                        target_ulong *cs_base, uint32_t *pflags)
{
    *pc = env->ip;
    *cs_base = 0;
    *pflags = 0;
}

#endif
