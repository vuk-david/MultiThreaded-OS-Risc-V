//
// Created by os on 7/28/22.
//
#include "../h/syscall_c.h"
#include "../lib/hw.h"

void *mem_alloc(size_t size)
{
    uint64 a0 = 1;
    size_t sizeInBlocks = (size % MEM_BLOCK_SIZE > 0) ? (size / MEM_BLOCK_SIZE + 1) : (size / MEM_BLOCK_SIZE);
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(sizeInBlocks));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    uint64 address = 0;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(address));
    return (uint64*)address;
}

int mem_free(void *address)
{
    uint64 a0 = 2;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(address));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int thread_create (
        thread_t* handle,
        Body body,
        void* arg)
{
    uint64 a0 = 11;
    asm volatile ("mv a7, %[a7]" : : [a7] "r"(arg));
    asm volatile ("mv a2, %[a2]" : : [a2] "r"(body));
    asm volatile ("mv a1, %[a1]" : : [a1] "r"(handle));
    asm volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    asm volatile ("ecall");
    int result = -1;
    asm volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int thread_exit()
{
    uint64 a0 = 12;
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

void thread_dispatch()
{
    uint64 a0 = 13;
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
}

void only_create_thread (
        thread_t* handle,
        Body body,
        void* arg
)
{
    uint64 a0 = 14;
    __asm__ volatile ("mv a7, %[a7]" : : [a7] "r"(arg));
    __asm__ volatile ("mv a2, %[a2]" : : [a2] "r"(body));
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(handle));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
}

int start_thread(thread_t handle)
{
    uint64 a0 = 15;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(handle));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int sem_open (
        sem_t* handle,
        unsigned init
)
{
    uint64 a0 = 21;
    __asm__ volatile ("mv a2, %[a2]" : : [a2] "r"(init));
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(handle));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int sem_close (sem_t handle)
{
    uint64 a0 = 22;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(handle));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int sem_wait (sem_t id)
{
    uint64 a0 = 23;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(id));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

int sem_signal (sem_t id)
{
    uint64 a0 = 24;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(id));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    int result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

char getc()
{
    uint64 a0 = 41;
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
    char result = -1;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(result));
    return result;
}

void putc(char c)
{
    uint64 a0 = 42;
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(c));
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
    __asm__ volatile ("ecall");
}