# This makefile is for an ATmega328P-PU at 16 MHz, USBasp programmer
# 
# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. I am using Arduino UNO as ISP and for this the
#                programmer is avrisp
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.
 
DEVICE     = atmega328p
CLOCK      = 16000000
PORT_LX    = /dev/ttyACM0
PORT_MAC   = /dev/tty.usbmodemfa131
PROGRAMMER = -c USBasp 
OBJECTS    = main.o spi.o conversion.o i2c.o max7219.o ds32323.o
FUSES      = -U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0x05:m

LOCAL_SOURCE = 

#
# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/
 
# Tune the lines below only if you know what you are doing:
 
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 4
COMPILE = avr-gcc -Wall -Os -Iusbdrv -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
 
# symbolic targets:
all:    main.hex
 
.c.o:
	$(COMPILE) -c $< -o $@
 
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.
 
.c.s:
	$(COMPILE) -S $< -o $@
 
flash:  all
	$(AVRDUDE) -U flash:w:main.hex:i
 
fuse:
	$(AVRDUDE) $(FUSES)
 
# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse
 
# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex
 
clean:
	rm -f main.hex main.elf $(OBJECTS) *~
 
# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)
 
main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
#   avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.
 
# Targets for code debugging and analysis:
disasm: main.elf
	avr-objdump -d main.elf
 
cpp:
	$(COMPILE) -E main.c