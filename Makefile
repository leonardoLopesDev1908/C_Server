CC := gcc
CFLAGS := -Wall -Wextra -g
LDFLAGS := -lpthread

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SRCS:.c=.o)
TARGET := main

INC_DIRS := $(shell find $(SRC_DIR) include -type d)
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

CFLAGS += $(INC_FLAGS)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir $(dir $@) 
	$(CC) $(CFLAGS) -c $^ -o $@ 

clean:
	rm -f $(BUILD_DIR)

-include $(DEPS)
