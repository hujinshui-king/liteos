#include "inode.h"
#include "Types.h"
#include "fs_config.h"
#include "commondefines.h"
#include "io_eeprom.h"
#include "vectornode.h"
#include "vectorflash.h"
#include "atmel-flash.h"

//check wehther node nodeaddr is a valid node that has been assigned 
int checkNodeValid(int nodeaddr)
{
	uint8_t validness; 
	uint8_t nodetype;
	
  validness = read8uint(nodeaddr, VALIDOFFSET);
  nodetype = read8uint(nodeaddr, TYPEOFFSET);
     
  if (validness == 0)
  	 {
       return 0;
     }	
	else
	     return (int)nodetype;
}
 
 
//build a new node based on the type, parent, uid, name, and addr information 

void buildNewNode(int addr, char* name, uint8_t uid, uint8_t parent, uint8_t type)
{
   uint8_t namelength; 
   namelength = strlen(name);
   if (namelength <12)
   namelength++;

   //consider several types of type
   if (type == DIRNODE)
   	{

   		writeBytes(addr, FILENAMEOFFSET, namelength, name);
   		write8uint(addr, TYPEOFFSET, type);
   		write8uint(addr, VALIDOFFSET, 1);
   		initBytes (addr, DIR_ADDRSUBOFFSET, 14, 0);
   		write8uint(addr, DIR_UIDOFFSET, uid);
   		write16uint(addr, DIR_SIZEOFFSET, 0);
   		write8uint(addr, DIR_PARENTOFFSET, parent);  		
   		
   	}
   else if (type == FILENODE)
   	{
      writeBytes(addr, FILENAMEOFFSET, namelength, name);
   		write8uint(addr, TYPEOFFSET, type);
   		write8uint(addr, VALIDOFFSET, 1);
   		initBytes (addr, FILE_ADDRPAGEOFFSET, 14, 0);
   		write8uint(addr, FILE_UIDOFFSET, uid);
   		write16uint(addr,FILE_SIZEOFFSET, 0);
   		write8uint(addr, FILE_PARENTOFFSET, parent);  		
      		
   	}
   
   else if (type == DEVNODE)
   	{
      		
   	  writeBytes(addr, FILENAMEOFFSET,namelength, name);
   		write8uint(addr, TYPEOFFSET, type);
   		write8uint(addr, VALIDOFFSET, 1);
   		initBytes (addr, DEV_ENTRYOFFSET, 14, 0);
   		write8uint(addr, DEV_UIDOFFSET, uid);
   		write16uint(addr,DEV_SIZEOFFSET, 0);
   		write8uint(addr, DEV_PARENTOFFSET, parent);  	
   		
   	}
   
   else if (type == APPNODE)
   	{
   		writeBytes(addr, FILENAMEOFFSET, namelength, name);
   		write8uint(addr, TYPEOFFSET, type);
   		write8uint(addr, VALIDOFFSET, 1);
   		initBytes (addr, APP_ENTRYOFFSET, 14, 0);
   		write8uint(addr, APP_UIDOFFSET, uid);
   		write16uint(addr,APP_SIZEOFFSET, 0);
   		write8uint(addr, APP_PARENTOFFSET, parent);  
   	}
   	
   if ((addr==0)&&(parent==0))
    {}
   else 
   	{
   		 int i;
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(parent, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr==0)
   		 	 	 {
   		 	 	 	 write8uint(parent, DIR_ADDRSUBOFFSET+i, addr);
   		 	 	 	 break;
   		 	 	 }  		 	
   		 }   		    		
   	}
   	
   	return;
   	
}


void printNode(int addr)
{
   	char filename[13];
   	uint8_t uid, parent, type, valid;
   	 
    uint8_t subdirectory[10];
	int i;
	uint8_t flashdirectory[8];


    filename[12] = '\0';
   	readBytes(addr, FILENAMEOFFSET, 12, filename);
   	type = read8uint(addr, TYPEOFFSET);


	  valid = read8uint(addr, VALIDOFFSET);

   if ((valid >0) &&( type == DIRNODE))
     {
	   for (i=0;i<10;i++)
	     subdirectory[i] = read8uint(addr, DIR_ADDRSUBOFFSET+i);
	 
	 
	 }

   if ((valid >0) &&( type == FILENODE))
   {
	   for (i=0;i<8;i++)
		   flashdirectory[i] = read8uint(addr, FILE_ADDRPAGEOFFSET+i);


   }
   
      uid = read8uint(addr, 28);
	  parent = read8uint(addr, 31);

	  //vs patch 
//	  printf("****Node %d*************\n", addr);
//	  printf("filename: %s\n", filename);
//	  printf("type (dir, file, dev, app) %d\n", type);
//	  printf("valid is %d\n", valid);
//	  if (type == DIRNODE)
//	  {for (i=0;i<10;i++)
//	  printf("The %d sub directory is %d\n", i, subdirectory[i]);
//	  }
	  if (type == FILENODE)
	  {
//		  for (i=0;i<8;i++)
//			  printf("The %d sub flash is %d\n", i, flashdirectory[i]);
	  }
//	  printf("The node parent is %d\n", parent);
//	  printf("*************************\n");

	  return; 
	
}




void formatFS()
{
   int i;
   for (i=0;i<=96;i++)
   initBytes(i, 0, 32, 0);
   //  write8uint(i, VALIDOFFSET, 0); 

   initVectorFlash();
   initVectorNode();

}
//to detelet the complete node, even if it is a directory, will delete recursively. 
//the parent of this addr will also be modified 
//now only handles the file system change. for device and program, the OS must change its scheduler as well


void deleteNode(int addr)
{
	uint8_t type;
	uint8_t parent; 
	type = read8uint(addr, TYPEOFFSET);
	parent = read8uint(addr, 31);
	
  if (type == DIRNODE)
   	{

   		 uint8_t i;
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(addr, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr > 0)
   		 	 	 {
   		 	 	 	 deleteNode(subaddr);
   		 	 	 }  		 	
   		 }   		  
   	 write8uint(addr, VALIDOFFSET, 0);
   	 if ((addr==0)&&(parent==0))
     {}
     else 
   	 {
   		 uint8_t i;
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(parent, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr== addr)
   		 	 	 {
   		 	 	 	 write8uint(parent, DIR_ADDRSUBOFFSET+i, 0);
   		 	 	 	 break;
   		 	 	 }  		 	
   		 }   		    		
   	 }			
   	}
   else if (type == FILENODE)
   	{
	 char i;
	 uint8_t readpage;
     write8uint(addr, VALIDOFFSET, 0);
   	 if ((addr==0)&&(parent==0))
     {}
     else 
   	 {
   		
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(parent, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr== addr)
   		 	 	 {
   		 	 	 	 write8uint(parent, DIR_ADDRSUBOFFSET+i, 0);
   		 	 	 	 break;
   		 	 	 }  		 	
   		 }   		    		
   	 }
	
     for (i=0;i<8;i++)
	 {
	  readpage = read8uint(addr, FILE_ADDRPAGEOFFSET+i);
	  if (readpage==0)
		  break;
	  else
	     releaseFlashPage(readpage);
	 }


    }
   
   else if (type == DEVNODE)
   	{
      		
   	 write8uint(addr, VALIDOFFSET, 0);
   	 if ((addr==0)&&(parent==0))
     {}
     else 
   	 {
   		 int i;
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(parent, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr== addr)
   		 	 	 {
   		 	 	 	 write8uint(parent, DIR_ADDRSUBOFFSET+i, 0);
   		 	 	 	 break;
   		 	 	 }  		 	
   		 }   		    		
   	 }		
   		
   	}
   
   else if (type == APPNODE)
   	{
   		write8uint(addr, VALIDOFFSET, 0);
   	 if ((addr==0)&&(parent==0))
     {}
     else 
   	 {
   		 int i;
   		 for (i=0;i<10;i++)
   		 {
   		 	 uint8_t subaddr;
   		 	 subaddr = read8uint(parent, DIR_ADDRSUBOFFSET+i);
   		 	 if (subaddr== addr)
   		 	 	 {
   		 	 	 	 write8uint(parent, DIR_ADDRSUBOFFSET+i, 0);
   		 	 	 	 break;
   		 	 	 }  		 	
   		 }   		    		
   	 }		
   	}
   	 
	releaseVectorNode(addr);
	
}



//initilize the system 
//void buildNewNode(int addr, char* name, uint8_t uid, uint8_t parent, uint8_t type);
void buildRootNode()
{
	 buildNewNode(FSROOTNODE, "root", 0, 0, DIRNODE);	 	
}



void copyVectorNode(int from, int to)
{
   int i;
   for (i=0;i<32;i++)
   {
	   uint8_t temp; 
	   temp = read8uint(from, i);
	   write8uint(to, i, temp);
   }
}


void copyVectorPage(int from, int to)
{
   int i;
   int pagefrom, pageto; 
   for (i=0;i<8;i++)
   {
	   pagefrom = i+(from-1)*8;
	   pageto = i+(to-1)*8;
	   copyFlash(pagefrom, pageto);
   }
   return;
}


int inodeMatch(uint8_t addr, char *string)
{
	char temp[13];
	uint8_t length; 
    uint8_t i; 
    uint8_t valid; 
	valid = read8uint(addr, VALIDOFFSET);
	if (valid == 0)
	return 0; 
   
 
	readBytes(addr, FILENAMEOFFSET, 12, temp);
	temp[12] = '\0';
    length = strlen(temp);
	for (i=0;i<length;i++)
	{	if (superstring(temp+i, string)==0)
			return 1;
	}
	return 0; 

}
