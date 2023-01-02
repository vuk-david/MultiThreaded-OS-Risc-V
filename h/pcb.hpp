//
// Created by os on 7/28/22.
//

#ifndef PROJECT_BASE_PCB_HPP
#define PROJECT_BASE_PCB_HPP
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/riscv.hpp"
#include "../h/list.hpp"
//#include "../h/syscall_cpp.hpp"
class List;
typedef enum
{
    CREATED = 0, READY, RUNNING, BLOCKED, SUSPENDED, FINISHED, IDLE
} ThreadStatus;

class PCB
{
public:
    using Body = void(*)(void *);

    ~PCB();

    static int thread_create(PCB **handle, Body body, void *arg, void *stack_space);
    static void only_create_thread(PCB **handle, Body body, void *arg, void *stack_space);
    static int start_thread(PCB *handle);
    static int thread_exit();
    static void dispatch();

    static PCB *running;

    int getID(){return id;}

    bool getFinished(){return finished;}
    void setFinished(bool finished){PCB::finished = finished;}

    ThreadStatus getStatus(){return status;}
    void setStatus(ThreadStatus status){PCB::status = status;}

    static int numberOfFinishedThreads;
    static int numberOfThreads;
private:
    explicit PCB(Body body, void *arg, uint64 *stack_space);
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    Body body;
    void *arg;
    Context context;
    bool finished;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);

    static int ID;
    int id;
    ThreadStatus status;
    size_t stackSize;
    time_t timeSlice;
    uint64 *stackPointer;
};

#endif //PROJECT_BASE_PCB_HPP
