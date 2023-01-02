//
// Created by os on 7/28/22.
//
#include "../h/scheduler.hpp"

List Scheduler::queue;

void Scheduler::put(PCB *p)
{
    queue.addLast(p);
}

PCB* Scheduler::get()
{
    return queue.removeFirst();
}

void Scheduler::remove(PCB *p)
{
    queue.remove(p);
}