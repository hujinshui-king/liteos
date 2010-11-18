#ifndef FSCONFIGH
#define FSCONFIGH
#include "../../types/types.h"

/** \ingroup filesystem 
*/

/** @{ */
#define FSROOTNODE 0
#define INODESIZE 32
#define INODENUM 96

/**@}*/

/** \addtogroup filesystem */

/**@{*/
///the relative position from next to current 
#define relativeAddr(current,next) (((next)-(current)+INODENUM)%INODENUM)
///the absolute addr from using current and the offset 
#define absoluteAddr(current,offset) ((current) + (offset) > INODENUM ? (\
(current) + (offset) - INODENUM):((current) + (offset)))

/** @} */

/** \ingroup filesystem */

/**@{ */
    ///node type structures 
     enum
     {
         DIRNODE = 1, FILENODE = 2, DEVNODE = 3, APPNODE = 4,
     };

///file inode structures 
#define FILENAMEOFFSET 0
#define TYPEOFFSET 12
#define VALIDOFFSET 13
///there are four types of files defined, directory, file, device, and user app 
struct directorynode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint8_t addr_subdir[10];
    uint8_t next;
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define DIR_ADDRSUBOFFSET 14
#define DIR_EXTRAOFFSET 24
#define DIR_NEXTOFFSET 24
#define DIR_PROTECTOFFSET 25
#define DIR_UIDOFFSET 28
#define DIR_SIZEOFFSET 29
#define DIR_PARENTOFFSET 31
struct filenode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint8_t addr_page[8];
    uint16_t endpageuse;
    uint8_t next;
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define FILE_ADDRPAGEOFFSET 14
#define FILE_ENDPAGEUSEOFFSET 22
#define FILE_EXTRAOFFSET 24
#define FILE_NEXTOFFSET 24
#define FILE_PROTECTOFFSET 25
#define FILE_UIDOFFSET 28
#define FILE_SIZEOFFSET 29
#define FILE_PARENTOFFSET 31
struct appnode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint32_t entrypoint;
    uint8_t padding[7];
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define APP_ENTRYOFFSET 14
#define APP_ENDPAGEUSEOFFSET 22
#define APP_EXTRAOFFSET 25
#define APP_PROTECTOFFSET 25
#define APP_UIDOFFSET 28
#define APP_SIZEOFFSET 29
#define APP_PARENTOFFSET 31
struct devnode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint16_t programentrypoint;
    uint16_t ramparameterentrypoint;
    uint16_t ramreturnentrypoint;

    //uint8_t parametersize; 
    uint8_t padding[5];
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define DEV_PROGRAMENTRYOFFSET 14
#define DEV_RAMENTRYOFFSET 16
//#define DEV_RAMSIZE 18
#define DEV_PROTECTOFFSET 25
#define DEV_UIDOFFSET 28
#define DEV_SIZEOFFSET 29
#define DEV_PARENTOFFSET 31

/**@}*/
//File system specific read and write of the byteStorage
uint8_t fsread8uint(int inode, int offset);
int8_t fsread8int(int inode, int offset);
uint16_t fsread16uint(int inode, int offset);
int16_t fsread16int(int inode, int offset);
uint32_t fsread32uint(int inode, int offset);
int32_t fsread32int(int inode, int offset);
void fswrite8uint(int inode, int offset, uint8_t value);
void fswrite8int(int inode, int offset, int8_t value);
void fswrite16uint(int inode, int offset, uint16_t value);
void fswrite16int(int inode, int offset, int16_t value);
void fswrite32uint(int inode, int offset, uint32_t value);
void fswrite32int(int inode, int offset, int32_t value);
void fsreadBytes(int inode, int offset, int nBytes, void *buffer);
void fswriteBytes(int inode, int offset, int nBytes, void *buffer);
void fsinitBytes(int inode, int offset, int nBytes, uint8_t value);
#endif
