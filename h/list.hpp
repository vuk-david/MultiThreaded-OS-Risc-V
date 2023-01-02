//
// Created by os on 8/1/22.
//

#ifndef PROJECT_BASE_LIST_HPP
#define PROJECT_BASE_LIST_HPP
#include "../lib/hw.h"
#include "../h/pcb.hpp"
class PCB;
class List
{
private:
    struct Elem
    {
        PCB *data;
        Elem *next;

        Elem(PCB *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;
    size_t size;
public:
    List() : head(0), tail(0), size(0) {}
    List(const List &) = delete;

    List &operator=(const List &) = delete;

    size_t getListSize() {return size;}

    void addFirst(PCB *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
        size++;
    }

    void addLast(PCB *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
        size++;
    }

    PCB *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        PCB *ret = elem->data;
        delete elem;
        size--;
        return ret;
    }

    PCB *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    PCB *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        PCB *ret = elem->data;
        delete elem;
        size--;
        return ret;
    }

    PCB *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

    void remove(PCB *data)
    {
        Elem *prev = nullptr;
        Elem *cur = (Elem*)head;
        while(cur->data != data)
        {
            prev = cur; cur = cur->next;
        }
        if(prev == 0)
        {
            head = head->next;
            if(head == nullptr)
                tail = nullptr;
        }
        else
        {
            if(cur == tail)
                tail = prev;
            prev->next = cur->next;
            cur->next = 0;
        }
        delete cur;
        size--;
    }
};
#endif //PROJECT_BASE_LIST_HPP
