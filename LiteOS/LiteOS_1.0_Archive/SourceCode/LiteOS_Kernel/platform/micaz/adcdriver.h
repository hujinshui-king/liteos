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

/** \file adcdriver.h
*   This file implements the ADC module that correponds to reading from sensors.
*   This file implements ADC, and has the following interface to the remaining of the system:
the APIs for reading sensors, and the variables adc_returnvalue and adc_parameters for setting up adc tasks.
*/
#ifndef ADCDRIVERH
#define ADCDRIVERH
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../types/types.h"

/** \defgroup device ADC driver implementations
The functions in this module perform various tasks for handling the adc sensors.  By calling these functions, the user applications can sample the ADC, including 
light, magnetic, etc, and get readings. 
*/

/** @{*/

/** \name Channels 
Defines the channels used by the MicaZ implemenation. 
*/
enum
{
    LIGHT_CHANNEL = 1, TEMP_CHANNEL = 1, ACCX_CHANNEL = 3, ACCY_CHANNEL = 4,
    MAGX_CHANNEL = 5, MAGY_CHANNEL = 6
};

/** \name ADC Operations 
These functions perform various tasks on ADC. To use them, make sure first call adcdriver_init_adc() to initilize the adc, then, use the correponding 
function APIs. 
\note Those functions ending with task, e.g. get_light_task() return the reading from ADC in the global variable adc_returnvalue. 
*/

/** \ingroup device 
Initilizes the ADC module 
*/
void adcdriver_init_adc();

/** \ingroup device 
Get ADC reading using a particular channel. 
\param channel The channel number as an integer input
\return the reading of the adc
*/
uint16_t get_adc(int channel);

/** \ingroup device 
Get light sensor reading 
\param NONE
\return the reading of the light sensor 
*/
uint16_t get_light();

/** \ingroup device 
Get temperature sensor reading 
\param NONE
\return the reading of the temperature sensor 
*/
uint16_t get_temp();

/** \ingroup device 
Get magnetic sensor reading in X axis 
\param NONE
\return the reading of the magnetic  sensor 
*/
uint16_t get_magx();

/** \ingroup device 
Get magnetic sensor reading in Y axis 
\param NONE
\return the reading of the magnetic  sensor 
*/
uint16_t get_magy();

/** \ingroup device 
Get accelerator sensor reading in X axis 
\param NONE
\return the reading of the accelerator sensor 
*/
uint16_t get_accx();

/** \ingroup device 
Get accelerator sensor reading in Y axis 
\param NONE
\return the reading of the accelerator sensor 
*/
uint16_t get_accy();

/** \name ADC Operation Tasks 
These functions perform various tasks on ADC using task model, instead of returning such values, put such values in variables. Used by the OS to respond to user side tasks 
*/

/** \ingroup device 
Get light  sensor reading as a task, put the return reading into a global variable 
\param NONE
\return NONE
*/
void get_light_task();

/** \ingroup device 
Get temp  sensor reading as a task, put the return reading into a global variable 
\param NONE
\return NONE
*/
void get_temp_task();

/** \ingroup device 
Get acc sensor reading as a task, put the return reading into a global variable 
\param NONE
\return NONE
*/
void get_acc_task_x();
void get_acc_task_y();

/** \ingroup device 
Get mag  sensor reading as a task, put the return reading into a global variable 
\param NONE
\return NONE
*/
void get_mag_task_x();
void get_mag_task_y();
void *getAdcParameterAddr();
void *getAdcReturnValueAddr();

/** @}*/
#endif
