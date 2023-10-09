#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

// struct queue{
//     struct proc* front;
//     struct proc* rear;
//     uint totalProcess;
//     uint slicetime;
// };

struct queue queues[4];
void initialize_queue()
{
    for (int i = 0; i < 4; i++)
    {
        queues[i].front = 0;
        queues[i].rear = 0;
        queues[i].totalProcess = 0;
        if (i == 0)
            queues[i].slicetime = 1;
        else if (i == 1)
            queues[i].slicetime = 3;
        else if (i == 2)
            queues[i].slicetime = 9;
        else if (i == 3)
            queues[i].slicetime = 15;
    }
}
void insert_queue(struct proc *p, uint queue_num, int flag,int fr)
{
    if (fr == 1)
    {
        if (queues[queue_num].rear == 0)
        {
            queues[queue_num].rear = p;
            p->next_queue = 0;
            p->prev_queue = 0;
            if (flag == 1)
            {
                p->queue_enter_time = ticks;
                p->ticks_used = 0;
            }
            p->queue_num = queue_num;
            p->inqueue = 1;
            if (queues[queue_num].front == 0)
                queues[queue_num].front = p;
            queues[queue_num].totalProcess++;
        }
        else
        {
            p->next_queue = 0;
            p->prev_queue = queues[queue_num].rear;
            queues[queue_num].rear->next_queue = p;
            queues[queue_num].rear = p;
            if (flag == 1)
            {
                p->queue_enter_time = ticks;
                p->ticks_used = 0;
            }
            p->queue_num = queue_num;
            p->inqueue = 1;
            queues[queue_num].totalProcess++;
        }
    }
    else{
        if(queues[queue_num].front==0)
        {
            queues[queue_num].front=p;
            p->next_queue = 0;
            p->prev_queue = 0;
            if (flag == 1)
            {
                p->queue_enter_time = ticks;
                p->ticks_used = 0;
            }
            p->queue_num = queue_num;
            p->inqueue = 1;
            if (queues[queue_num].rear == 0)
                queues[queue_num].rear = p;
            queues[queue_num].totalProcess++;
        }
        else{
            p->next_queue=queues[queue_num].front;
            queues[queue_num].front->prev_queue=p;
            p->prev_queue=0;
            queues[queue_num].front=p;
            if(flag==1)
            {
                p->queue_enter_time=ticks;
                p->ticks_used=0;
            }
            p->queue_num=queue_num;
            p->inqueue=1;
            queues[queue_num].totalProcess++;
        }
    }
}
void remove_queue(struct proc *p)
{
    if (queues[p->queue_num].totalProcess == 0)
    {
        queues[p->queue_num].front = 0;
        queues[p->queue_num].rear = 0;
        return;
    }
    struct proc *temp;
    temp = queues[p->queue_num].front;
    while (temp != 0)
    {
        if (temp == p)
        {
            if (temp->prev_queue != 0)
            {
                temp->prev_queue->next_queue = temp->next_queue;
            }
            if (temp->next_queue != 0)
            {
                temp->next_queue->prev_queue = temp->prev_queue;
            }
            if (temp->prev_queue == 0)
            {
                queues[p->queue_num].front = temp->next_queue;
            }
            if (temp->next_queue == 0)
                queues[p->queue_num].rear = temp->prev_queue;
            temp->prev_queue = 0;
            temp->next_queue = 0;
            temp->inqueue = 0;
            temp->no_retain = 1;
            temp->fr=1;
            queues[p->queue_num].totalProcess--;
            break;
        }
        temp = temp->next_queue;
    }
    if (queues[p->queue_num].totalProcess == 0)
    {
        queues[p->queue_num].front = 0;
        queues[p->queue_num].rear = 0;
    }
}