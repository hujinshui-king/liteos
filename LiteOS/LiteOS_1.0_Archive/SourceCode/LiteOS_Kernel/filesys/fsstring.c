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
#include "fsstring.h"
#include "../types/types.h"
#include "../types/string.h"
char *extractString(char *start, char *buffer)
{
    char *p, *q;

    q = buffer;
    p = start;
    if (*p == '/')
    {
        p++;
    }
    while ((isLetter(*p)) || (*p == '_') || (*p == '.') || (isDigit(*p)))
    {
        *q++ = *p++;
    }

    *q = '\0';
    return p;
}

//-------------------------------------------------------------------------
char *extractLastName(char *pathname)
{
    char *p, *start;
    int length;

    length = mystrlen(pathname);
    p = pathname + (length - 1);
    start = pathname;
    while ((*p != '/') && (p != start))
    {
        p--;
    }

    if ((p == start) && ((*start) != '/'))
    {
        return p;
    }
    else
    {
        return ++p;
    }
}

//-------------------------------------------------------------------------
int fileMode(char *s)
{
    if (mystrlen(s) == 1)
    {
        if (s[0] == 'r')
        {
            return 1;
        }

        if (s[0] == 'w')
        {
            return 2;
        }

        if (s[0] == 'a')
        {
            return 3;
        }

        if (s[0] == 't')
        {
            return 4;
        }
    }
    else if (mystrlen(s) == 2)
    {
        return 5;
    }
    else
    {
        return 0;
    }
    return 0;
}
