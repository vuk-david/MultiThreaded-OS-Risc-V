//
// Created by os on 8/2/22.
//
#include "../h/kernelSem.hpp"

KernelSem::KernelSem(unsigned int init)
{
    this->value = (int)init;
    this->blockedThreads = new List();
    this->allocated = true;
}

KernelSem::~KernelSem()
{
    delete this->blockedThreads;
}

int KernelSem::sem_open(KernelSem **handle, unsigned int init)
{
    KernelSem *temporaryHandle = new KernelSem(init);
    *handle = temporaryHandle;
    return (handle == nullptr) ? -1 : 0;
}

int KernelSem::sem_close(KernelSem *handle)
{
    if(handle == nullptr)
        return -1;

    size_t size = handle->blockedThreads->getListSize();
    while(size > 0)
    {
        PCB *found = handle->blockedThreads->removeFirst();
        found->setStatus(READY);
        Scheduler::put(found);
        size--;
    }
    handle->allocated = false;
    return 0;
}

int KernelSem::wait(KernelSem *id)
{
    if(id == nullptr || !id->allocated)
        return -1;
    --id->value;
    if(id->getSemValue() < 0)
    {
        PCB::running->setStatus(BLOCKED);
        id->blockedThreads->addLast(PCB::running);
        PCB::dispatch();
    }
    return 0;
}

int KernelSem::signal(KernelSem *id)
{
    if(id == nullptr)
        return -1;
    if(id->getSemValue() < 0)
    {
        if(id->blockedThreads->getListSize() != 0)
        {
            PCB *found = id->blockedThreads->removeFirst();
            found->setStatus(READY);
            Scheduler::put(found);
        }
    }
    id->value++;
    return 0;
}