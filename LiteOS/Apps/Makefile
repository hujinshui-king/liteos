###############################################################################
# Makefile for the project TestBlink
###############################################################################

## General Flags
PROJECT = TestBlink
MCU = atmega128
TARGET = TestBlink.elf
CC = c:/WinAVR/bin/avr-gcc.exe
AVRSIZE = c:/WinAVR/bin/avr-size.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -Os -g -fsigned-char
CFLAGS += -combine -fwhole-program 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
###LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=data.map


LDFLAGS += -Wl,-section-start=.data=0x800a00
LDFLAGS += -Wl,-section-start=.text=0x11000
LDFLAGS += -Xlinker --defsym
LDFLAGS += -Xlinker __stack=0xaff

LIBRARIES = ../../../Libraries


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings




## Build
all: $(TARGET) TestBlink.hex TestBlink.eep TestBlink.lss size


$(TARGET):
	$(CC) -I.  -I$(LIBRARIES) $(CFLAGS) $(LDFLAGS) $(wildcard ../*.c) $(wildcard $(LIBRARIES)/*.c) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	#@echo
	$(AVRSIZE) --version
	$(AVRSIZE) -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) TestBlink.elf dep/* TestBlink.hex TestBlink.eep TestBlink.lss TestBlink.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

