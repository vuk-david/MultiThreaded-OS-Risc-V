//
// Created by os on 7/28/22.
//
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"

void* operator new (size_t n)
{
    return mem_alloc(n);
}

void* operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *ptr)
{
    mem_free(ptr);
}

void operator delete[](void *ptr)
{
    mem_free(ptr);
}

void wrapper(void *arg)
{
    if(arg)
        ((Thread*)arg)->run();
}

Thread::Thread(void (*body)(void *), void *arg)
{
    this->myHandle = nullptr;
    only_create_thread(&myHandle, body, arg);
}

Thread::Thread()
{
    this->myHandle = nullptr;
    only_create_thread(&myHandle, wrapper, this);
}

Thread::~Thread()
{
    this->myHandle = nullptr;
    delete myHandle;
}

int Thread::start()
{
    int pid = -1;
    if(myHandle)
        pid = start_thread(myHandle);
    return pid;
}

void Thread::dispatch()
{
    thread_dispatch();
}

Semaphore::Semaphore(unsigned int init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
    delete myHandle;
}

int Semaphore::wait()
{
    if (sem_wait(myHandle))
        return 0;
    else return -1;
}

int Semaphore::signal()
{
    if (sem_signal(myHandle))
        return 0;
    else return -1;
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}

