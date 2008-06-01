/* The LiteOS Operating System Kernel */
/*
   The following is the license of LiteOS.
   This file is part of LiteOS.
   Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu
   LiteOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   LiteOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "threads.h"
#include "threaddata.h"
#include "scheduling.h"
#include "generictimer.h"
#include "packethandler.h"
#include "../utilities/eventlogger.h"
#include "../system/stdserial.h"
#include "../types/string.h"
#include "nodeconfig.h"
//#include "energycontrol.h"
#include "socket.h"
#include "../system/stdserial.h"
#include "../system/globaltiming.h"
#ifdef PLATFORM_AVR
#include "../platform/avr/avrhardware.h"
#endif
//the handles definition
extern radio_handle receivehandles[RECEIVE_HANDLE_NUM];
extern volatile mutex msend;
extern volatile mutex mserialsend;
extern volatile mutex *m_unlock;

//for energy accounting 
/*
   extern volatile uint16_t lastswitchtime_h; 
   extern volatile uint32_t lastswitchtime_l;
   extern volatile uint32_t interruptcost; 
   extern volatile uint8_t energyround; 
   volatile uint32_t cost1, cost2; 
 */
//stores the maximum thread bound updated when adding thread
volatile uint16_t *maxthreadrambound;

//Our thread table
thread thread_table[LITE_MAX_THREADS];
void (*thread_clear_func_table[LITE_MAX_THREADS]) ();
volatile thread *current_thread;
volatile uint16_t *old_stack_ptr;
volatile uint16_t *stackinterrupt_ptr;

//This is simply a way to track whether our task is running
volatile uint8_t thread_task_active;
typedef struct thread_create_block
{
    void (*fp) ();
    uint8_t *ram_start;
    uint8_t *stack_ptr;
    uint8_t priority;
    char *threadname;
} thread_create_block_type;
thread_create_block_type newthreadblock;

//then in thread_init, this data structure is initilized 
volatile mutex m_createthreadlock;

/*
   uint32_t timediff;
   uint32_t switchFromThreadTime , switchToThreadTime; 
   uint16_t counterFrom, counterTo; 
   #define READTIMINGOFFSET 0
   uint8_t countindex; 
 */
//-------------------------------------------------------------------------
void thread_init()
{
    uint8_t i;
    _atomic_t currentatomic;

    currentatomic = _atomic_start();
    nmemset(thread_table, 0, sizeof(thread) * LITE_MAX_THREADS);
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        thread_clear_func_table[i] = NULL;
    }
    m_createthreadlock.lock = m_createthreadlock.lockingthreadid =
        m_createthreadlock.waiting = 0;
    current_thread = 0;
    old_stack_ptr = 0;
    stackinterrupt_ptr = 0;
    thread_task_active = 0;
    maxthreadrambound = 0;
    _atomic_end(currentatomic);
    //    TimerM_Timer_start(9, TIMER_REPEAT, 1000);
}

//--------------------------------------------------------------------------
uint8_t is_thread()
{
    uint16_t SPvalue;
    asm volatile ("in %A0, 0x3d" "\n\t" "in %B0, 0x3e" "\n\t":"=r" (SPvalue):);

    if (SPvalue > (uint16_t) maxthreadrambound)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}                               // getSP

//  return (  !  ! current_thread );
//--------------------------------------------------------------------------
//return the data structure address of the newthreadblock for user to be used 
void *getNewThreadBlock()
{
    return (void *)&newthreadblock;
}

//--------------------------------------------------------------------------
//return the mutex address
void *getCreateThreadMutex()
{
    return (void *)&m_createthreadlock;
}

//--------------------------------------------------------------------------
void createThreadTaskPosted()
{
    create_thread(newthreadblock.fp, (uint16_t *) newthreadblock.ram_start,
                  (uint16_t *) newthreadblock.stack_ptr, 0,
                  newthreadblock.priority, newthreadblock.threadname, 0, 0);
}

//--------------------------------------------------------------------------
//call the task of creating thread based on the thread block information 
void createThreadTask()
{
    postTask(createThreadTaskPosted, 1);
}

//--------------------------------------------------------------------------
//Now adds the support for kernel built-in memory corrupt search and find 
//To extend to other platforms, this function prototype may need to be modified or encapsulated into modules 
//updated the romstart and romsize parameter in version 0.31 to support rom image loading conflict detection 
int create_thread(void (*fcn) (), uint16_t * ram_start, uint16_t * stack_ptr,
                  uint16_t staticdatasize, uint8_t priority, char *threadName,
                  uint16_t romstart, uint16_t romsize)
{
    int i;
    _atomic_t currentatomic;

    //used for handling the built-in memory corrupt detection
    uint16_t *kernelptr;

    //we do not allow threads context to create new threads
    if (is_thread())
    {
        return (0);
    }
    if (stack_ptr > maxthreadrambound)
    {
        maxthreadrambound = stack_ptr;
    }
    //First loop all the way through the table and find an empty slot 
    //computation time for space here 
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state == STATE_NULL)
        {
            break;
        }
    }
    //Cannot create thread, no resources... 
    if (i == LITE_MAX_THREADS)
    {
        return (0);
    }
    //Add the thread
    currentatomic = _atomic_start();
    //Populate the thread table
    current_thread = &thread_table[i];
    current_thread->sp = stack_ptr;
    current_thread->state = STATE_ACTIVE;
    current_thread->data.tp = fcn;
    current_thread->priority = priority;
    current_thread->remaincredits = priority;
    current_thread->ramstart = ram_start;
    current_thread->ramend = stack_ptr;
    current_thread->sizeofBss = staticdatasize;
    current_thread->romstart = romstart;
    current_thread->romsize = romsize;
    /*
       {
       uint8_t tempecb;
       ecb_block *tempptr; 
       tempecb =  get_ecb_block(); 
       tempptr =  get_ecb_address( tempecb ); 
       current_thread->ecbptr = tempptr;            
       }
     */
    //COPY file name 
    {
        uint8_t templen;

        templen = mystrlen(threadName);
        mystrncpy((char *)current_thread->threadName, (char *)threadName,
                  templen + 1);
    }
    //set up the corruption detection 
    kernelptr = (uint16_t *) ((uint8_t *) ram_start + staticdatasize);
    *kernelptr = 0xeeff;
    *(kernelptr + 1) = 0xeeff;
#ifdef PLATFORM_AVR
    //Prepare the fcn pointer on the new stack, so it can be executed
    PREPARE_REG_FOR_STACK();
#endif
    current_thread = 0;
    if (!thread_task_active)
    {
        postTask(thread_task, 2);
        thread_task_active = 1;
    }
    _atomic_end(currentatomic);
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_THREADCREATE
    addTrace(TRACE_THREADCREATE);
#endif
#endif
    return (1);
}

/* Set up the destroy thread function call */
void setThreadTerminateFunction(uint8_t currentindex, void (*fp) ())
{
    thread_clear_func_table[currentindex] = fp;
}

/* destroy_user_thread
 * This routine is only called when a users thread returns.
 * It removes it from the thread table.
 */
void destroy_user_thread()
{
    uint8_t *start, *end;
    _atomic_t currentatomic;
    int indexofthread;

    currentatomic = _atomic_start();
    current_thread->state = STATE_NULL;
    start = (uint8_t *) current_thread->ramstart;
    end = (uint8_t *) current_thread->ramend;
    deleteThreadRegistrationInReceiverHandles(start, end);
    indexofthread = getThreadIndexAddress();
    releaseMutexLockUponThreadKill(indexofthread);
    if (thread_clear_func_table[indexofthread] != NULL)
    {
        (*thread_clear_func_table[indexofthread]) ();
        thread_clear_func_table[indexofthread] = NULL;
    }
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_THREADDESTROY
    addTrace(TRACE_THREADDESTROY);
#endif
#endif
    thread_yield();
    _atomic_end(currentatomic);
}

// These are new routines
/* lite_switch_to_user_thread()
 * This routine swaps the stack and allows a thread to run.
 */
void __attribute__ ((noinline)) lite_switch_to_user_thread()    /* __attribute__(
                                                                   (naked)) */
{
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_CONTEXTSWITCH
    addTrace(TRACE_CONTEXTSWITCHTOUSERTHREAD);
#endif
#endif
#ifdef PLATFORM_AVR
    PUSH_REG_STATUS();
    PUSH_GPR();
    SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);
    POP_GPR();
    POP_REG_STATUS();
#endif
    //_enable_interrupt(); 
    return;
}

//-------------------------------------------------------------------------
void __attribute__ ((noinline)) thread_yield()  /* __attribute__((noinline)) */
{
#ifdef PLATFORM_AVR
    PUSH_REG_STATUS();
    PUSH_GPR();
    //Now swap the stacks back
    SWAP_STACK_PTR(current_thread->sp, old_stack_ptr);
    POP_GPR();
    POP_REG_STATUS();
    //   switchFromThreadTime = getCurrentResolution();
    //   timediff = switchFromThreadTime - switchToThreadTime - READTIMINGOFFSET; 
    /*
       if (current_thread -> remainingenergy > timediff /1000000 * CYCLEENERGY)
       current_thread  -> remainenergy -= timediff / 1000000 * CYCLEENERGY; 
       else
       {
       current_thread->remainenergy = 0; 
       current_thread->state = STATE_SUSPEND; 
       }
     */
#endif
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_CONTEXTSWITCH
    addTrace(TRACE_CONTEXTSWITCHFROMUSERTHREAD);
#endif
#endif
}

/* thread_sleep
 * This routine puts the current thread into a sleeping state.
 * It will not ever wake up until another task or thread wakes it up
 */
void sleepThread(uint16_t milli)
{
    if (!is_thread())
    {
        return;
    }
    //this is insid the thread!
    current_thread->state = STATE_PRESLEEP;
    current_thread->data.sleepstate.sleeptime = milli;
    thread_yield();
}

//This routine is called to perform system level utility change and schedules thread_task again 

void threads_handle_service() /*__attribute__((noinline))*/
{
    uint8_t i;
    uint8_t thread_presleep;
    _atomic_t currentatomic;

    currentatomic = _atomic_start();
    thread_presleep = 0;
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state == STATE_PRESLEEP)
        {
            break;
        }
    }
    thread_presleep = (i != LITE_MAX_THREADS);
    if (thread_presleep)
    {
        postTask(threads_handle_service, 3);
        thread_table[i].state = STATE_SLEEP;
        _atomic_end(currentatomic);
        // TimerM_Timer_start( i, TIMER_ONE_SHOT, thread_table[ i ].data.sleepstate.sleeptime );
        GenericTimerStart(i, TIMER_ONE_SHOT,
                          thread_table[i].data.sleepstate.sleeptime);
        thread_presleep = 0;
        return;
    }
    postTask(thread_task, 2);
    _atomic_end(currentatomic);
}

//-------------------------------------------------------------------------
uint8_t memory_conflict_detect(uint16_t createflashromstart, uint16_t
                               createflashromsize, uint16_t ramstackstart,
                               uint16_t ramstackend)
{
    uint8_t i;
    uint16_t userthreadromstart;
    uint16_t userthreadromend;
    uint16_t userthreadramstart;
    uint16_t userthreadramend;

    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state != STATE_NULL)
        {
            if (thread_table[i].romstart == 0)
            {
                continue;
            }
            userthreadromstart = thread_table[i].romstart;
            userthreadromend =
                thread_table[i].romsize / 2 + userthreadromstart;
            userthreadramstart = (uint16_t) thread_table[i].ramstart;
            userthreadramend = (uint16_t) thread_table[i].ramend;
            if (!
                ((createflashromstart > userthreadromend + 2)
                 || (createflashromstart + createflashromsize / 2 <
                     userthreadromstart - 2)))
            {
                return 1;
            }
            if (!((ramstackstart > userthreadramend + 1) || (ramstackend <
                                                             userthreadramstart
                                                             - 1)))
            {
                return 1;
            }
        }
    }
    return 0;
}

//this executes and cleans up a thread
//Make sure that no variables are allocated
// also make sure no functions are called with attributes
void thread_func_dispatcher() __attribute__ ((naked));
void thread_func_dispatcher()
{
    //    (*current_thread->data.tp)();
    call_fcn_ptr(current_thread->data.tp);
    destroy_user_thread();
}

//This function uses the remaining credits to find out the appropriate next thread and returns it 
inline int thread_get_next()
{
    int i;
    int credits;
    int currentcandidate;
    _atomic_t currentatomic;

    currentcandidate = -1;
    credits = -1;
    currentatomic = _atomic_start();
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (thread_table[i].state == STATE_ACTIVE)
        {
            if (credits < thread_table[i].remaincredits)
            {
                credits = thread_table[i].remaincredits;
                currentcandidate = i;
            }
        }
    }
    if (credits < 0)
    {
        thread_task_active = 0;
    }
    _atomic_end(currentatomic);
    if (credits > 0)
    {
        currentatomic = _atomic_start();
        thread_table[currentcandidate].remaincredits--;
        _atomic_end(currentatomic);
        return currentcandidate;
    }
    else if (credits == 0)
    {
        currentatomic = _atomic_start();
        for (i = 0; i < LITE_MAX_THREADS; i++)
        {
            if (thread_table[i].state == STATE_ACTIVE)
            {
                thread_table[i].remaincredits = thread_table[i].priority;
            }
        }
        thread_table[currentcandidate].remaincredits--;
        _atomic_end(currentatomic);
        return currentcandidate;
    }
    else if (credits < 0)
    {
        return -1;
    }
    return 0;
}

/* thread_task
 */
int check_for_memory_corrupt(int i)
{
    uint16_t *kernelptr;
    uint16_t *ram_start, *ram_end;
    uint16_t sizeofBss;

    ram_start = thread_table[i].ramstart;
    ram_end = thread_table[i].ramend;
    sizeofBss = thread_table[i].sizeofBss;
    kernelptr = (uint16_t *) ((uint8_t *) ram_start + sizeofBss);
    if ((*kernelptr != 0xeeff) || (*(kernelptr + 1) != 0xeeff))
    {
        thread_table[i].state = STATE_MEM_ERROR;
        return -1;
    }
    return i;
}

//-------------------------------------------------------------------------
void thread_task()
{
    int i;

    i = thread_get_next();
    if (i < 0)
    {
        //here is the exit      
        return;
    }
    i = check_for_memory_corrupt(i);
    if (i < 0)
    {
        //here is the exit      
        return;
    }
    current_thread = &(thread_table[i]);
    lite_switch_to_user_thread();
    current_thread = 0;
    //postTask
    threads_handle_service();
    return;
}

/* thread_wakeup
 * This routine wakes up a thread that was put to sleep.
 */
void thread_wakeup(uint8_t id)
{
    if (id >= LITE_MAX_THREADS)
    {
        return;
    }
    if (thread_table[id].state == STATE_SLEEP)
    {
        thread_table[id].state = STATE_ACTIVE;
    }
    if (thread_task_active == 0)
    {
        postTask(thread_task, 2);
    }
}

//-------------------------------------------------------------------------
void postNewThreadTask()
{
    if (thread_task_active == 0)
    {
        postTask(thread_task, 2);
    }
}

//-------------------------------------------------------------------------
void ServiceTimerFired(uint8_t id)
{
    if (id == 9)
    {
        if (thread_task_active == 0)
        {
            postTask(thread_task, 2);
        }
    }
    else
    {
        thread_wakeup(id);
    }
}

//Return the address pointer of the current_thread, (its address, not its value)
//void getThreadAddress()  __attribute__((naked));
thread **getThreadAddress()
{
    thread **addr;

    addr = (thread **) & current_thread;
    return addr;
    //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
    // asm volatile("ret"::); 
}

//-------------------------------------------------------------------------
void **getKernelStackAddress()
{
    void **addr;

    addr = (void **)&old_stack_ptr;
    
    return addr;
}

//void getThreadIndexAddress() __attribute__((naked));
int getThreadIndexAddress()
{
    int i;
    uint16_t index;
    asm volatile ("push r20" "\n\t" "push r21" "\n\t"::);

    index = 0;
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (current_thread == &(thread_table[i]))
        {
            index = i;
            break;
        }
    }
    asm volatile ("pop r21" "\n\t" "pop r20" "\n\t"::);

    return index;
    //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( index ) );
    //asm volatile("ret"::); 
}

/* This unblocks an IO bound thread.
 * This routine can be called from any context.
 */
void Barrier_unblock(uint8_t type, uint8_t id)
{
    uint8_t i;
    _atomic_t currentatomic;

    currentatomic = _atomic_start();
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        //Look for a thread waiting on this IO
        if ((thread_table[i].state == STATE_IO)
            && (thread_table[i].data.iostate.type == type)
            && (thread_table[i].data.iostate.id == id))
        {
            //Mark that thread as active
            thread_table[i].state = STATE_ACTIVE;
            postNewThreadTask();
        }
    }
    _atomic_end(currentatomic);
}

//-------------------------------------------------------------------------
void break_point_function()
{
    int i;
    uint16_t index;

    index = 0;
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
        if (current_thread == &(thread_table[i]))
        {
            index = i;
            break;
        }
    }
    thread_table[index].state = 8;
    thread_yield();
    //asm volatile("ret"::); 
}
