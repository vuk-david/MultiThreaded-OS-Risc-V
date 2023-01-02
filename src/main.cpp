//
// Created by os on 8/9/22.
//

//#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/pcb.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.h"

extern void userMain();
void userWrapper(void *) {
    userMain() ;
}

void main()
{
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_t mainThread;
    PCB::thread_create(&mainThread, nullptr, nullptr, nullptr);

    size_t forAllocation = (DEFAULT_STACK_SIZE * sizeof(uint64));
    size_t inBlocks = (forAllocation % MEM_BLOCK_SIZE > 0) ? (forAllocation / MEM_BLOCK_SIZE + 1) : (forAllocation / MEM_BLOCK_SIZE);
    void *stack_space = (void*) MemoryAllocator::mem_alloc(inBlocks);
    thread_t userMainThread;
    PCB::thread_create(&userMainThread, userWrapper, nullptr, stack_space);

    PCB::running = mainThread;
    PCB::dispatch();

    int size = PCB::numberOfThreads;
    while(1)
    {
        int finished = PCB::numberOfFinishedThreads;
        if(finished == (size - 1))
        {
            break;
        }
        else
        {
            PCB::dispatch();
        }
    }
}