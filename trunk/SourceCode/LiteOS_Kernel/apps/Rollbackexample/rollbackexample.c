#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"


uint32_t time1, time2, time3, time4;

 
uint8_t blinkbuffer[100]; 

int operation_try_me()
{
  int test; 
  test = lib_random();
  test = test%100;
   
  if (test < 90)
   return 0;
  else 
   return 1;
}



int blink()
{

 

    
	uint32_t count; 

	uint32_t state1, state2, state3; 

	volatile uint16_t snapshotindex;

   
     uint8_t success; 	
	 
	 time1=time2=time3 = 0; 
	 count = 0; 


      time1 = get_current_timestamp();
     	lib_printf_uinteger32(time3);
     	lib_printf_ln();
			time2 = get_current_timestamp();
			lib_print_usart_string("experiment now1\n");
			lib_printf_uinteger32(time1);
     	lib_printf_ln();
       lib_printf_uinteger32(time2);
     	lib_printf_ln();
     	
			
			
		
  
 //__asm__ __volatile__("sei" ::); 
 
   //for (index = 0;index <100;index++)
   while(1)
   	{
	   lib_green_toggle(); 
	   count++;
      
     //if (count%100==0)
     	lib_print_usart_string("rollback point 0\n");
		  state1 = state2 = state3 = 3; 
		  time1 = get_current_timestamp();
    // 	snapshotindex = lib_thread_create_snapshot();
			time2 = get_current_timestamp();
			time3 = get_current_timestamp();
			lib_print_usart_string("rollback point 0\n");
			lib_printf_uinteger32(time1);
 			lib_printf_ln();
 			lib_printf_uinteger32(time2);
 			lib_printf_ln();
 			time4 = get_current_timestamp();
 			
			lib_printf_uinteger32(time3);
 			lib_printf_ln();
 			lib_printf_uinteger32(time4);
 			lib_printf_ln();
     //   	success = operation_try_me();
      lib_print_usart_string("rollback point 1\n");
			  
//			   lib_printf_uinteger32((int32_t)state1);
//			   lib_printf_ln();
//   			   lib_printf_uinteger32((int32_t)state2);
//			   lib_printf_ln();
//               lib_printf_uinteger32((int32_t)state3);
//               lib_printf_ln();

			  state1 = state2 = state3 = 7; 

			  //lib_print_usart_string("Now before the rollback\n"); 
              //lib_printf_uinteger32((int32_t)state1);
			  
			   
     	   	  
			 	   lib_print_usart_string("rollback\n");
			 	 
			 	   lib_printf_uinteger32(time1);
 					 lib_printf_ln();
					   time1 = get_current_timestamp();
			//	   lib_thread_rollback_snapshot(snapshotindex);
				   
				   lib_print_usart_string("rollback point 1\n");
				   lib_printf_uinteger32((int32_t)state1);
				   lib_printf_ln();
   			       lib_printf_uinteger32((int32_t)state2);
				   lib_printf_ln();
                   lib_printf_uinteger32((int32_t)state3);
                   lib_printf_ln();
			   	lib_sleep_thread(2000);
				 
				 }
     	   	 
				 
    
	   
	  /* {
     volatile uint32_t gtime1, gtime2, gtime3; 
     gtime1 =getCPUcount();
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     
     gtime2 = getCPUcount();
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     __asm__ __volatile__("nop");
     
    gtime3 = getCPUcount();
   
     printfstring("the time as follows\n");
     printfuinteger32(gtime1);
     printfuinteger32(gtime2);
     printfuinteger32(gtime3);
     }
  */
    //count = ();
    //libradioSend_string("Hello, world!\n"); 
   // libprintfuinteger32(count);
   // libprintfstring("blink\n");
    
    
   
	return 0; 
}

