CC=gcc

BUILD_DIR=./build
SRCDIR=../src
INCDIR =../inc

CFLAGS =-g
CFLAGS =-std=c11
CFLAGS+=-Os
CFLAGS+=-Wall
CFLAGS+=$(addprefix -I, $(INCDIR))

LDFLAGS =-Os

vpath %.c $(SRCDIR)

SRC =slip.c
SRC+=slip_payload.c
SRC+=slip_test.c

OBJS=$(addprefix $(BUILD_DIR)/, $(subst .c,.o,$(SRC)))
DEPS=$(addprefix $(BUILD_DIR)/, $(subst .c,.d,$(SRC)))

TARGET_NAME=slip_test
TARGET=$(BUILD_DIR)/$(TARGET_NAME)

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -Rf $(BUILD_DIR)

$(BUILD_DIR)/%.d : %.c | $(BUILD_DIR)
	$(CC) -MM $(CFLAGS) -c $< |sed 's,\(.*\)\.o[ :]*,$(BUILD_DIR)/\1.o $(BUILD_DIR)/\1.d : ,g' > $@

-include $(DEPS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<


$(BUILD_DIR)/$(TARGET_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

print-%  : ; @echo $* = $($*)
