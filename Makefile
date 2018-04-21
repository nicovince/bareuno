CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS =-g
CFLAGS+=-Os
CFLAGS+=-Wall
CFLAGS+=-fno-exceptions
CFLAGS+=-ffunction-sections
CFLAGS+=-fdata-sections
CFLAGS+=-mmcu=atmega328p
CFLAGS+=-DF_CPU=16000000L
CFLAGS+=-MMD
CFLAGS+=-DUSB_VID=null
CFLAGS+=-DUSB_PID=null
CFLAGS+=-DARDUINO=105
CFLAGS+=-D__PROG_TYPES_COMPAT__

LDFLAGS =-Os
LDFLAGS+=-Wl,--gc-sections
LDFLAGS+=-mmcu=atmega328p

INCDIR =./inc

CFLAGS+=$(addprefix -I, $(INCDIR))

BUILD_DIR=./build
SRCDIR=./src
vpath %.c $(SRCDIR)

SRC =main.c

OBJS=$(addprefix $(BUILD_DIR)/, $(subst .c,.o,$(SRC)))

TARGET_NAME=bareuno
TARGET=$(BUILD_DIR)/$(TARGET_NAME).hex

all: $(TARGET)
	
.PHONY: clean

clean:
	rm -Rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<
	
	
$(BUILD_DIR)/$(TARGET_NAME).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	
	
flash: $(TARGET)
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(TARGET)
print-%  : ; @echo $* = $($*)
