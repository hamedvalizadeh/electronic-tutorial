#*************************
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
CFLAGS = -Wall -Os -Wextra -Werror -std=c99 -pedantic -Iusbdrv -Ilib -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
PORT_LX    = /dev/ttyACM0
PORT_MAC   = /dev/tty.usbmodemfa131
PROGRAMMER = -c USBasp 
# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/
FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# Source files
SRC = main.c $(wildcard lib/*.c)

# Build folder
BUILD_DIR  = build

# Object files will be inside build/
OBJECTS    = $(SRC:%.c=$(BUILD_DIR)/%.o)
 
# Tune the lines below only if you know what you are doing: 
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 4
# COMPILE = avr-gcc -Wall -Os -Iusbdrv -Ilib -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
COMPILE = avr-gcc $(CFLAGS)


all: $(BUILD_DIR)/main.hex  

# Ensure build folder exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) 

# Compile .c → .o into build/
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.
 
.c.s:
	$(COMPILE) -S $< -o $@
 
flash: clean all
	$(AVRDUDE) -U flash:w:$(BUILD_DIR)/main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)
 
# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	$(RM) -r $(BUILD_DIR) 

# Link object files into ELF
$(BUILD_DIR)/main.elf: $(OBJECTS)
	$(COMPILE) -o $@ $(OBJECTS)

# Create HEX file from ELF
$(BUILD_DIR)/main.hex: $(BUILD_DIR)/main.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
 
# Targets for code debugging and analysis:
disasm: $(BUILD_DIR)/main.elf
	avr-objdump -d $<

cpp:
	$(COMPILE) -E main.c