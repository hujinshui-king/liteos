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
#include "vectornode.h"
#include "fsconfig.h"
#include "fsapi.h"
#include "../system/bytestorage.h"
#include "../system/nodeconfig.h"
#include "../platform/micaz/ioeeprom.h"
char vectornode[12];
void writeVectorNodeToExternalStorage()
{
    genericreadBytes(EEPROMVECTORSTART, 12, vectornode);
}

//-------------------------------------------------------------------------
void readVectorNodeFromExternalStorage()
{
    genericwriteBytes(EEPROMVECTORSTART, 12, vectornode);
}

//-------------------------------------------------------------------------
int getbit(char *set, int number)
{
    set += number / 8;
    return (*set & (1 << (number % 8))) != 0;   /* 0 or 1       */
}

//-------------------------------------------------------------------------
void setbit(char *set, int number, int value)
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
void initVectorNode()
{
    int i;

    for (i = 0; i < 12; i++)
    {
        vectornode[i] = 0;
    }
}

//-------------------------------------------------------------------------
int getVectorNode()
{
    int num;

    for (num = 0; num < 96; num++)
    {
        if (getbit(vectornode, num) == 0)
        {
            setbit(vectornode, num, 1);
            return num + 1;
        }
    }
    return -1;
}

//-------------------------------------------------------------------------
void scanVectorNode()
{
    int num, addr;
    uint8_t valid;

    for (num = 0; num < 96; num++)
    {
        addr = num + 1;
        valid = fsread8uint(addr, VALIDOFFSET);
        if (valid == 0)
        {
            setbit(vectornode, num, 0);
        }
        else
        {
            setbit(vectornode, num, 1);
        }
    }
}

//-------------------------------------------------------------------------
void releaseVectorNode(int num)
{
    setbit(vectornode, num - 1, 0);
}

//-------------------------------------------------------------------------
int countVectorNode()
{
    int num, count;

    count = 0;
    for (num = 0; num < 96; num++)
    {
        if (getbit(vectornode, num) == 1)
        {
            count++;
        }
    }
    return 96 - count;
}

