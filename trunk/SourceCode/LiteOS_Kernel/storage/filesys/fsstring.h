#ifndef FSSTRINGH
#define FSSTRINGH

/** \addtogroup filesystem */

/**@{*/

/** Extract the string. For example, /c/Temp extracts c first.  
@param start the string to be extracted
@param buffer the buffer to put the string
@return the next string to be extracted in the original string 
*/
char *extractString(char *start, char *buffer);

/** Extract last file name in the path name parameter */
char *extractLastName(char *pathname);
int fileMode(char *s);

/**@}*/
#endif
