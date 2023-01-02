//
// Created by os on 7/28/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "../h/list.hpp"
#include "../h/pcb.hpp"
class PCB;
class Scheduler
{
private:
    static List queue;
public:
    static void put(PCB *p);
    static PCB *get();
    static void remove(PCB *p);
};

#endif //PROJECT_BASE_SCHEDULER_HPP
