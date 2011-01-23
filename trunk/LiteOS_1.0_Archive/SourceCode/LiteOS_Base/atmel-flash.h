#ifndef _ATMEL_FLASH_H_
#define _ATMEL_FLASH_H_


#define ATMEL_FLASH_PORT PORTD
#define ATMEL_FLASH_DIRE DDRD

#define ATMEL_FLASH_SELECT PORTA
#define ATMEL_FLASH_SELECT_PIN 3

#define ATMEL_FLASH_CLK 5
#define ATMEL_FLASH_OUT 2
#define ATMEL_FLASH_IN 3

#define ATMEL_FLASH_PAGE_SIZE 264
#define ATMEL_FLASH_MAX_PAGES 2048
#define ATMEL_FLASH_SIZE 540672

#define ATMEL_FLASH_BUFFER_1 0x1
#define ATMEL_FLASH_BUFFER_2 0x2

#define ATMEL_FLASH_DEFAULT_BUFFER ATMEL_FLASH_BUFFER_1


/* opcodes for the device */
enum {
    C_READ_BUFFER1 = 0xd4,
    C_READ_BUFFER2 = 0xd6,
    C_WRITE_BUFFER1 = 0x84,
    C_WRITE_BUFFER2 = 0x87,
    C_FILL_BUFFER1 = 0x53, 
    C_FILL_BUFFER2 = 0x55, 
    C_FLUSH_BUFFER1 = 0x83,
    C_FLUSH_BUFFER2 = 0x86,
    C_QFLUSH_BUFFER1 = 0x88,
    C_QFLUSH_BUFFER2 = 0x89,
    C_COMPARE_BUFFER1 = 0x60,
    C_COMPARE_BUFFER2 = 0x61,
    C_WRITE_THROUGH_BUFFER1 = 0x82,
    C_WRITE_THROUGH_BUFFER2 = 0x85,
    C_READ_THROUGH_MEMORY = 0xE8,
    C_REQ_STATUS = 0xd7,
    C_ERASE_PAGE = 0x81
};


void atmel_flash_init();


uint8_t atmel_flash_compare (uint8_t *p, uint16_t count);


uint16_t atmel_flash_crc(uint32_t count);


/*static*/ void atmel_flash_low();
/*static*/ void atmel_flash_high();
/*static*/ uint8_t atmel_flash_send_byte(uint8_t spiOut);
/*static*/ uint8_t atmel_flash_get_byte();

/*static*/ void atmel_flash_read_memory(uint16_t page,
				     uint16_t offset, 
				     void *reqData, uint16_t len);

/*static*/ uint16_t atmel_flash_crc_memory(uint16_t page,
					uint16_t offset, 
					uint32_t len);

/*static*/ uint8_t atmel_flash_busy();

/*static*/ uint8_t atmel_flash_write_buffer(uint8_t selected, uint16_t offset, 
					 void *reqdata, uint16_t len);

/*static*/ uint8_t atmel_flash_flush_buffer(uint8_t selected, uint16_t page);

/*static*/ uint8_t atmel_flash_compare_buffer(uint8_t selected, uint16_t page);

/*static*/ uint8_t atmel_flash_fill_buffer(uint8_t selected, uint16_t page);

/*static*/ uint8_t atmel_flash_erase_page(uint16_t page);

void readFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);

void writeFlash(int pagenum, uint8_t offset, void *buffer,  int NumOfBytes);

void copyFlash(int sourcepage, int targetpage);

uint16_t dev_read_DEV_ATMEL_FLASH (void *p, uint16_t count);

uint16_t dev_write_DEV_ATMEL_FLASH (const void *p, uint16_t count);


#endif
