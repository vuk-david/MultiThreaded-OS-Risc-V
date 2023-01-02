//
// Created by os on 8/2/22.
//

#ifndef PROJECT_BASE_KERNELSEM_HPP
#define PROJECT_BASE_KERNELSEM_HPP
#include "../h/pcb.hpp"
#include "../h/list.hpp"
#include "../h/scheduler.hpp"
class PCB;
class KernelSem
{
public:
    KernelSem(unsigned int init);
    ~KernelSem();

    static int sem_open(KernelSem **handle, unsigned int init);
    static int sem_close(KernelSem *handle);

    static int wait(KernelSem *id);
    static int signal(KernelSem *id);
    int getSemValue() {return value;}
    void setSemValue(unsigned int v) {value = v;}
private:
    int value;
    bool allocated;
    List *blockedThreads;
};
#endif //PROJECT_BASE_KERNELSEM_HPP
