//
// Created by os on 7/23/22.
//
#include "../h/MemoryAllocator.h"

FreeMem* MemoryAllocator::freeMemHead = nullptr;
FreeMem* MemoryAllocator::allocatedHead = nullptr;

void* MemoryAllocator::mem_alloc(size_t size) //size mi je blokovima
{
    if(freeMemHead == nullptr)
    {
        freeMemHead = (FreeMem*)HEAP_START_ADDR;
        freeMemHead->size = (size_t)((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR);
        freeMemHead->next = nullptr;
    }

    size_t forAllocation = size * MEM_BLOCK_SIZE;

    FreeMem *prev = nullptr;
    for(FreeMem *cur = freeMemHead; cur != nullptr || ((char*)cur + forAllocation) < HEAP_END_ADDR; prev = cur, cur = cur->next) {
        if (cur->size < forAllocation)
            continue;
        //ako mi nakom alokacije ostaje deo koji je veci moram da napravim novi slobodan fragment
        if((cur->size - forAllocation) > 0)
        {
            FreeMem *newFragment = (FreeMem*)((char*)cur + forAllocation + sizeof(FreeMem));
            if(prev) prev->next = newFragment;
            else freeMemHead = newFragment;
            newFragment->next = cur->next;
            newFragment->size = cur->size - forAllocation - sizeof(FreeMem);
            if(allocatedHead == nullptr || allocatedHead == freeMemHead)
            {
                allocatedHead = cur;
                cur->next = nullptr;
                cur->size = forAllocation;
            }
            else
            {
                FreeMem *preth = allocatedHead;
                if((char*)cur < (char*)preth)
                {
                    allocatedHead = cur;
                    allocatedHead->next = preth;
                    allocatedHead->size = cur->size;
                }
                else
                {
                    if(allocatedHead->next == nullptr)
                    {
                        FreeMem *pom = allocatedHead;
                        if((char*)pom > (char*)cur)
                        {
                            allocatedHead = cur;
                            allocatedHead->next = pom;
                            allocatedHead->size = cur->size;
                            pom->next = nullptr;
                        }
                        else
                        {
                            pom->next = cur;
                            cur->next = nullptr;
                            cur->size = forAllocation;
                        }
                    }
                    else
                    {
                        while(preth->next != nullptr && (char*)preth->next < (char*)cur)
                            preth = preth->next;
                        cur->next = preth->next;
                        cur->size = forAllocation;
                        preth->next = cur;
                    }
                }
            }
            return (void*)((char*)cur + sizeof(FreeMem));
        }
        //ako ga smestam na mesto koje je taman
        else
        {
            if(prev) prev->next = cur->next;
            else freeMemHead = cur->next;

            if(allocatedHead == nullptr)
            {
                allocatedHead = cur;
                cur->next = nullptr;
                cur->size = forAllocation;
            }
            else
            {
                FreeMem *preth = allocatedHead;
                if((char*)cur < (char*)preth)
                {
                    allocatedHead = cur;
                    allocatedHead->next = preth;
                    allocatedHead->size = cur->size;
                }
                else
                {
                    if(allocatedHead->next == nullptr)
                    {
                        FreeMem *pom = allocatedHead;
                        if((char*)pom > (char*)cur)
                        {
                            allocatedHead = cur;
                            allocatedHead->next = pom;
                            allocatedHead->size = cur->size;
                            pom->next = nullptr;
                        }
                        else
                        {
                            pom->next = cur;
                            cur->next = nullptr;
                            cur->size = forAllocation;
                        }
                    }
                    else
                    {
                        while(preth->next != nullptr && (char*)preth->next < (char*)cur)
                            preth = preth->next;
                        cur->next = preth->next;
                        cur->size = forAllocation;
                        preth->next = cur;
                    }
                }
            }
            return (void*)((char*)cur + sizeof(FreeMem));
        }
    }
    //ako uopste nisam uspela da nadjem mesto za njega vracam null
    return nullptr;
}

int MemoryAllocator::mem_free(void *addr)
{
    char *address = ((char*)addr - sizeof(FreeMem));

    FreeMem *cur = allocatedHead, *prev = nullptr;
    if(!allocatedHead || address < (char*)allocatedHead)
        cur = nullptr;
    else
    {
        while(cur->next != nullptr && address != (char *) (cur))
        {
            prev = cur;
            cur = cur->next;
        }
    }

    if(cur) {
        if(prev)
            prev->next = cur->next;
        else {
            if(cur->next == nullptr)
            {
                allocatedHead = freeMemHead;
            }
            else
            {
                allocatedHead = cur->next;
            }
        }
    }
    else return -1;
    FreeMem *myBlock = cur;
    //insertovanje u listu slobodnih ako mi NIJE PRAZNA
    if(freeMemHead)
    {
        //u slucaju da imam samo jedan element da bih posle mogla da krenem prolazak kroz listu izdvojila sam ovaj slucaj
        if(freeMemHead->next == nullptr)
        {
            FreeMem *pom = freeMemHead;
            if((char*)pom > (char*)myBlock)
            {
                freeMemHead = myBlock;
                freeMemHead->next = pom;
                freeMemHead->size = myBlock->size;
                pom->next = nullptr;
            }
            else
            {
                pom->next = myBlock;
                myBlock->next = nullptr;
            }
        }
        //imam vise od jednog elementa u listi
        else
        {
            FreeMem *preth = freeMemHead, *sled = freeMemHead->next;
            if((char*)myBlock < (char*)preth)
            {
                freeMemHead = myBlock;
                freeMemHead->next = preth;
                freeMemHead->size = myBlock->size;
                //pokusaj da spojis sa svojim ispred
                if((char*)freeMemHead + freeMemHead->size + sizeof(FreeMem) == (char*)preth)
                {
                    freeMemHead->next = preth->next;
                    freeMemHead->size = freeMemHead->size + preth->size + sizeof(FreeMem);
                }
            }
            else
            {
                while(preth->next != nullptr && (char*)preth->next < (char*)myBlock)
                    preth = preth->next;
                myBlock->next = preth->next;
                preth->next = myBlock;
                sled = myBlock->next;

                //ako mi je i poslednji null onda ubacujem ovo na kraj liste
                if(sled == nullptr)
                {
                    if((char*)myBlock + myBlock->size > (char*)HEAP_END_ADDR)
                        return -2; //ne mogu da ga ubacim jer mi prelazi velicinu hipa
                    else
                    {
                        sled->next = myBlock;
                        myBlock->next = nullptr;
                    }
                }
                //ako nije ubacujem izmedju ova dva sto sam nasla
                else
                {
                    //proveravam da li mogu da spojim
                    //pokusavam da spojim sa ovim sto mi je prethodni
                    if(preth->next && ((char*)preth + preth->size + sizeof(FreeMem)) == (char*)myBlock)
                    {
                        preth->size = preth->size + myBlock->size + sizeof(FreeMem);
                        preth->next = myBlock->next;
                        myBlock->next = nullptr;
                        //da proverim da li moze sad i sa ovim ispred?
                        if(preth->next && ((char*)preth + preth->size + sizeof(FreeMem)) == (char*)sled)
                        {
                            preth->size = preth->size + sled->size + sizeof(FreeMem);
                            preth->next = sled->next;
                            sled->next = nullptr;
                        }
                    }
                    //pokusavam da spojim sa ovim sto mi je sledeci
                    else
                    {
                        if(((char*)myBlock + myBlock->size + sizeof(FreeMem)) == (char*)sled)
                        {
                            myBlock->size = myBlock->size + sled->size + sizeof(FreeMem);
                            myBlock->next = sled->next;
                            sled->next = nullptr;
                        }
                    }
                }
            }
        }
    }
    //ako mi je freemem null onda ga samo stavim na myblock
    else
    {
        freeMemHead = myBlock;
        freeMemHead->size = myBlock->size;
        freeMemHead->next = nullptr;
    }
    return 0;
}