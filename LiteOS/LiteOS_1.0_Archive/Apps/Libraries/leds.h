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

#ifndef LEDSH
#define LEDSH

/** \defgroup leds LED API

      This module implements controlling the LED of the motes. 
*/


/** \ingroup leds

     Toggle the green LED. 
     \param NONE
     \return NONE
*/

void greenToggle();

/** \ingroup leds

     Toggle the red LED. 
     \param NONE
     \return NONE
*/

void redToggle();


/** \ingroup leds

     Toggle the yellow LED. 
     \param NONE
     \return NONE
*/

void yellowToggle();

/** \ingroup leds

     Turn on the green LED. 
     \param NONE
     \return NONE
*/


void greenOn();

/** \ingroup leds

     Turn off the green LED. 
     \param NONE
     \return NONE
*/

void greenOff();

/** \ingroup leds

     Turn on the red LED. 
     \param NONE
     \return NONE
*/

void redOn();

/** \ingroup leds

     Turn off the red LED. 
     \param NONE
     \return NONE
*/

void redOff();

/** \ingroup leds

     Turn on the yellow LED. 
     \param NONE
     \return NONE
*/


void yellowOn();

/** \ingroup leds

     Turn off the yellow LED. 
     \param NONE
     \return NONE
*/

void yellowOff();




#endif
