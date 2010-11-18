#include "io_eeprom.h" 
#include "fs_config.h"
#include "commondefines.h"

//this function reads a 8-bit uint from the inode-offset 

uint8_t  read8uint (int inode, int offset)
{
  	uint16_t addr;
  	
  	addr = (uint16_t)(inode*INODESIZE + offset);
  	
  	eeprom_busy_wait();
  	return eeprom_read_byte((uint8_t *)addr);
}


int8_t   read8int  (int inode, int offset)
{
	  int16_t addr;
	  
	  uint8_t value; 
	    	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	value = eeprom_read_byte((uint8_t *)addr);
	  
	  return *(int8_t*)&value; 
}

uint16_t read16uint(int inode, int offset)
{
	  int16_t addr;
	    	
  	addr = inode*INODESIZE + offset;
  	eeprom_busy_wait();
  	return eeprom_read_word((uint16_t *)addr);
	  	
	
}


int16_t  read16int (int inode, int offset)
{
	
	  int16_t addr;
	  
	  uint16_t value; 
	  	    	
  	addr = inode*INODESIZE + offset;
  	eeprom_busy_wait();
  	value = eeprom_read_word((uint16_t *)addr);
  	
  	return *(int16_t*)&value; 
		
}



void  write8uint (int inode, int offset, uint8_t value)
{
	
	  uint16_t addr;
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	eeprom_write_byte((uint8_t *)addr, value);
  	
  	return;  		
}

void write8int  (int inode, int offset, int8_t value)
{
	
	  uint16_t addr;
  	
  	uint8_t value2;
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	value2 = *(uint8_t *)&value;
  	
  	eeprom_write_byte((uint8_t *)addr, value2);
  	
  	return;  
	
}


void write16uint(int inode, int offset, uint16_t value)
{
	  uint16_t addr;
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	eeprom_write_word((uint16_t *)addr, value);
  	
  	return;  			
}


void  write16int (int inode, int offset, int16_t value)
{
	 
	  uint16_t addr;
  	
  	uint16_t value2;
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	value2 = *(uint16_t *)&value;
  	
  	eeprom_write_word((uint16_t *)addr, value2);
  	
  	return;  
	
	
	
}



uint32_t read32uint(int inode, int offset)
{
	  uint16_t addr;
  	
  	uint16_t value1;
  	
  	uint16_t value2;
  	
  	addr = inode*INODESIZE + offset;
  	
	  eeprom_busy_wait();
  	
  	value1 = eeprom_read_word((uint16_t *)addr);
  	
  	eeprom_busy_wait();
  	
  	value2 = eeprom_read_word((uint16_t *)(addr+2));
  	
  	return (((uint32_t)value2)<<16)+(uint32_t)value1;  
	
	
}


int32_t  read32int (int inode, int offset)
{
    uint16_t addr;
  	
  	uint16_t value1;
  	
  	uint16_t value2;

	uint32_t returnvalue; 
  	
  	addr = inode*INODESIZE + offset;
  	
	  eeprom_busy_wait();
  	
  	value1 = eeprom_read_word((uint16_t *)addr);
  	
  	eeprom_busy_wait();
  	
  	value2 = eeprom_read_word((uint16_t *)(addr+2));
  	
  	returnvalue = ((((uint32_t)value2)<<16)+(uint32_t)value1);  	
	
	return *(int32_t *)&returnvalue; 
}


void  write32uint(int inode, int offset, uint32_t value)
{
   
    uint16_t addr;
  	
  	uint16_t value1;
  	
  	uint16_t value2;
  	
  	value1 = (uint16_t)(value >> 16);
  	
  	value2 = (uint16_t)(value & 0x0000ffff);
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	//value2 = *(uint16_t *)&value;
  	
  	eeprom_write_word((uint16_t *)addr, value2);
  	
  	
  	addr = inode*INODESIZE + offset +2 ;
  	
  	eeprom_busy_wait();
  	
  	//value2 = *(uint16_t *)&value;
  	
  	eeprom_write_word((uint16_t *)addr, value1);
  	
  	
  	return;    	
	
	
}

void  write32int (int inode, int offset, int32_t value)
{
    
    uint16_t addr;
  	
  	uint16_t value1;
  	
  	uint32_t value2;
  	
  	value1 = (uint16_t)((*(uint32_t *)&value) >> 16);
  	
  	value2 = (uint16_t)((*(uint32_t *)&value) & 0x0000ffff);
  	
  	addr = inode*INODESIZE + offset;
  	
  	eeprom_busy_wait();
  	
  	//value2 = *(uint16_t *)&value;
  	
  	eeprom_write_word((uint16_t *)addr, value2);
  	
  	
  	addr = inode*INODESIZE + offset +2 ;
  	
  	eeprom_busy_wait();
  	
  	//value2 = *(uint16_t *)&value;
  	
  	eeprom_write_word((uint16_t *)addr, value1);
   	
  	return;    		

}



void readBytes (int inode, int offset, int nBytes, void *buffer)
{
	
	  uint16_t addr;
  	
  	addr = inode*INODESIZE + offset;
	
	  eeprom_busy_wait();
	
	  eeprom_read_block(buffer, (void *)addr, nBytes);
}


void writeBytes(int inode, int offset, int nBytes, void *buffer)
{
	
	  uint16_t addr;
  	
  	addr = inode*INODESIZE + offset;
	
	  eeprom_busy_wait();
	
	  eeprom_write_block(buffer, (void *)addr, nBytes);
	
	
}


//this function initializes all the values as reqeuested 

void initBytes(int inode, int offset, int nBytes, uint8_t value)
{
  	int i;
  	for (i=0;i<nBytes;i++)
   	 write8uint (inode, offset+i, value);	
}
