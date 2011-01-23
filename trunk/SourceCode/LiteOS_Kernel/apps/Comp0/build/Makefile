###############################################################################
# Makefile for the project App
###############################################################################

## General Flags
PROJECT = App
MCU = atmega128
TARGET = App.elf

## Get OS type using uname
## Fedora core 7 : GNU/Linux 
## Cygwin: Cygwin

OSTYPE = $(shell uname -o)

## Check if this is linux machine
ifeq ($(OSTYPE), GNU/Linux)
	CC = avr-gcc
	AVRSIZE = avr-size
endif

## Check if this is cygwin machine
ifeq ($(OSTYPE), Cygwin)
	CC = C:/WinAVR/bin/avr-gcc.exe
	AVRSIZE = C:/WinAVR/bin/avr-size.exe
endif

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


LDFLAGS += -Wl,-section-start=.data=0x800B72
LDFLAGS += -Wl,-section-start=.text=0x17200
LDFLAGS += -Xlinker --defsym
LDFLAGS += -Xlinker __stack=0xc62

LIBRARIES = ../../../Libraries


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings




## Build
all: $(TARGET) App.hex App.eep App.lss size


$(TARGET):
	$(CC) -I.  -I$(LIBRARIES) $(CFLAGS) $(LDFLAGS) $(wildcard ../*.c) $(wildcard $(LIBRARIES)/*.c) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	$(AVRSIZE) --version
	$(AVRSIZE) -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) App.elf dep/* App.hex App.eep App.lss App.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

