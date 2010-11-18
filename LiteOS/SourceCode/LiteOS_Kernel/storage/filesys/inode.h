#ifndef INODEH
#define INODEH
#include "../../types/types.h"

/**\addtogroup filesystem 
*Following is the code to test this module 
\code 
Following is the test code for this module 
#include "ioeeprom.h"
#include "inode.h"
#include "fsconfig.h"
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
\endcode
*/

/**@{*/
///check wehther node nodeaddr is a valid node that has been assigned 
int checkNodeValid(int addr);

///build a new node based on the type, parent, uid, name, and addr information 
void buildNewNode(int addr, char *name, uint8_t uid, uint8_t parent, uint8_t
                  type);
///to detelet the complete node, even if it is a directory, will delete recursively. 
///the parent of this addr will also be modified 
///now only handles the file system change. for device and program, the OS must change its scheduler as well
void deleteNode(int addr);
void printNode(int addr);
void formatFS();
void buildRootNode();
int inodeMatch(uint8_t addr, char *string);
void copyVectorNode(int from, int to);
void copyVectorPage(int from, int to);
void formatFSLite();

/**@}*/
#endif
