/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

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


#ifndef LIBFILEH
#define LIBFILEH

#include "liteoscommon.h"


/** \defgroup file LiteFS API

      This module implements reading/writing files for the motes. 
      Testing code:

      \code
      
	#include "leds.h"
	#include "libthread.h"
	#include "adc.h"
	#include "radio.h"
	#include "Serial.h"
	#include "File.h"


	uint8_t receivedata[20]; 
	uint8_t receiveserial[7]; 
	uint8_t temp;
	uint16_t i; 
	uint16_t j; 

	MYFILE *fp; 
	int main()
	{
	 __asm__ __volatile__("sei" ::); 

	 for (i=0;i<20;i++)
 		 receivedata[i] = i*3; 

	  serialSend_string("Now testing file system\n\r");
	  serialSend_string("Open file abc\n\r");
	  fp =   mfopen("/abc", "w");
	  
	  for (i=0; i<10; i++)
	 {
	  mfwrite(fp , receivedata, 20); 
	  mfseek(fp, 20, 1);
	 }

	 mfclose(fp); 


 	 serialSend_string("File created\n\r");

	 serialReceive(3, 6, receiveserial); 

	 //serialSend_string("Command received\n\r");

	  fp =   mfopen("/abc", "r");
	  
	  for (i=0; i<10; i++)
	 {
	  mfread(fp , receivedata, 20);

	  
	  serialSend_string("\nNow send a 20 read\n\r");

	//	  serialSend(20, receivedata); 

	
	  mfseek(fp, 20, 1);
	
	 }
	
	 mfclose(fp); 

 
	return 0; 
	}

	\endcode
*/



/** \ingroup file 

  	Open a file based on its parameters and for later operations. 
  	\param pathname The path name of the file to be opened. 
  	\param mode The mode of the file to be opened
  	\return MYFILE The pointer to the file to be opened
*/

LIB_MYFILE *lib_mfopen(const char *pathname, const char *mode);

/** \ingroup file 

      Close a file based on the file pointer.
      \param fp The file pointer to the previously opened file. 
      \return NONE
*/
void lib_mfclose(LIB_MYFILE *fp); 

/** \ingroup file 

      Read data from a file
      \param fp The pointer of the previously opened file
      \param buffer The buffer for the read data
      \param nBytes The number of bytes to be read
      \return NONE
*/
void lib_mfread(LIB_MYFILE *fp, void *buffer, int nBytes);

/** \ingroup file 

      Write to a file. 
      \param fp The pointer of the previously opened file. 
      \param buffer The buffer for the data to be written. 
      \param nBytes The number of bytes to be written. 
      \return NONE
*/
void lib_mfwrite(LIB_MYFILE *fp, void *buffer, int nBytes);

/** \ingroup file 

      Seek the file for repositioning the file pointer
      \param fp The pointer of the previously opened file. 
      \param offset The offset of the seek operation
      \param position The position of the seek operation. 
      \return NONE
*/

void lib_mfseek(LIB_MYFILE *fp, int offset, int position);

/** \ingroup file 

      Write to a file without specifying the length
      \param fp The pointer of the previously opened file. 
      \param buffer The start position for the write buffer. 
      \return NONE
*/

void lib_mfwrite_withoutlength(LIB_MYFILE *fp, void *buffer);

#endif 
