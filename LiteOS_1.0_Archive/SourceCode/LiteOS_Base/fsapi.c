#include "thread_data.h"
#include "stdfsa.h"
#include "commondefines.h"
#include "fs_structure.h"
#include "vectornode.h"
#include "vectorflash.h"
#include "fsstring.h"
#include "fs_config.h"
#include "inode.h"
#include "io_eeprom.h"
#include "atmel-flash.h"


extern fid fidtable[8]; 






//This function opens a file according to a pathname and the mode. 
//The file opened must be a file, not program, device driver and directory 

MYFILE* fopen2(char *pathname, char *mode)
{
 
 int retaddr, state;
 int openmode; 

 retaddr = locateFileName(pathname, &state);
 openmode = fileMode(mode);
 
 //ERROR in open mode 
 if (openmode==0)
 	return NULL; 

//If the file location fails, return -1 

if (retaddr == -1)
 	return NULL; 
 	 
//first, file does not exist
 if (state==0)
  {
    if (openmode == 2)
    	{
           int blockaddr; 
           int fid;
		   char *temp;

		   temp = extractLastName(pathname);
           blockaddr = createFile(temp , retaddr);
           fid = getFreeFid();
           openFile(blockaddr, fid, openmode);
           return &fidtable[fid];    		    		
    	}
  
  	else return NULL; 
  }

if (state != FILENODE)
	 return NULL;

  {
	int fid;
	fid = getFreeFid();
	openFile(retaddr, fid, openmode);
	return &fidtable[fid];
	}
}



//This function closes the file and reports the result 

void  fclose2(MYFILE *fp)
{
	 releaseFid(fp->index);
	 return;
}


//This function changes the current fpos location 

int  fseek2 (MYFILE *fp, int offset, int position)
{
	 int temp; 

	 if (position == 0)
	 {
	 	  if (offset > fp->size)
	 	  	return 	-1;
	 	
	 	
	 fp->fpos = offset; 
	 return 0;
	 }
	 
	 temp = fp->fpos; 

	 if (position == 1)
	 	fp->fpos = fp->fpos + offset;
     
	 if ((fp->fpos <0)||(fp->fpos > fp->size))
	 {		  fp->fpos = temp; 
	      return -1; 
     }
	 	
	 if (position == 2)
	 	fp->fpos = fp->size + offset; 
	
	 if ((fp->fpos <0)||(fp->fpos > fp->size))
	 {		  fp->fpos = temp; 
	 return -1; 
	 }
 
	return 0;
}


//This function checks if the file specificed by pathname exists or not 
//it returns -1 if not exist, or the block id of the file if it does exist 
   
int fexist2(char *pathname)
{
	
 int retaddr, state;
 retaddr = locateFileName(pathname, &state);
 
 if ((retaddr == -1)||(state == 0))
 	return -1;
 
 else
 	return retaddr; 
	
}


//This function creates a directory as specified by the pathname parameter 
int   fcreatedir2(char *pathname)
{
  	
	int retaddr, state;

  retaddr = locateFileName(pathname, &state);


if (retaddr == -1)
 	return -1;
 	 
//first, file does not exist

 if (state==0)
  {
           createDir(extractLastName(pathname), retaddr);
           return 0;
 	}
  
  return -1;	
}



//Right now this is implemented in a simple way. Bsaically it checks the pathname and see if the file is there. If it is, 
//then it deletes the block of the file. 

int   fdelete2(char *pathname)
{
	int ret;
  	if ((ret = fexist2(pathname))==-1)
  		 return -1;
  	else
  		{
  		  deleteNode(ret);  			
  		  return 0; 
  		}
 }





int   fcheckEEPROM()
{
  return countVectorNode();	
}


int   fcheckFlash()
{
	return countVectorFlash();
}




//vs patch
void fsprintall()
{
  int i;
//  printf("*****************************************************************\n");
  for (i=0;i<=96;i++)
  {if (checkNodeValid(i)>0)  
	  printNode(i);
  }  
}







int   fread2(MYFILE *fp, void *buffer, int nBytes)
{

	 
	int startsector;
	int endsector; 
    uint8_t realsector;   
	int readbytes;
	int pagenum, blockoffset, pageoffset;  

	if (fp->fpos + nBytes > fp->size)
		 return -1; 

    startsector = fp->fpos/2048;
    endsector = (fp->fpos+nBytes-1)/2048;

	if (startsector == endsector)
	{
	   blockoffset = fp->fpos%2048;
	   pageoffset = blockoffset%256;

       pagenum = blockoffset/256;
       realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
       pagenum = pagenum+(realsector-1)*8;
	
	   //now pagenum, offset2 means the actual start location just read it

       readFlash(pagenum, pageoffset, buffer, nBytes);
	}
	else
	{
		blockoffset = fp->fpos%2048;
		pageoffset = blockoffset%256;

		pagenum = blockoffset/256;
		realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
		pagenum = pagenum+(realsector-1)*8;

        readbytes = 256 - pageoffset; 

		//now pagenum, offset2 means the actual start location just read it
		readFlash(pagenum, pageoffset, buffer, readbytes);

        buffer = (void *)((char *)buffer + readbytes); 

		blockoffset = (fp->fpos+nBytes-1)%2048;
		pageoffset = blockoffset%256;
		pagenum = blockoffset/256;

		realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+endsector);
		pagenum = pagenum+(realsector-1)*8;

		readbytes = nBytes - readbytes; 
		//now pagenum, offset2 means the actual start location just read it
		readFlash(pagenum, 0, buffer, readbytes);


	}
	return 0; 
}


int   fwrite2(MYFILE *fp, void *buffer, int nBytes)
{


	int startsector;
	int endsector; 

	uint8_t realsector; 

	int readbytes;

	int pagenum, blockoffset, pageoffset;  


	//first it checks whether there is enough space for the writing to take place, then it does the actual writing in the same way as above 
    if (fp->fpos + nBytes <= fp->size)
	{
	  //there is no need to change the size or allocate more space for the write

	}
	else
	{
      int allocate;
	  allocate = (fp->size/2048)*2048;
	  
	  if (fp->size%2048>0)
		   allocate+=2048;

	  if (fp->fpos + nBytes > allocate)
		   newSector(fp->addr);
      if (fp->fpos + nBytes > fp->size)
	  {
		  fp->size = fp->fpos + nBytes;
		  write16uint(fp->addr, FILE_SIZEOFFSET, fp->size);
	  }
	}





	startsector = fp->fpos/2048;
	endsector = (fp->fpos+nBytes-1)/2048;

	if (startsector == endsector)
	{
		blockoffset = fp->fpos%2048;
		pageoffset = blockoffset%256;

		pagenum = blockoffset/256;
		realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
		pagenum = pagenum+(realsector-1)*8;

		//now pagenum, offset2 means the actual start location just read it

		writeFlash(pagenum, pageoffset, buffer, nBytes);
	}
	else
	{
		blockoffset = fp->fpos%2048;
		pageoffset = blockoffset%256;

		pagenum = blockoffset/256;
		realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
		pagenum = pagenum+(realsector-1)*8;

		readbytes = 256 - pageoffset; 

		//now pagenum, offset2 means the actual start location just read it
		writeFlash(pagenum, pageoffset, buffer, readbytes);

		buffer = (void *)((char *)buffer + readbytes); 

		blockoffset = (fp->fpos+nBytes-1)%2048;
		pageoffset = blockoffset%256;
		pagenum = blockoffset/256;

		realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+endsector);
		pagenum = pagenum+(realsector-1)*8;

		readbytes = nBytes - readbytes; 
		//now pagenum, offset2 means the actual start location just read it
		writeFlash(pagenum, 0, buffer, readbytes);


	}	

	return 0;
}


//locate the file and change its parent name info and should be ok 
//to be implemented
//Only changes the parent information is necessary 

                                                                                                                                            
int   fmove(char *source, char *target)
{
	 //if ((ret = fexist(pathname))==-1)
	int ret1, ret2; 
	int state1, state2;
	uint8_t parent; 
	char *p, *q;
	uint8_t namelength; 

	p = extractLastName(target);

	q = target + strlen(target); 

	namelength = q-p ; 
	
	ret1 = locateFileName(source, &state1);
    
	parent = read8uint(ret1, FILE_PARENTOFFSET);

	removeChildNode(parent, ret1);

	ret2 = locateFileName(target, &state2);
	
	write8uint(ret1, FILE_PARENTOFFSET, ret2);

	writeBytes(ret1, FILENAMEOFFSET, namelength, p);

	write8uint(ret1, FILENAMEOFFSET+namelength, 0);

	addChildNode(ret2, ret1);

   return 0; 	
	
}


//to be implemented. Basically what this does is to generate a new file,copy the file information, and allocate new spaces and copy blocks
//it uses an interface called copyflash(sourcepage, targetpage)
//Implemented only to support files and the copy operation 


int   fcopy(char *source, char *target)
{
	//if ((ret = fexist(pathname))==-1)

	int ret1, ret2; 
	int state1, state2;
	
    uint8_t i;

	char *p;
    char *q;
	
	int NewNode; 

	uint8_t namelength; 

	p = extractLastName(target);

	q = target + strlen(target); 

	namelength = q-p ; 


	ret1 = locateFileName(source, &state1);

	ret2 = locateFileName(target, &state2);

    NewNode = getVectorNode();

	copyVectorNode(ret1, NewNode);

	writeBytes(NewNode, FILENAMEOFFSET, namelength, p);

	write8uint(NewNode, FILENAMEOFFSET+namelength,  0);
	
	write8uint(NewNode, FILE_PARENTOFFSET, ret2);

    

	addChildNode(ret2, NewNode);
 

	for (i=0;i<8;i++)
	{
	  uint8_t temp, temp1;
	  temp =  read8uint(ret1, FILE_ADDRPAGEOFFSET+i);
	  if (temp >0)
	  { temp1 = getFlashPage();
	    write8uint(NewNode, FILE_ADDRPAGEOFFSET+i, temp1);
	    copyVectorPage(temp, temp1);
	  }

	}

	return 0; 	
}


//according to the path information, change the dir needed

void fchangedir(char *path)
{
	int ret, state; 

	ret = locateFileName(path, &state);

	setPwd(ret);

	return; 

}


//get the current location given a buffer
//we expect that the buffer is big enough to hold the directory information 

void fcurrentdir(char *buffer, int size)
{

 
  int currentP; 
  char *p; 
  int len; 
  char namebuffer[13];
  int i; 
  char *q; 
  p = buffer + size -1;
  *p = '\0';
  p--; 
  
  
  currentP = getPwd();
  
  while (1)
  {

  
  getName(namebuffer, currentP);
 
  len = strlen(namebuffer);

  mystrncpy(p-len, namebuffer, len);
  
  *p = '/';

  p = p-len-1;  

  currentP = read8uint(currentP, 31);

  if (currentP == 0)
	   break;
  }

  *p = '/';

  len = buffer+size-1-p; 
 
  q = buffer; 
  for (i=0;i<len;i++)
	  *q++ = *p++;
  q--; 
  *q = '\0';
	   
  return;
}



//this function is used for the ls command 
void finfonode(char *buffer, int addr)
{
   readBytes(addr, FILENAMEOFFSET, 12, buffer);
   buffer[12] = read8uint(addr, TYPEOFFSET); 
   buffer[13] = read8uint(addr, 29); 
   buffer[14] = read8uint(addr, 30); 
  
}


//search for the information based on the string required. The results are put into addrlist, with size 

void fsearch(uint8_t *addrlist, uint8_t *size, char *string)
{
   uint8_t i;
   uint8_t *p;
   p = addrlist; 
   *size = 0;

   for (i=1;i<=INODENUM;i++)
   {
	   if (inodeMatch(i, string)==1)
	   {
		   p[*size] = i;
           (*size)++;
	   }
   }
   return; 
}



void formatSystem()
{
	formatFS();
}

//This function gets the node directory information and puts it into the buffer

void fdirnode(char *buffer, int size, int addr)
{
	int currentP; 
	char *p; 
	int len; 
	char namebuffer[13];
	int i; 
	char *q; 
	p = buffer + size -1;
	*p = '\0';
	p--; 


	currentP = addr; 

	while (1)
	{


		getName(namebuffer, currentP);

		len = strlen(namebuffer);

		mystrncpy(p-len, namebuffer, len);

		*p = '/';

		p = p-len-1;  

		currentP = read8uint(currentP, 31);

		if (currentP == 0)
			break;
	}

	*p = '/';

	len = buffer+size-1-p; 

	q = buffer; 
	for (i=0;i<len;i++)
		*q++ = *p++;
	q--; 
	*q = '\0';

	return;
}
