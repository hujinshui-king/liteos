#ifndef COMMANDHANDLEH
#define COMMANDHANDLEH
#include "../types/types.h"
#include "../storage/filesys/fsapi.h"

/**\defgroup commands Interactive Shell API 
This module implements various command handlers. 
*/

/**@{ */

/** \brief This function defines the command handler for an incoming message. 
This is the key function that handles the incoming messages. Basically the messages themselves contain the info on how should be delivered and scheduled. 
\param receivebuffer The pointer to the start location of the incoming messages
\param total The total number of bytes of the incoming messages
\return VOID 
*/
void commandHandle(uint8_t * receivebuffer, uint8_t total);

/** @} */
void InitShell();
extern uint8_t shellbuffer[256];
void ShellThread();

//uint8_t createThreadFromFile(uint8_t *filename); 
//test the bootloader
//void create_thread_task();
#endif
