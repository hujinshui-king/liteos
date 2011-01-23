#ifndef INODEH
#define INODEH


#include "Types.h"
#include "commondefines.h"

int checkNodeValid(int addr);
void buildNewNode(int addr, char* name, uint8_t uid, uint8_t parent, uint8_t type);
void deleteNode(int addr);
void printNode(int addr);
void formatFS();
void buildRootNode();
int inodeMatch(uint8_t addr, char *string);
void copyVectorNode(int from, int to);
void copyVectorPage(int from, int to);

#endif


/*Following is the test code for this module 

#include "io_eeprom.h"
#include "inode.h"
#include "fs_config.h"

int main()
{


   int valid; 

   formatFS();
   buildNewNode(0, "root", 0, 0, DIRNODE);
   buildNewNode(10, "abc.h", 0, 0, FILENODE);
   buildNewNode(20, "test", 0, 0, DIRNODE);

   buildNewNode(30, "test1", 0, 20, FILENODE);
   buildNewNode(40, "test2", 0, 20, DIRNODE);
   buildNewNode(50, "test21", 0, 40, FILENODE);


   printNode(0);
   printNode(10);
   printNode(20);
   printNode(30);
   printNode(40);
   printNode(50);

   deleteNode(20);
   printNode(0);
   printNode(20);

   deleteNode(10);

   printNode(0);   

   valid = checkNodeValid(3);
   valid = checkNodeValid(6);

   //printNode(0);
   //printNode(10);
   //printNode(20);

 
  return 0;
    
}
*/
