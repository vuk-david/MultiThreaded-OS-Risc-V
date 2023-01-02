//
// Created by os on 7/28/22.
//
#include "../h/pcb.hpp"
#include "../h/scheduler.hpp"

int PCB::ID = 0;
PCB* PCB::running = nullptr;
int PCB::numberOfFinishedThreads = 0;
int PCB::numberOfThreads = 0;

PCB::PCB(Body body, void *arg, uint64 *stack_space)
{
    this->body = body;
    this->arg = arg;
    this->stackSize = DEFAULT_STACK_SIZE;
    this->stackPointer = (body != nullptr) ? stack_space : nullptr;
    this->timeSlice = DEFAULT_TIME_SLICE;
    this->finished = false;
    this->context = {
            (uint64) &threadWrapper,
            stackPointer != nullptr ? (uint64)&stackPointer[DEFAULT_STACK_SIZE] : 0
    };
    this->id = ++ID;
    this->status = CREATED;

    PCB::numberOfThreads++;
}

PCB::~PCB()
{
    delete [] stackPointer;
}

int PCB::thread_create(PCB **handle, Body body, void *arg, void *stack_space)
{
    PCB *temporaryHandle = new PCB(body, arg, (uint64*)stack_space);
    *handle = temporaryHandle;
    if(body != nullptr)
        Scheduler::put(temporaryHandle);
    return handle == nullptr ? -1 : 0;
}

void PCB::only_create_thread(PCB **handle, Body body, void *arg, void *stack_space)
{
    PCB *temporaryHandle = new PCB(body, arg, (uint64*)stack_space);
    *handle = temporaryHandle;
}

int PCB::start_thread(PCB *handle)
{
    if (handle) {
        Scheduler::put(handle);
        return 0;
    }
    return -1;
}

int PCB::thread_exit()
{
    if(!running->getFinished())
    {
        running->setFinished(true);
        running->setStatus(FINISHED);
	PCB::numberOfFinishedThreads++;
        return 0;
    }
    return -1;
}

void PCB::dispatch()
{
    PCB *old = running;
    if(!old->getFinished() && old->getStatus() != ThreadStatus::BLOCKED) {Scheduler::put(old);}
    running = Scheduler::get();

    PCB::contextSwitch(&old->context, &running->context);
}

void PCB::threadWrapper()
{
    if(running->getID() >= 2)
    {
        uint64 a0 = 3;
        __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
        __asm__ volatile ("ecall");
    }
    running->body(running->arg);
    if(running->getID() >= 2)
    {
        uint64 a0 = 4;
        __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
        __asm__ volatile ("ecall");
    }
    thread_exit();
}