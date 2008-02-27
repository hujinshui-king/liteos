/* LiteOS Version 0.3 */
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
 
#include "inode.h"
#include "../types/types.h"
#include "fsconfig.h"
#include "../types/string.h"


#include "vectornode.h"
#include "vectorflash.h"
#include "../system/socket.h"


#ifdef PLATFORM_AVR
 #include "../platform/micaz/ioeeprom.h"
 #include "../platform/micaz/atmelflash.h"
 #include "../platform/micaz/adcdriver.h"
 #include "../platform/micaz/leds.h"
#endif

extern uint8_t adc_parameter[ 6 ];
extern uint16_t adc_returnvalue;


int checkNodeValid( int nodeaddr ) {
   uint8_t validness;
   uint8_t nodetype;
   validness = fsread8uint( nodeaddr, VALIDOFFSET );
   nodetype = fsread8uint( nodeaddr, TYPEOFFSET );
   if ( validness == 0 ) {
      return 0;
   } else {
      return ( int )nodetype;
   } 
}

//-------------------------------------------------------------------------
void buildNewNode( int addr, char *name, uint8_t uid, uint8_t parent, uint8_t type ) {
   uint8_t namelength;
   namelength = mystrlen( name );
   if ( namelength < 12 ) {
      namelength ++;
   } 
   //consider several types of type
   if ( type == DIRNODE ) {
      fswriteBytes( addr, FILENAMEOFFSET, namelength, name );
      fswrite8uint( addr, TYPEOFFSET, type );
      fswrite8uint( addr, VALIDOFFSET, 1 );
      fsinitBytes( addr, DIR_ADDRSUBOFFSET, 14, 0 );
      fswrite8uint( addr, DIR_UIDOFFSET, uid );
      fswrite16uint( addr, DIR_SIZEOFFSET, 0 );
      fswrite8uint( addr, DIR_PARENTOFFSET, parent );
   } else if ( type == FILENODE ) {
      fswriteBytes( addr, FILENAMEOFFSET, namelength, name );
      fswrite8uint( addr, TYPEOFFSET, type );
      fswrite8uint( addr, VALIDOFFSET, 1 );
      fsinitBytes( addr, FILE_ADDRPAGEOFFSET, 14, 0 );
      fswrite8uint( addr, FILE_UIDOFFSET, uid );
      fswrite16uint( addr, FILE_SIZEOFFSET, 0 );
      fswrite8uint( addr, FILE_PARENTOFFSET, parent );
   }
    else if ( type == DEVNODE ) {
      fswriteBytes( addr, FILENAMEOFFSET, namelength, name );
      fswrite8uint( addr, TYPEOFFSET, type );
      fswrite8uint( addr, VALIDOFFSET, 1 );
      fsinitBytes( addr, DEV_PROGRAMENTRYOFFSET, 14, 0 );
      fswrite8uint( addr, DEV_UIDOFFSET, uid );
      fswrite16uint( addr, DEV_SIZEOFFSET, 0 );
      fswrite8uint( addr, DEV_PARENTOFFSET, parent );
   }
    else if ( type == APPNODE ) {
      fswriteBytes( addr, FILENAMEOFFSET, namelength, name );
      fswrite8uint( addr, TYPEOFFSET, type );
      fswrite8uint( addr, VALIDOFFSET, 1 );
      fsinitBytes( addr, APP_ENTRYOFFSET, 14, 0 );
      fswrite8uint( addr, APP_UIDOFFSET, uid );
      fswrite16uint( addr, APP_SIZEOFFSET, 0 );
      fswrite8uint( addr, APP_PARENTOFFSET, parent );
   }
   if (( addr == 0 ) && ( parent == 0 )){}
    else {
      int i;
      for ( i = 0; i < 10; i ++ ) {
         uint8_t subaddr;
         subaddr = fsread8uint( parent, DIR_ADDRSUBOFFSET + i );
         if ( subaddr == 0 ) {
            fswrite8uint( parent, DIR_ADDRSUBOFFSET + i, addr );
            break;
         }
      }
   }
   return ;
}

//-------------------------------------------------------------------------
void printNode( int addr ) {
   char filename[ 13 ];
   uint8_t uid, parent, type, valid;
   uint8_t subdirectory[ 10 ];
   int i;
   uint8_t flashdirectory[ 8 ];
   filename[ 12 ] = '\0';
   fsreadBytes( addr, FILENAMEOFFSET, 12, filename );
   type = fsread8uint( addr, TYPEOFFSET );
   valid = fsread8uint( addr, VALIDOFFSET );
   if (( valid > 0 ) && ( type == DIRNODE )) {
      for ( i = 0; i < 10; i ++ ) {
         subdirectory[ i ] = fsread8uint( addr, DIR_ADDRSUBOFFSET + i );
      } 
   }
   if (( valid > 0 ) && ( type == FILENODE )) {
      for ( i = 0; i < 8; i ++ ) {
         flashdirectory[ i ] = fsread8uint( addr, FILE_ADDRPAGEOFFSET + i );
      } 
   }
   uid = fsread8uint( addr, 28 );
   parent = fsread8uint( addr, 31 );
   //vs patch 
   //	  printf("****Node %d*************\n", addr);
   //	  printf("filename: %s\n", filename);
   //	  printf("type (dir, file, dev, app) %d\n", type);
   //	  printf("valid is %d\n", valid);
   //	  if (type == DIRNODE)
   //	  {for (i=0;i<10;i++)
   //	  printf("The %d sub directory is %d\n", i, subdirectory[i]);
   //	  }
   if ( type == FILENODE ) {
      //		  for (i=0;i<8;i++)
      //			  printf("The %d sub flash is %d\n", i, flashdirectory[i]);
   }
   //	  printf("The node parent is %d\n", parent);
   //	  printf("*************************\n");
   return ;
}

//-------------------------------------------------------------------------
void formatFS() {
   int i;
   for ( i = 0; i <= 96; i ++ ) {
      fsinitBytes( i, 0, 32, 0 );
   } 
   //  write8uint(i, VALIDOFFSET, 0); 
   initVectorFlash();
   initVectorNode();
}

//-------------------------------------------------------------------------

void formatFSLite() {
  
   initVectorFlash();
   initVectorNode();
}


//-------------------------------------------------------------------------
void deleteNode( int addr ) {
   uint8_t type;
   uint8_t parent;
   type = fsread8uint( addr, TYPEOFFSET );
   parent = fsread8uint( addr, 31 );
   if ( type == DIRNODE ) {
      uint8_t i;
      for ( i = 0; i < 10; i ++ ) {
         uint8_t subaddr;
         subaddr = fsread8uint( addr, DIR_ADDRSUBOFFSET + i );
         if ( subaddr > 0 ) {
            deleteNode( subaddr );
         }
      }
      fswrite8uint( addr, VALIDOFFSET, 0 );
      if (( addr == 0 ) && ( parent == 0 )){}
       else {
         uint8_t i;
         for ( i = 0; i < 10; i ++ ) {
            uint8_t subaddr;
            subaddr = fsread8uint( parent, DIR_ADDRSUBOFFSET + i );
            if ( subaddr == addr ) {
               fswrite8uint( parent, DIR_ADDRSUBOFFSET + i, 0 );
               break;
            }
         }
      }
   } else if ( type == FILENODE ) {
      char i;
      uint8_t readpage;
      fswrite8uint( addr, VALIDOFFSET, 0 );
      if (( addr == 0 ) && ( parent == 0 )){}
       else {
         for ( i = 0; i < 10; i ++ ) {
            uint8_t subaddr;
            subaddr = fsread8uint( parent, DIR_ADDRSUBOFFSET + i );
            if ( subaddr == addr ) {
               fswrite8uint( parent, DIR_ADDRSUBOFFSET + i, 0 );
               break;
            }
         }
      }
      /*   for (i=0;i<8;i++)
      {
      readpage = read8uint(addr, FILE_ADDRPAGEOFFSET+i);
      if (readpage==0)
      break;
      else
      releaseFlashPage(readpage);
      }
       */
      for ( i = 0; i < 8; i ++ ) {
         readpage = fsread8uint( addr, FILE_ADDRPAGEOFFSET + i );
         if ( readpage == 0 ) {
            break;
         } else {
            releaseFlashPage( readpage );
         } 
      }
       {
         uint8_t currentaddr;
         //	   uint8_t currentstep;
         currentaddr = addr;
         while ( 1 ) {
            currentaddr = fsread8uint( currentaddr, FILE_NEXTOFFSET );
            if ( currentaddr > 0 ) {
               releaseVectorNode( currentaddr );
               for ( i = 0; i < 8; i ++ ) {
                  readpage = fsread8uint( currentaddr, FILE_ADDRPAGEOFFSET + i );
                  if ( readpage == 0 ) {
                     break;
                  } else {
                     releaseFlashPage( readpage );
                  } 
               }
            } else {
               break;
            } 
         }
      }
   }
    else if ( type == DEVNODE ) {
      fswrite8uint( addr, VALIDOFFSET, 0 );
      if (( addr == 0 ) && ( parent == 0 )){}
       else {
         int i;
         for ( i = 0; i < 10; i ++ ) {
            uint8_t subaddr;
            subaddr = fsread8uint( parent, DIR_ADDRSUBOFFSET + i );
            if ( subaddr == addr ) {
               fswrite8uint( parent, DIR_ADDRSUBOFFSET + i, 0 );
               break;
            }
         }
      }
   }
    else if ( type == APPNODE ) {
      fswrite8uint( addr, VALIDOFFSET, 0 );
      if (( addr == 0 ) && ( parent == 0 )){}
       else {
         int i;
         for ( i = 0; i < 10; i ++ ) {
            uint8_t subaddr;
            subaddr = fsread8uint( parent, DIR_ADDRSUBOFFSET + i );
            if ( subaddr == addr ) {
              fswrite8uint( parent, DIR_ADDRSUBOFFSET + i, 0 );
               break;
            }
         }
      }
   }
   releaseVectorNode( addr );
}

//-------------------------------------------------------------------------
void buildRootNode() {
   buildNewNode( FSROOTNODE, "root", 0, 0, DIRNODE );
}

void SocketSendRadioThroughCommandandAdcTask()
{
  //dummy now. to fix. 

}

//-------------------------------------------------------------------------
void buildDeviceDirectory() {
   uint8_t newaddr;
   newaddr = getVectorNode();
   buildNewNode( newaddr, "dev", 0, FSROOTNODE, DIRNODE );
   //Now start building the device nodes 
   //platform dependent
   
   #ifdef PLATFORM_AVR
   {
   	uint8_t *adc_parameter;
   	uint8_t *adc_returnvalue; 
   	adc_parameter = getAdcParameterAddr();
   	adc_returnvalue = getAdcReturnValueAddr();
   buildDeviceNode( newaddr, "led", set_led_task, ( uint8_t* )adc_parameter, ( uint8_t* ) adc_returnvalue );
   buildDeviceNode( newaddr, "light", get_light_task, ( uint8_t* )adc_parameter, ( uint8_t* ) adc_returnvalue );
   buildDeviceNode( newaddr, "temp", get_temp_task, ( uint8_t* )adc_parameter, ( uint8_t* )adc_returnvalue );
   buildDeviceNode( newaddr, "magnet", get_mag_task_x, ( uint8_t* )adc_parameter, ( uint8_t* ) adc_returnvalue );
   buildDeviceNode( newaddr, "accel", get_acc_task_x, ( uint8_t* )adc_parameter, ( uint8_t* ) adc_returnvalue );
   buildDeviceNode( newaddr, "radio", SocketSendRadioThroughCommandandAdcTask, ( uint8_t* )adc_parameter, ( uint8_t* ) adc_returnvalue );
   }
   #endif
}

//-------------------------------------------------------------------------
void buildDeviceNode( uint8_t parentnode, char *name, void( *entryfunc )( void ), uint8_t *ramparameterstart, uint8_t *ramreturnstart ) {
   uint8_t newaddr;
   uint16_t temp;
   newaddr = getVectorNode();
   buildNewNode( newaddr, name, 0, parentnode, DEVNODE );
   temp = ( uint16_t )entryfunc;
   fswrite16uint( newaddr, DEV_PROGRAMENTRYOFFSET, temp );
   temp = ( uint16_t )ramparameterstart;
   fswrite16int( newaddr, DEV_PROGRAMENTRYOFFSET + 2, temp );
   temp = ( uint16_t )ramreturnstart;
   fswrite16uint( newaddr, DEV_PROGRAMENTRYOFFSET + 4, temp );
}

//-------------------------------------------------------------------------
void copyVectorNode( int from, int to ) {
   int i;
   for ( i = 0; i < 32; i ++ ) {
      uint8_t temp;
      temp = fsread8uint( from, i );
      fswrite8uint( to, i, temp );
   }
}

//-------------------------------------------------------------------------
void copyVectorPage( int from, int to ) {
   int i;
   int pagefrom, pageto;
   for ( i = 0; i < 8; i ++ ) {
      pagefrom = i + ( from - 1 ) *8;
      pageto = i + ( to - 1 ) *8;
      copyFlash( pagefrom, pageto );
   }
   return ;
}

//-------------------------------------------------------------------------
int inodeMatch( uint8_t addr, char *string ) {
   char temp[ 13 ];
   uint8_t length;
   uint8_t i;
   uint8_t valid;
   valid = fsread8uint( addr, VALIDOFFSET );
   if ( valid == 0 ) {
      return 0;
   } 
   fsreadBytes( addr, FILENAMEOFFSET, 12, temp );
   temp[ 12 ] = '\0';
   length = mystrlen( temp );
   for ( i = 0; i < length; i ++ ) {
      if ( superstring( temp + i, string ) == 0 ) {
         return 1;
      } 
   }
   return 0;
}
