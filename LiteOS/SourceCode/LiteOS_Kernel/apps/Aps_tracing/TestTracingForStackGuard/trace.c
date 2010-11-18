#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "serial.h"
#include "file.h"
#include "eeprom.h"
#include "adc.h"
#include "eventtrace.h"
#include "trace.h"

 	
tracingblock *blockaddr; 
volatile uint8_t stacklevel; 
volatile uint8_t update = 0; 
uint8_t *eventid; 

uint16_t currentindex; 
uint8_t *threadid; 
volatile uint16_t regsource; 

thread **thiscurrentthread;
thread *thisthread; 
 
uint8_t writetask; 

uint16_t value; 

uint16_t popcanary;
uint8_t temploop; 
          

uint16_t originalsp; 

//PATCHDEFSTART$$$$$
volatile uint8_t tracepatch0[16];
volatile uint8_t tracepatch1[16];
//PATCHDEFEND$$$$$


volatile uint16_t **oldstackptraddr;
volatile uint16_t *currentthreadptr;

 uint16_t pagenum;
 uint8_t pageoffset;

#define SWAP_STACK_PTR(OLD, NEW) \
  __asm__("in %A0, __SP_L__\n\t in %B0, __SP_H__":"=r"(OLD):);\
  __asm__("out __SP_H__,%B0\n\t out __SP_L__,%A0"::"r"(NEW))

 
//APPTRACESTART$$$$$
#define	APPTRACEPOINT0	0xb464
#define 	APPTRACEPOINT1	0xb465
#define	APPTRACEPOINT2	0xb466
#define 	APPTRACEPOINT3	0xb467
#define	APPTRACEPOINT4	0xb468
#define 	APPTRACEPOINT5	0xb469
#define	APPTRACEPOINT6	0xb470
#define 	APPTRACEPOINT7	0xb471
#define	APPTRACEPOINT8	0xb472
#define 	APPTRACEPOINT9	0xb473
#define	APPTRACEPOINT10	0xb474
#define 	APPTRACEPOINT11	0xb475
#define	APPTRACEPOINT12	0xb476
#define 	APPTRACEPOINT13	0xb477
#define	APPTRACEPOINT14	0xb478
#define 	APPTRACEPOINT15	0xb479
#define	APPTRACEPOINT16	0xb480
#define 	APPTRACEPOINT17	0xb481
#define	APPTRACEPOINT18	0xb482
#define 	APPTRACEPOINT19	0xb483
#define	APPTRACEPOINT20	0xb484
#define 	APPTRACEPOINT21	0xb485
#define	APPTRACEPOINT22	0xb486
#define 	APPTRACEPOINT23	0xb487
#define	APPTRACEPOINT24	0xb488
#define 	APPTRACEPOINT25	0xb489
#define	APPTRACEPOINT26	0xb490
#define 	APPTRACEPOINT27	0xb491
#define	APPTRACEPOINT28	0xb492
#define 	APPTRACEPOINT29	0xb493
#define	APPTRACEPOINT30	0xb494
#define 	APPTRACEPOINT31	0xb495
#define	APPTRACEPOINT32	0xb496
#define 	APPTRACEPOINT33	0xb497
#define	APPTRACEPOINT34	0xb498
#define 	APPTRACEPOINT35	0xb499
#define	APPTRACEPOINT36	0xb500
#define 	APPTRACEPOINT37	0xb501
#define	APPTRACEPOINT38	0xb502
#define 	APPTRACEPOINT39	0xb503
#define	APPTRACEPOINT40	0xb504
#define 	APPTRACEPOINT41	0xb505
#define	APPTRACEPOINT42	0xb506
#define 	APPTRACEPOINT43	0xb507
#define	APPTRACEPOINT44	0xb508
#define 	APPTRACEPOINT45	0xb509
#define	APPTRACEPOINT46	0xb510
#define 	APPTRACEPOINT47	0xb511
#define	APPTRACEPOINT48	0xb512
#define 	APPTRACEPOINT49	0xb513
#define	APPTRACEPOINT50	0xb514
#define 	APPTRACEPOINT51	0xb515
#define	APPTRACEPOINT52	0xb516
#define	APPTRACEPOINT53	0xb517
#define 	APPTRACEPOINT54	0xb518
#define	APPTRACEPOINT55	0xb519
#define	APPTRACEPOINT56	0xb520
#define 	APPTRACEPOINT57	0xb521
#define	APPTRACEPOINT58	0xb522
#define	APPTRACEPOINT59	0xb523
#define 	APPTRACEPOINT60	0xb524
#define	APPTRACEPOINT61	0xb525
#define	APPTRACEPOINT62	0xb526
#define 	APPTRACEPOINT63	0xb527
#define	APPTRACEPOINT64	0xb528
#define	APPTRACEPOINT65	0xb529
#define 	APPTRACEPOINT66	0xb530
#define	APPTRACEPOINT67	0xb531
#define	APPTRACEPOINT68	0xb532
#define 	APPTRACEPOINT69	0xb533
#define	APPTRACEPOINT70	0xb534
#define	APPTRACEPOINT71	0xb535
#define 	APPTRACEPOINT72	0xb536
#define	APPTRACEPOINT73	0xb537
#define	APPTRACEPOINT74	0xb538
#define 	APPTRACEPOINT75	0xb539
#define	APPTRACEPOINT76	0xb540
#define	APPTRACEPOINT77	0xb541
#define 	APPTRACEPOINT78	0xb542
#define	APPTRACEPOINT79	0xb543
#define	APPTRACEPOINT80	0xb544
#define 	APPTRACEPOINT81	0xb545
#define	APPTRACEPOINT82	0xb546
#define	APPTRACEPOINT83	0xb547
#define 	APPTRACEPOINT84	0xb548
#define	APPTRACEPOINT85	0xb549
#define	APPTRACEPOINT86	0xb550
#define 	APPTRACEPOINT87	0xb551
#define	APPTRACEPOINT88	0xb552
#define	APPTRACEPOINT89	0xb553
#define 	APPTRACEPOINT90	0xb554
#define	APPTRACEPOINT91	0xb555
#define	APPTRACEPOINT92	0xb556
#define 	APPTRACEPOINT93	0xb557
#define	APPTRACEPOINT94	0xb558
#define	APPTRACEPOINT95	0xb559
#define 	APPTRACEPOINT96	0xb560
#define	APPTRACEPOINT97	0xb561
#define	APPTRACEPOINT98	0xb562
#define	APPTRACEPOINT99	0xb563


#define 	APPTRACEPOINTCOMPARE0	0x7000
#define 	APPTRACEPOINTCOMPARE1	0x7001
#define 	APPTRACEPOINTCOMPARE2	0x7002
#define 	APPTRACEPOINTCOMPARE3	0x7003
#define 	APPTRACEPOINTCOMPARE4	0x7004
#define 	APPTRACEPOINTCOMPARE5	0x7005
#define 	APPTRACEPOINTCOMPARE6	0x7006
#define 	APPTRACEPOINTCOMPARE7	0x7007
#define 	APPTRACEPOINTCOMPARE8	0x7008
#define 	APPTRACEPOINTCOMPARE9	0x7009
#define 	APPTRACEPOINTCOMPARE10	0x7010
#define 	APPTRACEPOINTCOMPARE11	0x7011
#define 	APPTRACEPOINTCOMPARE12	0x7012
#define 	APPTRACEPOINTCOMPARE13	0x7013
#define 	APPTRACEPOINTCOMPARE14	0x7014
#define 	APPTRACEPOINTCOMPARE15	0x7015
#define 	APPTRACEPOINTCOMPARE16	0x7016
#define 	APPTRACEPOINTCOMPARE17	0x7017
#define 	APPTRACEPOINTCOMPARE18	0x7018
#define 	APPTRACEPOINTCOMPARE19	0x7019
#define 	APPTRACEPOINTCOMPARE20	0x7020
#define 	APPTRACEPOINTCOMPARE21	0x7021
#define 	APPTRACEPOINTCOMPARE22	0x7022
#define 	APPTRACEPOINTCOMPARE23	0x7023
#define 	APPTRACEPOINTCOMPARE24	0x7024
#define 	APPTRACEPOINTCOMPARE25	0x7025
#define 	APPTRACEPOINTCOMPARE26	0x7026
#define 	APPTRACEPOINTCOMPARE27	0x7027
#define 	APPTRACEPOINTCOMPARE28	0x7028
#define 	APPTRACEPOINTCOMPARE29	0x7029
#define 	APPTRACEPOINTCOMPARE30	0x7030
#define 	APPTRACEPOINTCOMPARE31	0x7031
#define 	APPTRACEPOINTCOMPARE32	0x7032
#define 	APPTRACEPOINTCOMPARE33	0x7033
#define 	APPTRACEPOINTCOMPARE34	0x7034
#define 	APPTRACEPOINTCOMPARE35	0x7035
#define 	APPTRACEPOINTCOMPARE36	0x7036
#define 	APPTRACEPOINTCOMPARE37	0x7037
#define 	APPTRACEPOINTCOMPARE38	0x7038
#define 	APPTRACEPOINTCOMPARE39	0x7039
#define 	APPTRACEPOINTCOMPARE40	0x7040
#define 	APPTRACEPOINTCOMPARE41	0x7041
#define 	APPTRACEPOINTCOMPARE42	0x7042
#define 	APPTRACEPOINTCOMPARE43	0x7043
#define 	APPTRACEPOINTCOMPARE44	0x7044
#define 	APPTRACEPOINTCOMPARE45	0x7045
#define 	APPTRACEPOINTCOMPARE46	0x7046
#define 	APPTRACEPOINTCOMPARE47	0x7047
#define 	APPTRACEPOINTCOMPARE48	0x7048
#define 	APPTRACEPOINTCOMPARE49	0x7049
#define 	APPTRACEPOINTCOMPARE50	0x7050
#define 	APPTRACEPOINTCOMPARE51	0x7051
#define 	APPTRACEPOINTCOMPARE52	0x7052
#define 	APPTRACEPOINTCOMPARE53	0x7053
#define 	APPTRACEPOINTCOMPARE54	0x7054
#define 	APPTRACEPOINTCOMPARE55	0x7055
#define 	APPTRACEPOINTCOMPARE56	0x7056
#define 	APPTRACEPOINTCOMPARE57	0x7057
#define 	APPTRACEPOINTCOMPARE58	0x7058
#define 	APPTRACEPOINTCOMPARE59	0x7059
#define 	APPTRACEPOINTCOMPARE60	0x7060
#define 	APPTRACEPOINTCOMPARE61	0x7061
#define 	APPTRACEPOINTCOMPARE62	0x7062
#define 	APPTRACEPOINTCOMPARE63	0x7063
#define 	APPTRACEPOINTCOMPARE64	0x7064
#define 	APPTRACEPOINTCOMPARE65	0x7065
#define 	APPTRACEPOINTCOMPARE66	0x7066
#define 	APPTRACEPOINTCOMPARE67	0x7067
#define 	APPTRACEPOINTCOMPARE68	0x7068
#define 	APPTRACEPOINTCOMPARE69	0x7069
#define 	APPTRACEPOINTCOMPARE70	0x7070
#define 	APPTRACEPOINTCOMPARE71	0x7071
#define 	APPTRACEPOINTCOMPARE72	0x7072
#define 	APPTRACEPOINTCOMPARE73	0x7073
#define 	APPTRACEPOINTCOMPARE74	0x7074
#define 	APPTRACEPOINTCOMPARE75	0x7075
#define 	APPTRACEPOINTCOMPARE76	0x7076
#define 	APPTRACEPOINTCOMPARE77	0x7077
#define 	APPTRACEPOINTCOMPARE78	0x7078
#define 	APPTRACEPOINTCOMPARE79	0x7079
#define 	APPTRACEPOINTCOMPARE80	0x7080
#define 	APPTRACEPOINTCOMPARE81	0x7081
#define 	APPTRACEPOINTCOMPARE82	0x7082
#define 	APPTRACEPOINTCOMPARE83	0x7083
#define 	APPTRACEPOINTCOMPARE84	0x7084
#define 	APPTRACEPOINTCOMPARE85	0x7085
#define 	APPTRACEPOINTCOMPARE86	0x7086
#define 	APPTRACEPOINTCOMPARE87	0x7087
#define 	APPTRACEPOINTCOMPARE88	0x7088
#define 	APPTRACEPOINTCOMPARE89	0x7089
#define 	APPTRACEPOINTCOMPARE90	0x7090
#define 	APPTRACEPOINTCOMPARE91	0x7091
#define 	APPTRACEPOINTCOMPARE92	0x7092
#define 	APPTRACEPOINTCOMPARE93	0x7093
#define 	APPTRACEPOINTCOMPARE94	0x7094
#define 	APPTRACEPOINTCOMPARE95	0x7095
#define 	APPTRACEPOINTCOMPARE96	0x7096
#define 	APPTRACEPOINTCOMPARE97	0x7097
#define 	APPTRACEPOINTCOMPARE98	0x7098
#define 	APPTRACEPOINTCOMPARE99	0x7099





void tracingcontroluser()  __attribute__(( naked ));
void tracingcontroluser() {
	
	
	  uint16_t SPvalue; 
	  uint16_t canaryaddr; 
	  uint16_t *sp;



    asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (SPvalue)
    :
    );
   
       
    SPvalue = SPvalue+5+4; 
    sp = (uint16_t*)SPvalue; 
    regsource = (uint16_t)*(uint16_t*)sp;
    //reghigh = *(sp+1); 
    SPvalue = SPvalue-7; 
    
     asm volatile (
    "out 0x3d, %A0" "\n\t"
    "out 0x3e, %B0" "\n\t"
    :: "r" (SPvalue)
    
    );
    
    //APPJUMPSTART$$$$$
if ((regsource == APPTRACEPOINTCOMPARE0)||(regsource == APPTRACEPOINTCOMPARE1)||(regsource == APPTRACEPOINTCOMPARE2)||(regsource == APPTRACEPOINTCOMPARE3)||(regsource == APPTRACEPOINTCOMPARE4)||(regsource == APPTRACEPOINTCOMPARE5)||(regsource == APPTRACEPOINTCOMPARE6)||(regsource == APPTRACEPOINTCOMPARE7)||(regsource == APPTRACEPOINTCOMPARE8)||(regsource == APPTRACEPOINTCOMPARE9)||(regsource == APPTRACEPOINTCOMPARE10)||(regsource == APPTRACEPOINTCOMPARE11)||(regsource == APPTRACEPOINTCOMPARE12)||(regsource == APPTRACEPOINTCOMPARE13)||(regsource == APPTRACEPOINTCOMPARE14)||(regsource == APPTRACEPOINTCOMPARE15)||(regsource == APPTRACEPOINTCOMPARE16)||(regsource == APPTRACEPOINTCOMPARE17)||(regsource == APPTRACEPOINTCOMPARE18)||(regsource == APPTRACEPOINTCOMPARE19)||(regsource == APPTRACEPOINTCOMPARE20)||(regsource == APPTRACEPOINTCOMPARE21)||(regsource == APPTRACEPOINTCOMPARE22)||(regsource == APPTRACEPOINTCOMPARE23)||(regsource == APPTRACEPOINTCOMPARE24)||(regsource == APPTRACEPOINTCOMPARE25)||(regsource == APPTRACEPOINTCOMPARE26)||(regsource == APPTRACEPOINTCOMPARE27)||(regsource == APPTRACEPOINTCOMPARE28)||(regsource == APPTRACEPOINTCOMPARE29)||(regsource == APPTRACEPOINTCOMPARE30)||(regsource == APPTRACEPOINTCOMPARE31)||(regsource == APPTRACEPOINTCOMPARE32)||(regsource == APPTRACEPOINTCOMPARE33)||(regsource == APPTRACEPOINTCOMPARE34)||(regsource == APPTRACEPOINTCOMPARE35)||(regsource == APPTRACEPOINTCOMPARE36)||(regsource == APPTRACEPOINTCOMPARE37)||(regsource == APPTRACEPOINTCOMPARE38)||(regsource == APPTRACEPOINTCOMPARE39)||(regsource == APPTRACEPOINTCOMPARE40)||(regsource == APPTRACEPOINTCOMPARE41)||(regsource == APPTRACEPOINTCOMPARE42)||(regsource == APPTRACEPOINTCOMPARE43)||(regsource == APPTRACEPOINTCOMPARE44)||(regsource == APPTRACEPOINTCOMPARE45)||(regsource == APPTRACEPOINTCOMPARE46)||(regsource == APPTRACEPOINTCOMPARE47)||(regsource == APPTRACEPOINTCOMPARE48)||(regsource == APPTRACEPOINTCOMPARE49))
{

__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r25" ::);
__asm__ __volatile__("pop r24" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
	
__asm__ __volatile__("push r30" ::);
__asm__ __volatile__("push r31" ::);	
__asm__ __volatile__("push r30" ::);
__asm__ __volatile__("push r31" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);

 asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (canaryaddr)
    :
    );
   
   
     
  canaryaddr = canaryaddr + 7; 
  (*(uint16_t*)canaryaddr) = 0x1234;
  
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r24" ::);
__asm__ __volatile__("pop r25" ::); 
__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
    
__asm__ __volatile__("push r28"::);

__asm__ __volatile__("push r29"::);

__asm__ __volatile__("in r28, 0x3d"::);

__asm__ __volatile__("in r29, 0x3e"::);

__asm__ __volatile__("sbiw r28, 0x0a"::);

__asm__ __volatile__("in r0, 0x3f"::);

__asm__ __volatile__("cli"::);

}

else if ((regsource == APPTRACEPOINTCOMPARE50)||(regsource == APPTRACEPOINTCOMPARE51)||(regsource == APPTRACEPOINTCOMPARE52)||(regsource == APPTRACEPOINTCOMPARE53)||(regsource == APPTRACEPOINTCOMPARE54)||(regsource == APPTRACEPOINTCOMPARE55)||(regsource == APPTRACEPOINTCOMPARE56)||(regsource == APPTRACEPOINTCOMPARE57)||(regsource == APPTRACEPOINTCOMPARE58)||(regsource == APPTRACEPOINTCOMPARE59)||(regsource == APPTRACEPOINTCOMPARE60)||(regsource == APPTRACEPOINTCOMPARE61)||(regsource == APPTRACEPOINTCOMPARE62)||(regsource == APPTRACEPOINTCOMPARE63)||(regsource == APPTRACEPOINTCOMPARE64)||(regsource == APPTRACEPOINTCOMPARE65)||(regsource == APPTRACEPOINTCOMPARE66)||(regsource == APPTRACEPOINTCOMPARE67)||(regsource == APPTRACEPOINTCOMPARE68)||(regsource == APPTRACEPOINTCOMPARE69)||(regsource == APPTRACEPOINTCOMPARE70)||(regsource == APPTRACEPOINTCOMPARE71)||(regsource == APPTRACEPOINTCOMPARE72)||(regsource == APPTRACEPOINTCOMPARE73)||(regsource == APPTRACEPOINTCOMPARE74)||(regsource == APPTRACEPOINTCOMPARE75)||(regsource == APPTRACEPOINTCOMPARE76)||(regsource == APPTRACEPOINTCOMPARE77)||(regsource == APPTRACEPOINTCOMPARE78)||(regsource == APPTRACEPOINTCOMPARE79)||(regsource == APPTRACEPOINTCOMPARE80)||(regsource == APPTRACEPOINTCOMPARE81)||(regsource == APPTRACEPOINTCOMPARE82)||(regsource == APPTRACEPOINTCOMPARE83)||(regsource == APPTRACEPOINTCOMPARE84)||(regsource == APPTRACEPOINTCOMPARE85)||(regsource == APPTRACEPOINTCOMPARE86)||(regsource == APPTRACEPOINTCOMPARE87)||(regsource == APPTRACEPOINTCOMPARE88)||(regsource == APPTRACEPOINTCOMPARE89)||(regsource == APPTRACEPOINTCOMPARE90)||(regsource == APPTRACEPOINTCOMPARE91)||(regsource == APPTRACEPOINTCOMPARE92)||(regsource == APPTRACEPOINTCOMPARE93)||(regsource == APPTRACEPOINTCOMPARE94)||(regsource == APPTRACEPOINTCOMPARE95)||(regsource == APPTRACEPOINTCOMPARE96)||(regsource == APPTRACEPOINTCOMPARE97)||(regsource == APPTRACEPOINTCOMPARE98)||(regsource == APPTRACEPOINTCOMPARE99))

{

__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r25" ::);
__asm__ __volatile__("pop r24" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);

__asm__ __volatile__("in r0, 0x3f"::);

__asm__ __volatile__("cli"::);

__asm__ __volatile__("out 0x3e, r29"::);

__asm__ __volatile__("out 0x3f, r0"::);

__asm__ __volatile__("out 0x3d, r28"::);

__asm__ __volatile__("pop r29"::);

__asm__ __volatile__("pop r28"::);


__asm__ __volatile__("push r31" ::);
__asm__ __volatile__("push r30" ::);	
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);

 asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (canaryaddr)
    :
    );
   
   
     
  canaryaddr = canaryaddr + 9; 
  popcanary = *(uint16_t*)canaryaddr;
  if (popcanary != 0x1234)
  	 {
        	
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::); 	
  	 }
  
  canaryaddr = canaryaddr+1;

  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
  canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
  canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;  
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    	 
  __asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r24" ::);
__asm__ __volatile__("pop r25" ::);
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r30" ::);
__asm__ __volatile__("pop r31" ::);	

}

   {
    PUSH_GPR();           
    PUSH_REG_STATUS();    
                          
    {                      
    _atomic_t _atomic = _atomic_start();
   
 
    _atomic_end(_atomic); 
    
    }
            
    
    POP_REG_STATUS();
    POP_GPR();
    	
   	
   	
   	
   } 
    
    
    
	  __asm__ __volatile__("push r30" ::); 	
	  __asm__ __volatile__("push r31" ::); 
	  	
    
   asm volatile(" mov r31, %A0" "\n\t"
	              " mov r30, %B0" "\n\t"
				 :
				 :"r" (regsource)
                );
    
  
   __asm__ __volatile__("ijmp" ::); 
    
   
}


 
//////////////////////////////////////////////////////////////////////////////////////
/// Clean up task for the thread
//////////////////////////////////////////////////////////////////////////////////////


//The trace arrays should be application specific here
void cleanUpTraceFunction() {

//CLEANTRACESTART$$$$$                     


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

removeUserTracePoint( pagenum,  pageoffset , tracepatch0);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT1*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

removeUserTracePoint( pagenum,  pageoffset , tracepatch1);

}


//CLEANTRACEEND$$$$$
	
}




int main()
{ 


  
  __asm__ __volatile__("sei" ::); 
 
 
 writetask = 0; 
 stacklevel = 0; 
 currentindex = 0; 
 //fileptr = mfopen("/logtrace", "w");
 enableTracing();  

 blockaddr = (tracingblock *)getEventBlockAddress();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 //blockaddr->externaladdtracefp = tracingcontrol; 
 blockaddr->externalapptracefp = tracingcontroluser; 
 
 thiscurrentthread = getCurrentThread(); 
 thisthread = *thiscurrentthread; 
 
 //setTimerFunction(500, 0, traceMemory);
 
 
 oldstackptraddr = (uint16_t**)getKernelStackPtr(); 
 
 //setThreadTerminateHandler(cleanUpTraceFunction); 
 
 // setTimerFunction(50, 0, testUpdate);
 
//CLEANDEFSTART$$$$$
tracepatch0[0] = 0xcf;

tracepatch0[1] = 0x93;

tracepatch0[2] = 0xdf;

tracepatch0[3] = 0x93;

tracepatch0[4] = 0xcd;

tracepatch0[5] = 0xb7;

tracepatch0[6] = 0xde;

tracepatch0[7] = 0xb7;

tracepatch0[8] = 0x2a;

tracepatch0[9] = 0x97;

tracepatch0[10] = 0x0f;

tracepatch0[11] = 0xb6;

tracepatch0[12] = 0xf8;

tracepatch0[13] = 0x94;

tracepatch0[ 14 ] = 0xff;

tracepatch0[ 15 ] = 0xff;

tracepatch1[0] = 0x0f;

tracepatch1[1] = 0xb6;

tracepatch1[2] = 0xf8;

tracepatch1[3] = 0x94;

tracepatch1[4] = 0xde;

tracepatch1[5] = 0xbf;

tracepatch1[6] = 0x0f;

tracepatch1[7] = 0xbe;

tracepatch1[8] = 0xcd;

tracepatch1[9] = 0xbf;

tracepatch1[10] = 0xdf;

tracepatch1[11] = 0x91;

tracepatch1[12] = 0xcf;

tracepatch1[13] = 0x91;

tracepatch1[ 14 ] = 0xff;

tracepatch1[ 15 ] = 0xff;

//CLEANDEFEND$$$$$
 
//ADDTRACESTART$$$$$


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT1*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT2*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT3*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT4*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT5*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT6*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT7*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT10*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT8*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT9*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT11*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT12*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT13*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT14*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT15*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT16*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT17*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT18*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT19*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT20*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT21*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT22*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT23*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT24*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT25*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT26*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT27*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT30*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT28*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT29*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT31*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT32*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT33*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT34*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT35*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT36*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT37*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT38*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT39*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT40*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT41*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT42*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT43*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT44*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT45*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT46*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT47*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT50*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT48*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT49*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT51*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT52*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT53*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT54*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT55*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT56*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT57*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT58*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT59*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT60*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT61*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT62*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT63*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT64*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT65*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT66*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT67*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT70*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT68*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT69*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT71*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT72*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT73*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT74*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT75*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT76*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT77*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT78*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT79*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT80*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT81*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT82*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT83*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT84*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT85*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT86*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT87*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT90*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT88*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT89*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT91*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT92*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}



{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT93*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT94*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT95*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT96*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT97*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT98*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT99*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}

//ADDTRACEEND$$$$$

 while (1)
 {
 	hibernateThread();
 	
 }
 return 0; 
}

