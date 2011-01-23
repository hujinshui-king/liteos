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

#ifndef LIBADCH
#define LIBADCH

/** \defgroup device Sensor API

      This module implements sensor related API for reading from light, temperature, magnetic, and accelerator sensors. 
*/




/** \ingroup device 

      Return light sensor reading. 
      \param NONE
      \return The light sensor reading
*/
int lib_get_light();


/** \ingroup device 

      Return temperature sensor reading. 
      \param NONE
      \return The temperature sensor reading
*/
int lib_get_temp();

/** \ingroup device 

      Return magnetic sensor reading in X axis. 
      \param NONE
      \return The magnetic sensor reading in X axis
*/
int lib_get_magx();

/** \ingroup device 

      Return magnetic sensor reading in Y axis. 
      \param NONE
      \return The magnetic sensor reading in Y axis
*/
int lib_get_magy();

/** \ingroup device 

      Return accelerator sensor reading in X axis. 
      \param NONE
      \return The magnetic sensor reading in X axis
*/
int lib_get_accx();

/** \ingroup device 

      Return accelerator sensor reading in Y axis. 
      \param NONE
      \return The magnetic sensor reading in Y axis
*/
int lib_get_accy(); 



#endif 
