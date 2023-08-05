//
// Created by zfb457 on 2023/8/4.
//

#ifndef NATIVEINJECTOR_TOOLS_H
#define NATIVEINJECTOR_TOOLS_H

#include <asm/ptrace.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/elf.h>
#include <pthread.h>
#include <string.h>
#include <cutils/uio.h>
#include "err.h"
#include "java_def.h"

#define ARM_cpsr    uregs[16]
#define ARM_pc      uregs[15]
#define ARM_lr      uregs[14]
#define ARM_sp      uregs[13]
#define ARM_ip      uregs[12]
#define ARM_fp      uregs[11]
#define ARM_r10     uregs[10]
#define ARM_r9      uregs[9]
#define ARM_r8      uregs[8]
#define ARM_r7      uregs[7]
#define ARM_r6      uregs[6]
#define ARM_r5      uregs[5]
#define ARM_r4      uregs[4]
#define ARM_r3      uregs[3]
#define ARM_r2      uregs[2]
#define ARM_r1      uregs[1]
#define ARM_r0      uregs[0]
#define ARM_ORIG_r0 uregs[17]
#define CPSR_T_MASK                       1<<5

typedef pid_t tid_t;
typedef struct pt_regs {
    unsigned long uregs[18];
} pt_regs;
typedef struct user_pt_regs user_pt_regs;
typedef struct Tools {
    int (*attach) (tid_t);
    int (*cont) (tid_t);
    int (*detach) (tid_t);
    int (*ptrace_read) (pid_t, void*, uint8_t*, size_t);
    int (*readString) (pid_t, void*, uint8_t*);
    int (*ptrace_write) (pid_t, void*, uint8_t*, size_t);
    int (*getRegs) (tid_t, void*);
    int (*setRegs) (tid_t, void*);
    int (*call) (tid_t, long, long*, int, void*);
} Tools;

static pthread_mutex_t mutex;

Tools createTools(int arch);

#endif //NATIVEINJECTOR_TOOLS_H
