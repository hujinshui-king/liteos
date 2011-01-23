#include "commondefines.h"



char networkid[6]; 
uint8_t nodeid;  
char filenameid[11];  

int strlen(char* s) {
	int count=0;
	while(s[count] != '\0')
		count++;
	return count;
}




int fileMode(char* s)
{
  if (strlen(s)==1)
  	 {
  	   if (s[0]=='r')
  	   	 return 1;
  	   if (s[0]=='w')
  	   	 return 2;
  	   if (s[0]=='a')
  	   	 return 3;
  	   if (s[0]=='t')
  	   	 return 4;
  	 }
  else if (strlen(s)==2)
  	return 5;
  	
  else return 0;
  
  return 0; 	
}


int superstring(char *string1, char *string2)
{
	char *p, *q;
	p = string1; 
	q = string2; 
	while ((*p==*q)&&(*p!='\0'))
	{
		p++;
		q++;
	}
   if ((*q=='\0')&&(*p!='\0'))
	   return 0;
   if ((*q=='\0')&&(*p=='\0'))
       return 0;
   return 1; 
}

void mystrncpy(char *dest, const char *src, uint16_t n)
{
   int i;
   for (i=0;i<n;i++)
     dest[i] = src[i]; 
   

}




int mystrncmp(char *s, uint8_t start1, char *t, uint8_t start2, uint8_t length)
{
   uint8_t i;
   for (i=0;i<length;i++)
    {
	  if (s[i+start1]!=t[i+start2])
	    return 1; 	
	}
	return 0;   
}


uint16_t hex2value(uint8_t hex)
{
  char a = (char)hex; 
  uint16_t high; 
          
  if ((a>='0')&&(a<='9'))
	     high = a-'0';
  else
		 high = a-'A'+ 10; 
  return high; 

}


