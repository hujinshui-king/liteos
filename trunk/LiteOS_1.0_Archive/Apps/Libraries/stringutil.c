/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "stringutil.h"



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


int String_length(char* s) {
	int count=0;
	while(s[count] != '\0')
		count++;
	return count;
}




void mystrncpy(char *dest, const char *src, uint16_t n)
{
	uint16_t i;
	for (i=0;i<n;i++)
		dest[i] = src[i];
}


void mystrcpy(char *dest, const char *src)
{
	int i;
	i = 0;
	while (src[i]!='\0')
	{ dest[i] = src[i];
	i++;
	}
	dest[i] = '\0';
}


char dec2char(uint8_t value) {

	if (/*value >= 0) &&*/(value <=9))
		return (char) (value + 0x30);
	else
		return 0;
}


char *String_intToString(int num)
{
	static char temp[7];
	int internal;
	uint8_t length;
	uint8_t i;
	uint8_t offset = 5;
	uint8_t remainder;

	if (num <0)
		internal = -num;
	else
		internal = num;

	temp[6] = '\0';

	do
	{
		remainder = internal%10;
		temp[offset]= dec2char(remainder);
		internal = internal/10;
		offset--;
	}
	while (internal!=0);

	if (num<0)
	{temp[offset] = '-';
	 if (offset ==0)
		 return temp;
	 else
		 offset--;

	}




	{
		length = 7-offset-1;
		for (i=0;i<length;i++)
			temp[i] = temp[i+offset+1];
	}
	return temp;
}

void String_append(char *base, char *string)
{
	int length1, length2;
	uint8_t i;
	length1 = String_length(base);
	length2 = String_length(string);
	for (i=0;i<length2;i++)
	{
		base[i+length1] = string[i];

	}
	base[length1+length2] = '\0';
}

char* string_split(char ** string, char delimiter)
{
	uint16_t i;
	char* output = (*string);

	for (i = 0; output[i] != 0; i++) 
		if (output[i] == delimiter) {
			output[i] = 0;
			*string = output + i + 1;
			return output;
		}

	*string = output + i;
	return output;
}

