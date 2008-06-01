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
#include "vectorflash.h"
#include "../system/bytestorage.h"
#include "fsconfig.h"
#include "../system/nodeconfig.h"
#ifdef PLATFROM_AVR
#include "../platform/micaz/ioeeprom.h"
#endif
static char vectorflash[32];

void writeVectorFlashToExternalStorage()
{
#ifdef PLATFORM_AVR
    genericwriteBytes(FLASHVECTORSTART, 32, vectorflash);
#endif
}

//-------------------------------------------------------------------------
void readVectorFlashFromExternalStorage()
{
#ifdef PLATFORM_AVR
    genericreadBytes(FLASHVECTORSTART, 32, vectorflash);
#endif
}

//-------------------------------------------------------------------------
static int getbit(char *set, int number)
{
    set += number / 8;
    return (*set & (1 << (number % 8))) != 0;   /* 0 or 1       */
}

//-------------------------------------------------------------------------
static void setbit(char *set, int number, int value)
{
    set += number / 8;
    if (value)
    {
        *set |= 1 << (number % 8);
    }                           /* set bit      */
    else
    {
        *set &= ~(1 << (number % 8));
    }                           /* clear bit    */
}

//-------------------------------------------------------------------------
void initVectorFlash()
{
    int i;

    for (i = 0; i < 32; i++)
    {
        vectorflash[i] = 0;
    }
}

//-------------------------------------------------------------------------
int getFlashPage()
{
    int num;

    for (num = 0; num < 256; num++)
    {
        if (getbit(vectorflash, num) == 0)
        {
            setbit(vectorflash, num, 1);
            return num + 1;
        }
    }
    return -1;
}

//-------------------------------------------------------------------------
void scanVectorFlash()
{
    int num, addr;
    uint8_t valid;
    uint8_t type;
    uint8_t readpage;

    for (num = 0; num < 96; num++)
    {
        addr = num + 1;
        valid = fsread8uint(addr, VALIDOFFSET);
        type = fsread8uint(addr, TYPEOFFSET);
        if ((valid == 1) && (type == FILENODE))
        {
            char i;

            for (i = 0; i < 8; i++)
            {
                readpage = fsread8uint(addr, FILE_ADDRPAGEOFFSET + i);
                if (readpage > 0)
                {
                    setbit(vectorflash, readpage - 1, 1);
                }
            }
        }
    }
}

//-------------------------------------------------------------------------
void releaseFlashPage(int num)
{
    setbit(vectorflash, num - 1, 0);
}

//-------------------------------------------------------------------------
int countVectorFlash()
{
    int count, num;

    count = 0;
    for (num = 0; num < 256; num++)
    {
        if (getbit(vectorflash, num) == 1)
        {
            count++;
        }
    }
    return 256 - count;
}

//-------------------------------------------------------------------------
void printVectorFlash()
{
    int data[256];
    int i;

    for (i = 0; i < 256; i++)
    {
        if (getbit(vectorflash, i) == 0)
        {
            data[i] = 0;
        }
        else
        {
            data[i] = 1;
        }
    }
}
