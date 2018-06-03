CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
NM=avr-nm
GENMSG=tools/genmsg/genmsg.py

ifdef VERBOSE
V=
else
V=@
endif

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
LDFLAGS+=-Wl,-Map,$(BUILD_DIR)/$(TARGET_NAME).map
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
SRC+=slip_payload.c
SRC+=crc.c
SRC+=gpio.c
SRC+=tim.c

OBJS=$(addprefix $(BUILD_DIR)/, $(subst .c,.o,$(SRC)))
DEPS=$(addprefix $(BUILD_DIR)/, $(subst .c,.d,$(SRC)))

TARGET_NAME=bareuno
TARGET=$(BUILD_DIR)/$(TARGET_NAME).elf

all: $(TARGET)

.PHONY: clean

clean:
	$(V)rm -Rf $(BUILD_DIR)

$(BUILD_DIR):
	$(V)mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.d : %.c inc/messages.h | $(BUILD_DIR)
	$(V)$(CC) -MM $(CFLAGS) -c $< |sed 's,\(.*\)\.o[ :]*,$(BUILD_DIR)/\1.o $(BUILD_DIR)/\1.d : ,g' > $@

-include $(DEPS)

inc/messages.h: src/messages.yaml $(GENMSG)
	$(V)$(GENMSG) $< --h-gen --h-dest $(shell dirname $@) --py-gen --py-dest $(shell dirname $(GENMSG))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(V)$(CC) $(CFLAGS) -o $@ -c $<


$(BUILD_DIR)/$(TARGET_NAME).elf: $(OBJS)
	@echo "Linking $@"
	$(V)$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(V)$(SIZE) $@

%.hex: %.elf
	$(V)$(OBJCOPY) -O ihex -R .eeprom $< $@


.PHONY: flash
flash: $(TARGET)
	@echo "Flash $<"
	$(V)avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(TARGET)
	
.PHONY: symbols_size
symbols_size: $(TARGET)
	$(V)$(NM) --print-size --size-sort --radix=d --synthetic $<
	
print-%  : ; @echo $* = $($*)

AVR_DIR=/usr/lib/avr
AVR_INC_DIR=$(AVR_DIR)/include
.PHONY: tags
tags:
	ctags -R * $(AVR_INC_DIR)/avr/iom328p.h \
		$(AVR_INC_DIR)/util \
		$(AVR_INC_DIR)/compat
