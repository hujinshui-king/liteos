#include "fs_structure.h"
#include "inode.h"
#include "commondefines.h"
#include "fs_config.h"

fid fidtable[4];


//check whether the file system is present in the system

int checkFsValid()
{
  if (checkNodeValid(FSROOTNODE)==DIRNODE)
  	 return 1;
  else return 0;	
}





//init the fid table used to keep track of the nodes 
void initFidTable()
{
	int i;
	for (i=0;i<8;i++)
	 fidtable[i].valid = 0;
}


//get a free fid 
int getFreeFid()

{
	int i;
	for (i=0;i<8;i++)
	 {
	   if (fidtable[i].valid == 0)
	   	break;	
	 }
	
	fidtable[i].valid = 1;
	fidtable[i].index = i;
	fidtable[i].addr = fidtable[i].mode = 0;
	fidtable[i].fpos = -1;
	return i; 
}


//release a fid 
void releaseFid(int fid)
{
	fidtable[fid].valid = 0;
}


