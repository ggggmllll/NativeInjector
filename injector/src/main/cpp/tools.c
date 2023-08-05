//
// Created by zfb457 on 2023/8/4.
//

#include "tools.h"

int ptrace_attach(tid_t tid) {
    pthread_mutex_lock(&mutex);

    if (ptrace(PTRACE_ATTACH, tid, NULL, 0) < 0) {
        perror("attach");
        err = ERR_ATTACH;
        return -1;
    }

    waitpid(tid, NULL, WUNTRACED);
    return 0;
}

int ptrace_cont(tid_t tid) {

    if (ptrace(PTRACE_CONT, tid, NULL, 0) < 0) {
        perror("cont");
        err = ERR_CONT;
        return -1;
    }

    return 0;
}

int ptrace_detach(tid_t tid) {

    if (ptrace(PTRACE_DETACH, tid, NULL, 0) < 0) {
        perror("detach");
        err = ERR_DETACH;
        return -1;
    }

    return 0;
}

int ptrace_read(pid_t pid, void *addr, void *buf, size_t size) {
    uint32_t i, j, remain;
    uint8_t *laddr, *uaddr;

    union u {
        long val;
        char chars[sizeof(long)];
    } d;

    remain = size % 4;
    j = size / 4;
    laddr = buf;
    uaddr = (uint8_t *) addr;

    for (i = 0; i < j; i++) {
        d.val = ptrace(PTRACE_PEEKTEXT, pid, uaddr, NULL);
        memcpy(laddr, d.chars, 4);
        uaddr += 4;
        laddr += 4;
    }

    if (remain > 0) {
        d.val = ptrace(PTRACE_PEEKTEXT, pid, uaddr, d.val);
        memcpy(laddr, d.chars, remain);
    }

    return 0;
}

int ptrace_readString(pid_t pid, void *addr, void *buf) {
    void *saddr;
    int len;
    char chr;
    saddr = addr;

    do {
        if (ptrace_read(pid, addr, &chr, sizeof(char)) < 0) {
            return -1;
        }
        addr += sizeof(char);
        len++;
    } while (chr != '\0');

    len++;
    addr = saddr;

    if (ptrace_read(pid, addr, buf, len) == -1) {
        return -1;
    }

    return 0;
}

int ptrace_write(pid_t pid, void *addr, void *data, size_t size) {
    uint32_t i, j, remain;
    uint8_t *laddr, *uaddr;

    union u {
        long val;
        char chars[sizeof(long)];
    } d;

    laddr = data;
    j = size / 4;
    remain = size % 4;
    uaddr = (uint8_t *) addr;

    for (i = 0; i < j; i++) {
        memcpy(d.chars, laddr, 4);
        ptrace(PTRACE_POKETEXT, pid, uaddr, d.val);
        uaddr += 4;
        laddr += 4;
    }

    if (remain > 0) {
        memcpy(d.chars, laddr, remain);
        ptrace(PTRACE_POKETEXT, pid, uaddr, d.val);
    }

    return 0;
}

int ptrace_getReg_arm32(tid_t tid, void* regs)
{
    struct iovec iov;
    iov.iov_base = regs;
    iov.iov_len = sizeof(pt_regs);

    if (ptrace(PTRACE_GETREGSET, tid, NT_PRSTATUS, &iov) < 0)
    {
        err = ERR_REGS;
        perror("get regs");
        return -1;
    }

    return 0;
}

int ptrace_getReg_arm64(tid_t tid, void* regs)
{
    struct iovec iov;
    iov.iov_base = regs;
    iov.iov_len = sizeof(user_pt_regs);

    if (ptrace(PTRACE_GETREGSET, tid, NT_PRSTATUS, &iov) < 0)
    {
        err = ERR_REGS;
        perror("get regs");
        return -1;
    }

    return 0;
}

int ptrace_setReg_arm32(tid_t tid, void* regs)
{
    struct iovec iov;
    iov.iov_base = regs;
    iov.iov_len = sizeof(pt_regs);

    if (ptrace(PTRACE_SETREGSET, tid, NT_PRSTATUS, &iov) < 0)
    {
        err = ERR_REGS;
        perror("set regs");
        return -1;
    }

    return 0;
}

int ptrace_setReg_arm64(tid_t tid, void* regs)
{
    struct iovec iov;
    iov.iov_base = regs;
    iov.iov_len = sizeof(user_pt_regs);

    if (ptrace(PTRACE_SETREGSET, tid, NT_PRSTATUS, &iov) < 0)
    {
        err = ERR_REGS;
        perror("set regs");
        return -1;
    }

    return 0;
}

int ptrace_call_arm32(tid_t tid, long addr, long* parms, int num, void* regs)
{
    uint32_t i;
    int ret = -1;

    for (i = 0; i < num && i<4; i++)
    {
        ((pt_regs*) regs)->uregs[i] = parms[i];
    }

    if (i< num)
    {
        ((pt_regs*) regs)->ARM_sp -= (num - i) * sizeof(long);
        ptrace_write(tid, (void *)(((pt_regs*) regs)->ARM_sp), (uint8_t*)&parms[i], (num - i) * sizeof(long));
    }

    ((pt_regs*) regs)->ARM_pc = addr;

    if (((pt_regs*) regs)->ARM_pc & 1)
    {
        ((pt_regs*) regs)->ARM_pc &= (~1u);
        ((pt_regs*) regs)->ARM_cpsr |= CPSR_T_MASK;
    }
    else
    {
        ((pt_regs*) regs)->ARM_cpsr &= (~CPSR_T_MASK);
    }

    ((pt_regs*) regs)->ARM_lr = 0;

    if (ptrace_setReg_arm32(tid, (pt_regs*) regs) == -1 || ptrace_cont(tid) == -1)
    {
        err = ERR_CALL;
        perror("call");
        goto EXIT;
    }

    waitpid(tid,NULL,WCONTINUED);

    int status=0;

    while (WSTOPSIG(status) != SIGSEGV)
    {
        waitpid(tid,&status,WUNTRACED);
    }

    ret = 0;

EXIT:
    return ret;
}

int ptrace_call_arm64(tid_t tid, long addr, long* parms, int num, void* regs)
{
    uint32_t i;
    int ret = -1;

    for (i = 0; i < num && i<8; i++)
    {
        ((user_pt_regs*) regs)->regs[i] = parms[i];
    }

    if (i< num)
    {
        ((user_pt_regs*) regs)->sp -= (num - i) * sizeof(__u64);
        ptrace_write(tid, (void *)(((user_pt_regs*) regs)->sp), (uint8_t*)&parms[i], (num - i) * sizeof(__u64));
    }

    ((user_pt_regs*) regs)->pc = addr;
    ((user_pt_regs*) regs)->regs[29] = 0;

    if (ptrace_setReg_arm64(tid, regs) == -1 || ptrace_cont(tid) == -1)
    {
        err = ERR_CALL;
        perror("call\n");
        goto EXIT;
    }

    waitpid(tid,NULL,WCONTINUED);

    int status=0;

    while (WSTOPSIG(status)!=SIGSEGV)
    {
        waitpid(tid,&status,WUNTRACED);
    }

EXIT:
    return ret;
}

Tools createTools(int arch) {
    Tools tools;
    tools.attach = ptrace_attach;
    tools.detach = ptrace_detach;
    tools.cont = ptrace_cont;
    tools.ptrace_read = ptrace_read;
    tools.ptrace_write = ptrace_write;

    if (arch == ARCH_ARM32) {
        tools.getRegs = ptrace_getReg_arm32;
        tools.setRegs = ptrace_setReg_arm32;
        tools.call = ptrace_call_arm32;
        return tools;
    }

    tools.getRegs = ptrace_getReg_arm64;
    tools.setRegs = ptrace_setReg_arm64;
    tools.call = ptrace_call_arm64;
    return tools;
}