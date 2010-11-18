 #include "threads.h"
 #include "thread_data.h"
 #include "avrhardware.h"
 #include "scheduling.h"
 #include "TimerRaw.h"


extern volatile mutex msend;  
extern volatile mutex *m_unlock; 
 
 //Our thread table
thread thread_table[TOSH_MAX_THREADS];


  //A pointer to the current thread, is null while in regular tinyos thread
volatile thread * current_thread;

  //This stores the tinyos stack pointer while we are executing a thread
volatile uint16_t * old_stack_ptr ;
volatile uint16_t *stackinterrupt_ptr; 

//This is simply a way to track whether our task is running
volatile uint8_t thread_task_active ;


uint8_t is_thread(){
    return(!!current_thread);
  }
  

void getThreadAddress()  __attribute__((naked));
void getThreadAddress()
  {
    volatile thread **addr = &current_thread; 
    asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (addr)
                );
    asm volatile("ret"::); 
}



void getThreadIndexAddress() __attribute__((naked));
void getThreadIndexAddress()
 {
    int i;
	uint16_t index; 
	 
	for (i=0;i<TOSH_MAX_THREADS;i++)
	  {
	    if (current_thread == &(thread_table[i]))
	     {index = i; 	 
		  break;
		 }
	  }

    asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (index)
                );
    asm volatile("ret"::); 
}




void getRadioMutexAddress()  __attribute__((naked)) ;
void getRadioMutexAddress()
{
   
    volatile mutex *msendaddr = &msend; 
    asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (msendaddr)
                );
    asm volatile("ret"::); 
}



static inline void *nmemset(void *to, int val, size_t n)
{
  char *cto = to;
  while (n--) *cto++ = val;
  return to;
}



void thread_init()
{
    _atomic_t currentatomic;
     currentatomic = _atomic_start();
    
    nmemset(thread_table, 0, sizeof(thread) * TOSH_MAX_THREADS);
	  current_thread = 0;
	  old_stack_ptr = 0;
	  stackinterrupt_ptr = 0;
	  thread_task_active = 0;   
    
    
    _atomic_end(currentatomic);  
//    TimerM_Timer_start(9, TIMER_REPEAT, 1000);
}


//This routine is called to perform system level utility change and schedules thread_task again 

void service_threads() /*__attribute__((noinline))*/{

     uint8_t i;
     uint8_t thread_presleep;
     _atomic_t currentatomic;    

     currentatomic = _atomic_start();

     thread_presleep = 0;
      for(i=0;i<TOSH_MAX_THREADS;i++){
        if(thread_table[i].state == STATE_PRESLEEP){
          break;
        }
      }
   
    thread_presleep = (i != TOSH_MAX_THREADS);
     
	  if(thread_presleep){
    
      postTask(service_threads,3);
		  thread_table[i].state = STATE_SLEEP;
      _atomic_end(currentatomic);
      TimerM_Timer_start(i, TIMER_ONE_SHOT, thread_table[i].data.sleepstate.sleeptime);
      thread_presleep = 0;
	    return;    
     }
    
   
      postTask(thread_task, 2);
      
    _atomic_end(currentatomic);
 }


  // These are new routines
  /* platform_switch_to_thread()
   * This routine swaps the stack and allows a thread to run.
   */
void __attribute__((noinline)) platform_switch_to_thread()/* __attribute__((naked))*/{
    PUSH_STATUS();
    PUSH_GPR();
    SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);
    POP_GPR();
    POP_STATUS();
    __asm__("sei"); 
    return;
  }


  /* yield
   * This routine pauses the current thread and swaps to the tinyos stack.
   * Attributes set to avoid any inlining, since we need this to actually be 
   * called(pushing the pc) and returned(popping the pc).
   */
void __attribute__((noinline)) yield() /* __attribute__((noinline))*/{
    PUSH_STATUS();
    PUSH_GPR();
    //Now swap the stacks back
    SWAP_STACK_PTR(current_thread->sp, old_stack_ptr);
    POP_GPR();
    POP_STATUS();
  }


  /* remove_thread
   * This routine is only called when a users thread returns.
   * It removes it from the thread table.
   */  
  void remove_thread(){
     
     _atomic_t currentatomic;
     currentatomic = _atomic_start();
     
      current_thread->state = STATE_NULL;
      yield();
      
    _atomic_end(currentatomic);
  }



  //this executes and cleans up a thread
  //Make sure that no variables are allocated
  // also make sure no functions are called with attributes

void  thread_wrapper() __attribute__((naked));
void  thread_wrapper()
{
//    (*current_thread->data.tp)();
    call_fcn_ptr(current_thread->data.tp);
    remove_thread();
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
	  for(i=0;i<TOSH_MAX_THREADS;i++){
        if(thread_table[i].state == STATE_ACTIVE){
          if (credits < thread_table[i].remaincredits)
          	{
          	  credits = thread_table[i].remaincredits; 
          	  currentcandidate = i;	
          	  
          	}
        }       
      }
   if (credits <0)
   	  thread_task_active = 0; 
   
   _atomic_end(currentatomic);
   	  
   	  
   if (credits >0)
    {
     currentatomic = _atomic_start();
     thread_table[currentcandidate].remaincredits--;
     _atomic_end(currentatomic);
     return currentcandidate;  	
    	
    }
   else if (credits ==0)
   	{
   		currentatomic = _atomic_start();
   	 	for(i=0;i<TOSH_MAX_THREADS;i++){
        if(thread_table[i].state == STATE_ACTIVE){
        	thread_table[i].remaincredits = thread_table[i].priority;    		          	
   	    }
		}
     thread_table[currentcandidate].remaincredits--;
   	 _atomic_end(currentatomic);
    
   	 return currentcandidate;  

   	 
	  }

   else if (credits <0)
   	
   	{return -1;}
  
  return 0;
}
/* thread_task
 * This is posted and executed by tinyos.  Included is our threads.
*/

void thread_task(){
 
       int i;
       
       i = thread_get_next();
       if (i<0)
       	 {
       	  //here is the exit 	
       	  
       	  return;       	
      	 }
       
      
       	 
       current_thread = &(thread_table[i]);
       
       platform_switch_to_thread();
       
       current_thread = 0;    
      
      
       //postTask
       service_threads();
       return;
  }

  

void ServiceTimerFired(uint8_t id)
{  
   
     if (id==9)
       {if (thread_task_active==0)
       postTask(thread_task, 2);
       } 
      else      	     
      thread_wakeup(id);
}
  
 
 
  int create_thread(void (*fcn)(), uint16_t * stack_ptr, uint8_t priority){

    int i;
    _atomic_t currentatomic;
    
    //we do not allow threads to create new threads
    if(is_thread())
      return(0);
     
    //First loop all the way through the table and find an empty slot 
    //computation time for space here 
    
    for(i=0;i<TOSH_MAX_THREADS;i++){
      if(thread_table[i].state == STATE_NULL)
        break;
    }
  
    //Cannot create thread, no resources... 
    
    if(i == TOSH_MAX_THREADS)
      return(0);
    
    //Add the thread
    
      currentatomic = _atomic_start();

      //Populate the thread table
      current_thread = &thread_table[i];
      current_thread->sp = stack_ptr;
      current_thread->state = STATE_ACTIVE;
      current_thread->data.tp = fcn;
      current_thread->priority = priority; 
      current_thread->remaincredits = priority; 
      
      //Prepare the fcn pointer on the new stack, so it can be executed
      PREPARE_STACK();
   
      current_thread = 0;
      
      if(!thread_task_active){
        postTask(thread_task, 2);
        thread_task_active = 1;
      }
      
    _atomic_end(currentatomic);  
    
    return(1);
  }

 
 
 /* thread_sleep
   * This routine puts the current thread into a sleeping state.
   * It will not ever wake up until another task or thread wakes it up
   */
 void sleepThread(uint16_t milli){
   
    if(!is_thread())
      return;
   

   //this is insid the thread!

    current_thread->state = STATE_PRESLEEP;
	current_thread->data.sleepstate.sleeptime = milli;
    yield();
  }

 
 
 
  /* thread_wakeup
   * This routine wakes up a thread that was put to sleep.
   */
  void thread_wakeup(uint8_t id){
    if(id >= TOSH_MAX_THREADS)
      return;	    
   
    
   if(thread_table[id].state == STATE_SLEEP)
      thread_table[id].state = STATE_ACTIVE;
    
    if (thread_task_active==0)
       postTask(thread_task, 2);
  }

 
 
void mutexUnlockFunction()  __attribute__((naked));
void mutexUnlockFunction()
{
   
    
    asm volatile("mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r"(m_unlock)
				 :
                );
    Mutex_unlock((mutex *)m_unlock); 
 
}

 
 
  /* mutex_unlock
   * This routine unlocks a mutex, and will yield the current thread
   * if there are any other threads waiting on this mutex
   * This routine is safe to call inside or outside of a thread
   */
 void Mutex_unlock(mutex * m){
    _atomic_t currentatomic;
    currentatomic = _atomic_start();
    
      if(m->lock){
        if(m->waiting){
         int i;
          //Go wake up the next thread in line
          for(i=0;i<=TOSH_MAX_THREADS;i++){

         //here I removed the threadidx
		   int idx = i; 
           //Look for the next thread waiting on this mutex
            if((thread_table[idx].state == STATE_BLOCKED) && 
                (thread_table[idx].data.m == m)){
              //Mark that thread as active
              thread_table[idx].state = STATE_ACTIVE;
              if (thread_task_active == 0)
			    postTask(thread_task, 2);
		
		        //Let next task run (we are active, but will yield anyways
              
			  yield();
              break;
            }
          }
        }else
		{
          m->lock = 0;
        }//m-waiting
      }//m->lock
    _atomic_end(currentatomic);
 }

 
 

 
 
 
  /* io_unblock
   * This unblocks an IO bound thread.
   * This routine can be called from any context.
   */
 void Barrier_unblock(uint8_t type,uint8_t id){
    uint8_t i;
      _atomic_t currentatomic;

    currentatomic = _atomic_start();
    
    
      for(i=0;i<TOSH_MAX_THREADS;i++){
        //Look for a thread waiting on this IO
        if((thread_table[i].state == STATE_IO) && 
            (thread_table[i].data.iostate.type == type) &&
            (thread_table[i].data.iostate.id == id)){
          //Mark that thread as active
          thread_table[i].state = STATE_ACTIVE;
          if (thread_task_active == 0)
			    postTask(thread_task, 2);
        }
      }
    
    _atomic_end(currentatomic);
  }    




