//User tasks 

#include "Types.h"
#include "thread_data.h"
#include "threads.h"
#include "Leds.h"
#include "amradio.h"
#include "Socket.h"
#include "usertask.h"
#include "TimerRaw.h"

uint16_t stackuser[5];

uint16_t stackuser2[5];

uint16_t stackuser3[5];

uint16_t stackuser4[5];

uint32_t a, b,c ;

uint32_t d;

uint32_t msg[6];

Radio_Msg rawmsg; 


void mystrcpy(char *target, const char *source, int n)
{
        	int i;

        for (i=0;i<n;i++)
		    *target++ = *source++;
        

        return;
}

/*


void Task2()
{

while (1)
 {
 
  //Leds_greenToggle();
  
  sleepThread(1000);
  while (1)
   {
   sleepThread(20);
   a++;
   msg[0] = a; 
   msg[1] = 2; 
   mystrcpy((char *)rawmsg.data, (char *)msg, 12);
   SocketSend(6, 0xffff, 24, &rawmsg);
   //AMStandard_SendMsg_send(7, 0xffff, 12, &rawmsg);  
   }
 
 }

}



void Task3()
{

while (1)
 {
 
  //Leds_greenToggle();
  
  sleepThread(1000);
  while (1)
   {
   sleepThread(20);
   b++;
   msg[0] = b; 
   msg[1] = 3; 
   mystrcpy((char *)rawmsg.data, (char *)msg, 12);
   SocketSend(6, 0xffff, 24, &rawmsg);
   //AMStandard_SendMsg_send(7, 0xffff, 12, &rawmsg);  
   }
 
 }

}
*/
void Task6()
{

while (1)
 {
 
  //Leds_greenToggle();
  
  sleepThread(1000);
  while (1)
   {
   sleepThread(20);
   d++;
   msg[0] = d; 
   msg[1] = 6; 
   mystrcpy((char *)rawmsg.data, (char *)msg, 12);
   //SocketSend(6, 0xffff, 24, &rawmsg);
   //AMStandard_SendMsg_send(7, 0xffff, 12, &rawmsg);  
   }
 
 }

}
void Task5()
{

while (1)
 {
  
  c++;
  //Leds_yellowToggle();
  //sleepThread(1000);

 }

}


void dataTask()
{

  AMStandard_SendMsg_send(7, 0xffff, 12, &rawmsg);  
}

void mytimer_fired(){

 
 postTask(dataTask, 10);
//add
//PhotoTempM_ExternalPhotoADC_getData();
//PhotoTempM_ExternalPhotoADC_dataReady(6);

}



void Task1()
{
 uint16_t i;
  for (i=0;i<3;i++)
   msg[i] = 0; 

  a = b = c = d = 0; 

  //create_thread(Task2, STACK_TOP(stackuser), 10);
  //create_thread(Task3, STACK_TOP(stackuser2),10);
  //create_thread(Task5, STACK_TOP(stackuser3),5);
  //create_thread(Task6, STACK_TOP(stackuser4),10);
  mystrcpy((char *)rawmsg.data, (char *)msg, 24);
  //SocketSend(7, 0xffff, 12, &rawmsg); 
 
 // for (i=0;i<10000;i++)
 // for (j=0;j<100;j++)
 // {
 //   a = 0; 
 // }
 // AMStandard_SendMsg_send(7, 0xffff, 12, &rawmsg);    

 //TimerM_Timer_start(3, TIMER_ONE_SHOT, 300*MILLISECOND);
 
}
