/* The LiteOS Operating System Kernel */
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
 
#ifndef ERRORLOGH
#define ERRORLOGH
/* \defgroup debug Debugging Utilities
   These definitons collectively form kernel support for output information as it runs. The system uses preprocessor to reduce the system overhead. To use, follow
   the following example. The printf function sends the data to the standard output, usually serial output on MicaZ, for example 
   \code
   #ifdef AMRADIO_FUNCTIONA
   printfstr("This is trace point A at AM RADIO Function A"); 
   #endif
   \endcode
 */
//Radio stack output debugging information
#define DEUBG_RADIO_ON
#ifdef DEBUG_RADIO_ON
//The radio moduel debugging output is turned on 
#define AMRADIO_FUNCTIONA
#else
//The radio module debugging output is turned off 
#endif
#endif
