#include "fsstring.h"
#include "commondefines.h"

char* extractString(char* start, char *buffer)
{
	char *p,*q;
	
	q = buffer;
	p = start; 
	
	if (*p=='/')
		 p++;
  
  while ((isLetter(*p))||(*p=='_')||(*p=='.')||(isDigit(*p)))
	  *q++=*p++;
	*q = '\0';
	return p; 
	
}


char* extractLastName(char *pathname)
{
  	char *p,*start;
  	int length;
  	length = strlen(pathname);
  	p = pathname+(length-1);
  	start = pathname;
  	while ((*p!='/')&&(p!=start))
  	 p--;
    if ((p==start)&&((*start)!='/'))
    	return p;
    else
    	return ++p;	
	
}
