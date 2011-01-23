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
#include "adcsocket.h"
#include "adcdriver.h"
#include "../../system/threaddata.h"
#include "../../system/threads.h"

/** \brief the thread handle of the current thread */
void ADCLight()
{
    int temp;

    temp = get_light();
    current_thread->data.adcstate.adcreading = temp;
    return;
}

//-------------------------------------------------------------------------
void ADCTemp()
{
    int temp;

    temp = get_temp();
    current_thread->data.adcstate.adcreading = temp;
    return;
}

//-------------------------------------------------------------------------
void ADCMagX()
{
    int temp;

    temp = get_magx();
    current_thread->data.adcstate.adcreading = temp;
    return;
}

//-------------------------------------------------------------------------
void ADCMagY()
{
    int temp;

    temp = get_magy();
    current_thread->data.adcstate.adcreading = temp;
    return;
}

//-------------------------------------------------------------------------
void ADCAccX()
{
    int temp;

    temp = get_accx();
    current_thread->data.adcstate.adcreading = temp;
    return;
}

//-------------------------------------------------------------------------
void ADCAccY()
{
    int temp;

    temp = get_accy();
    current_thread->data.adcstate.adcreading = temp;
    return;
}
