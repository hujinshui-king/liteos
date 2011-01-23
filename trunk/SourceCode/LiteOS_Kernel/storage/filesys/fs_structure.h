
#ifndef FSSTRUCTUREH
#define FSSTRUCTUREH

#include "fsapi.h"
extern fid fidtable[MAX_FILE_TABLE_SIZE];

/** \addtogroup filesystem */

/** @{ */
///check whether the file system is present in the system
int checkFsValid();

///init the fid table used to keep track of the nodes 
void initFidTable();

///get a free fid 
int getFreeFid();

/////release a fid
void releaseFid(int fid);

void releaseAllFid();

/**@}*/
#endif
