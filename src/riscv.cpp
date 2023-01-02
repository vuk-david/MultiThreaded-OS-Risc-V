//
// Created by os on 7/24/22.
//
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/pcb.hpp"
#include "../lib/mem.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../test/printing.hpp"


void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if(scause == 0x0000000000000009UL || scause == 0x0000000000000008UL)
    {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        uint64 type;
        __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(type));
        switch (type)
        {
            case 1:
            {
                size_t forAllocation;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(forAllocation));
                void *addr = (void*)MemoryAllocator::mem_alloc(forAllocation);
                uint64 *address = (uint64*)addr;
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(address));
                break;
            }
            case 2:
            {
                void *address = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(address));
                int result = MemoryAllocator::mem_free(address);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 3:
            {
                uint64 volatile sepc=r_sepc() + 4;
                mc_sstatus(SSTATUS_SPP);
                uint64 volatile status=r_sstatus();
                w_sepc(sepc);
                w_sstatus(status);
                __asm__ volatile("sret");
            }
            case 4:
            {
                uint64 volatile sepc=r_sepc() + 4;
                ms_sstatus(SSTATUS_SPP);
                uint64 volatile status=r_sstatus();
                w_sepc(sepc);
                w_sstatus(status);
                __asm__ volatile("sret");
            }
            case 11:
            {
                thread_t *handle = nullptr;
                Body body = nullptr;
                void *arg = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(handle));
                __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(body));
                __asm__ volatile ("mv %[a7], a7" : [a7] "=r"(arg));
                size_t forAllocation = (DEFAULT_STACK_SIZE * sizeof(uint64));
                size_t inBlocks = (forAllocation % MEM_BLOCK_SIZE > 0) ? (forAllocation / MEM_BLOCK_SIZE + 1) : (forAllocation / MEM_BLOCK_SIZE);
                void *stack_space = (void*) MemoryAllocator::mem_alloc(inBlocks);
                int result = PCB::thread_create(handle, body, arg, stack_space);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 12:
            {
                int result = PCB::thread_exit();
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 13:
            {
                PCB::dispatch();
                break;
            }
            case 14:
            {
                thread_t *handle = nullptr;
                Body body = nullptr;
                void *arg = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(handle));
                __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(body));
                __asm__ volatile ("mv %[a7], a7" : [a7] "=r"(arg));
                size_t forAllocation = (DEFAULT_STACK_SIZE * sizeof(uint64));
                size_t inBlocks = (forAllocation % MEM_BLOCK_SIZE > 0) ? (forAllocation / MEM_BLOCK_SIZE + 1) : (forAllocation / MEM_BLOCK_SIZE);
                void *stack_space = (void*) MemoryAllocator::mem_alloc(inBlocks);
                PCB::only_create_thread(handle, body, arg, stack_space);
                break;
            }
            case 15:
            {
                thread_t handle = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(handle));
                int result = PCB::start_thread(handle);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 21:
            {
                sem_t *handle = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(handle));
                unsigned int init = 0;
                __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(init));
                int result = KernelSem::sem_open(handle, init);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 22:
            {
                sem_t handle = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(handle));
                int result = KernelSem::sem_close(handle);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 23:
            {
                sem_t id = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(id));
                int result = KernelSem::wait(id);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 24:
            {
                sem_t id = nullptr;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(id));
                int result = KernelSem::signal(id);
                __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(result));
                break;
            }
            case 41:
            {
                __getc();
                break;
            }
            case 42:
            {
                char c = 0;
                __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(c));
                __putc(c);
                break;
            }
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000009UL) //console
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL) //timer
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        mc_sip(SIP_SSIP);
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x0000000000000005UL)
    {
        printString("load access fault\n");
    }
    else if (scause == 0x0000000000000007UL)
    {
        printString("store access fault\n");
    }
    else if (scause == 0x0000000000000002UL)
    {
        printString("illegal instruction\n");
    }
    else
    {

    }
}