#include "commandHandle.h"
#include "stdfsa.h"
#include "io_eeprom.h"
#include "fs_config.h"
#include "testUSART2.h"
#include "usertask.h"
#include "commondefines.h"
#include "inode.h"
#include "threads.h"
#include "bootloader.h"
#include "Types.h"
#include "PacketHandler.h"

extern char networkid[6]; 
extern char filenameid[11]; 
extern uint8_t nodeid;
extern fid fidtable[8];  
extern volatile thread * current_thread;

extern volatile uint8_t thread_task_active ;

uint8_t IncomingLength; 
uint8_t IncomingMsg[32]; 
uint8_t PacketInfo[2]; 


uint8_t shellbuffer[128]; 
thread *thisthread; 


//const char *programstring = "20064E57000C94C6640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E36411241FBEC0E0D7E0DEBFCDBF11E0A0E0B1E0EEEDFAEC00E00BBF02C007900D92A030B107D9F715E0A0E0B5E001C01D92A630B107E1F70C94E5640C948064C0E0D7E0DEBFCDBF88EE93E00E940A650E945565F9CFCF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF80E09AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF9A8389838BE494E09093030580930205E0910205F09103050190F081E02D84E08283E0910205F09103050190F081E02D89819A81968385830E94F06422960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF89E09AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF82E19AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895";
MYFILE *openedfile; 
unsigned char reply[32]; 

void reply101()
{

   uint8_t temp = strlen(networkid); 
   reply[0] = temp + 3;
   reply[1] = 101;
   reply[2] = nodeid; 
   mystrncpy(&reply[3], networkid, temp); 

   printStringN(reply,32); 

  //printString("hello,reply2!\n\0");



}


void reply102(uint8_t *receivebuffer)
{
   uint8_t temp = receivebuffer[0] - 3;  
   if (mystrncmp((char *)receivebuffer, 3, (char *)networkid, 0, temp)==0)
      {   
       temp = strlen(filenameid); 
       reply[0] = temp + 3;
       reply[1] = 102;
       reply[2] = nodeid; 
       mystrncpy(&reply[3], filenameid, temp); 
       printStringN(reply,32); 
      }
}


//Get the block information of sub directory and files using simplfied version 
void reply103(uint8_t block)
{

//Actually this is not used because to ensure that the system is simple, we only use the long version 
return; 


}



//Get the block information of sub directory and files using long version 
void reply104(uint8_t block)
{

  uint8_t blockindex;
  uint8_t childblock; 
  uint8_t seq = 0; 

  for (blockindex = 0; blockindex <10; blockindex++)
  {
     childblock = read8uint(block, DIR_ADDRSUBOFFSET+blockindex); 
     if (childblock == 0)
	   continue; 
     
     reply[0] = 18;
	 reply[1] = 104; 
	 reply[2] = nodeid; 
	 reply[3] = childblock; 
	 reply[4] = seq; 
	 seq ++;
     finfonode(&reply[5], (int)childblock); 
     printStringN(reply,32); 
  
  }
}




void reply141(uint8_t *receivebuffer)
{
    uint8_t blockid; 
	char dirname[13];
    uint8_t newblockid; 

    blockid = receivebuffer[3]; 
	mystrncpy(dirname, &receivebuffer[4], receivebuffer[0]-4); 
	dirname[receivebuffer[0]-4] = '\0'; 
	newblockid = createDir(dirname, (int)blockid);
   reply[0] = 4;
   reply[1] = 141;
   reply[2] = nodeid; 
   reply[3] = newblockid; 
  
   printStringN(reply,32); 	
}


void reply121(uint8_t *receivebuffer)
{
    uint8_t blockid; 
	char filename[13];
	int fid; 

    uint8_t newblockid; 

    blockid = receivebuffer[3]; 
	mystrncpy(filename, &receivebuffer[4], receivebuffer[0]-4); 
	filename[receivebuffer[0]-4] = '\0'; 
    newblockid = createFile(filename, (int)blockid);
    
   reply[0] = 4;
   reply[1] = 121;
   reply[2] = nodeid; 
   reply[3] = newblockid; 

   fid = getFreeFid();
   openFile(newblockid, fid, 2);
   openedfile = &fidtable[fid];    		    

  
   printStringN(reply,32); 	
}

void reply122(uint8_t *receivebuffer)
{

  fwrite2(openedfile, &receivebuffer[7], receivebuffer[6]); 
  fseek2 (openedfile, receivebuffer[6], 1);
  printStringN(reply,32); 	
  
}

void reply123()
{
 

  fclose2(openedfile);  
  printStringN(reply,32); 	
 

}
void reply151(uint8_t *receivebuffer)
{
    uint8_t blockid; 
	char filename[13];
    uint8_t newblockid; 

    blockid = receivebuffer[3]; 
	mystrncpy(filename, &receivebuffer[4], receivebuffer[0]-4); 
	filename[receivebuffer[0]-4] = '\0'; 
	newblockid = createFile(filename, (int)blockid);
    
   reply[0] = 4;
   reply[1] = 151;
   reply[2] = nodeid; 
   reply[3] = newblockid; 
  
   printStringN(reply,32); 	
}



void reply124(uint8_t *receivebuffer)
{
   
    uint8_t blockid; 
	char filename[13];
	int fid; 
	MYFILE *fp; 
	uint16_t filesize; 
	uint16_t round; 
	uint16_t remain; 
    uint16_t i; 
	uint8_t newblockid; 
	uint8_t temp1, temp2; 
    

    blockid = receivebuffer[3]; 
	mystrncpy(filename, &receivebuffer[4], receivebuffer[0]-4); 
    filename[receivebuffer[0]-4] = '\0'; 
	   
    newblockid = existBlockAddr(filename, (int)blockid); 
    
	reply[1] = 124;
    reply[2] = nodeid; 
   

   

    fid = getFreeFid();
    openFile(newblockid, fid, 1);
    fp = &fidtable[fid]; 
	filesize =fp->size;

    temp1 = filesize/256;
	temp2 = filesize%256; 

	reply[3] = temp1; 
	reply[4] = temp2; 

	printStringN(reply, 32);


    for (round = 0; round <1000;round++)
	for (remain =0; remain<30;remain++)
	{
	  temp1 = 2; 
	}


    reply[3] = newblockid; 

	round  = filesize/20; 
	remain = filesize%20; 

	for (i=0;i<round;i++)
	 {
	   fread2(fp, &reply[4], 20);
	   fseek2(fp, 20, 1);
	   reply[0] = 24; 
       printStringN(reply,32); 		    
	 }
	 
	if (remain >0)
     {
       fread2(fp, &reply[4], remain);
	   reply[0] = (uint8_t)remain +4; 
       printStringN(reply,32);
	    	
     }

	 fclose2(fp); 

}


void reply161(uint8_t *receivebuffer)
{
   
    uint8_t blockid;   
	char filename[13];
    uint8_t newblockid; 

    blockid = receivebuffer[3]; 
	mystrncpy(filename, &receivebuffer[4], receivebuffer[0]-4); 
	filename[receivebuffer[0]-4] = '\0'; 
	
	
	newblockid = existBlockAddr(filename, (int)blockid);
    
    deleteNode(newblockid); 

    reply[0] = 4;
    reply[1] = 161;
    reply[2] = nodeid; 
    reply[3] = newblockid; 
  
   printStringN(reply,32); 	


}


void reply211(uint8_t *receivebuffer)
{
   
    int e1, e2, e3; 

	e1 = fcheckEEPROM();
	e2 = fcheckFlash(); 
	e3 = 0; 

    if (e2==256)
	 {
	  e2 = 128;
	  e3 = 128; 
	 }

 
    reply[0] = 6;
    reply[1] = 211; 
    reply[2] = nodeid; 
    reply[3] = e1; 
	reply[4] = e2; 
	reply[5] = e3; 
  
   printStringN(reply,32); 	
}


void reply221(uint8_t *receivebuffer)
{
    
	char filename[13];
    uint8_t searchresults[20];
    uint8_t numofsearch; 
	uint8_t i; 

	mystrncpy(filename, &receivebuffer[3], receivebuffer[0]-3); 
	filename[receivebuffer[0]-3] = '\0'; 
	
	
	fsearch(searchresults, &numofsearch, filename);
   
   for (i=0;i<numofsearch;i++)
   {
    fdirnode(&reply[3], 29, searchresults[i]);
    reply[0] = strlen(&reply[3])+3;  
	reply[1] = 221; 
	reply[2] = nodeid;    
    printStringN(reply,32); 	
   }

}





void reply231(uint8_t *receivebuffer)
//void reply231()
{
   
    uint8_t blockid; 
	char filename[13];
	int fid; 
	uint8_t tempbuffer[20]; 
	MYFILE *fp; 
	uint16_t filesize; 
	uint16_t round; 
	uint8_t remain; 
    uint16_t i; 
	uint8_t newblockid; 
    uint8_t sysinfo[10]; 

    uint16_t pageno; 
	uint16_t entryaddr; 
	uint16_t ramstackend; 


   blockid = receivebuffer[3]; 
	mystrncpy(filename, &receivebuffer[4], receivebuffer[0]-4); 
    filename[receivebuffer[0]-4] = '\0'; 
	   
    newblockid = existBlockAddr(filename, (int)blockid); 
    
	reply[1] = 231;
    reply[2] = nodeid; 
    
   

    fid = getFreeFid();
    openFile(newblockid, fid, 1);
    fp = &fidtable[fid]; 
	filesize =fp->size;

    fread2(fp, sysinfo, 10);
	fseek2(fp, 10, 1); 



	pageno = hex2value(sysinfo[0])*100+hex2value(sysinfo[1])*10+hex2value(sysinfo[2]);
	entryaddr =  (uint16_t)(hex2value(sysinfo[3]))*4096+(uint16_t)(hex2value(sysinfo[4]))*256+ (uint16_t)(hex2value(sysinfo[5]))*16+ (uint16_t)(hex2value(sysinfo[6]));
    ramstackend = (uint16_t)(hex2value(sysinfo[7]))*256 + (uint16_t)(hex2value(sysinfo[8]))*16 + (uint16_t)(hex2value(sysinfo[9])); 


	round  = (filesize-10)/16; 
	remain = (filesize-10)%16; 

    ProgramSetPage(pageno); 

	for (i=0;i<round;i++)
	 {
	   fread2(fp, tempbuffer, 16);
	   fseek2(fp, 16, 1);
//       mystrncpy(tempbuffer, &programstring[i*16+10], 16); 
	   ProgramString(tempbuffer);  		    
	 }
	 
	if (remain >0)
     {
       fread2(fp, tempbuffer, remain);
	   //mystrncpy(tempbuffer, &programstring[round*16+10], remain);
	   ProgramLastString(tempbuffer, remain); 
	    	
     }
   else
     {
	 ProgramRemains();
     }
    
	 fclose2(fp); 

	{
 
     void (*task2)(void) = (void (*)(void))entryaddr; 
     uint16_t  * stackpointer; 
     stackpointer = (uint16_t *)ramstackend; 
     create_thread(task2, stackpointer, 10);

    }

    printStringN(reply,32); 


}




void WakeupMe()
{
    thisthread->state = STATE_ACTIVE; 
	if (thread_task_active==0)
       postTask(thread_task, 2);
}


void InitShell()
{
   IncomingLength = 0; 
   registerEvent(0xfefe, 32, &IncomingLength, IncomingMsg, PacketInfo, WakeupMe);


}



//This is the stub function of the shell thread that is used to handle incoming packets 

void ShellThread()
{
  while(1)
  	{
   if (IncomingLength > 0)
   	{
       commandHandle(IncomingMsg, IncomingLength);   
	   IncomingLength = 0; 
 
    }
   else
   	{
      current_thread->state = STATE_SLEEP;
	  thisthread = current_thread; 
      yield(); 
    }
  	}

}


void  commandHandle(uint8_t *receivebuffer, uint8_t total)
{
    uint8_t commandtype; 

  
    if (!((receivebuffer[2] == 0)||(receivebuffer[2] == nodeid)))
     return; 
  

   commandtype = receivebuffer[1];
   
   switch (commandtype) {
     case  101:
        reply101();
	   break; 

     case 102:
	    reply102(receivebuffer);
	   break;

	 case 103:
	    reply104(receivebuffer[3]);
       break;

	 case 104:
	    reply104(receivebuffer[3]); 
	   break; 

     case 121:
	    reply121(receivebuffer); 
		break;
		 
     case 122:
	    reply122(receivebuffer);
		break; 
  
     case 123:
	    reply123();
		break; 
  
     case 124:
	    reply124(receivebuffer);
		break;

	 case 141:
	    reply141(receivebuffer); 
	   break; 

     case 151:
	    reply151(receivebuffer); 
	   break; 

     case 161:
	    reply161(receivebuffer); 
	   break; 

     case 211:
	    reply211(receivebuffer); 
       break; 
	
	 case 221:
	    reply221(receivebuffer); 
       break; 

     case 231:
	    reply231(receivebuffer); 
	
	 }
       
   
 

}
