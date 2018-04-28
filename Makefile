CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
NM=avr-nm

CFLAGS =-g
CFLAGS =-std=c11
CFLAGS+=-Os
CFLAGS+=-Wall
CFLAGS+=-fno-exceptions
CFLAGS+=-ffunction-sections
CFLAGS+=-fdata-sections
CFLAGS+=-mmcu=atmega328p
CFLAGS+=-DF_CPU=16000000L
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
SRC+=usart.c
SRC+=fifo.c
SRC+=slip.c

OBJS=$(addprefix $(BUILD_DIR)/, $(subst .c,.o,$(SRC)))
DEPS=$(addprefix $(BUILD_DIR)/, $(subst .c,.d,$(SRC)))

TARGET_NAME=bareuno
TARGET=$(BUILD_DIR)/$(TARGET_NAME).elf

all: $(TARGET)

.PHONY: clean

clean:
	rm -Rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.d : %.c | $(BUILD_DIR)
	$(CC) -MM $(CFLAGS) -c $< |sed 's,\(.*\)\.o[ :]*,$(BUILD_DIR)/\1.o $(BUILD_DIR)/\1.d : ,g' > $@

-include $(DEPS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<


$(BUILD_DIR)/$(TARGET_NAME).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@


.PHONY: flash
flash: $(TARGET)
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(TARGET)
	
.PHONY: symbols_size
symbols_size: $(TARGET)
	$(NM) --print-size --size-sort --radix=d --synthetic $<
	
print-%  : ; @echo $* = $($*)
