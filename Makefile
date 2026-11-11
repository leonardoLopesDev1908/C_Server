SRC_DIR := src
BUILD_DIR := build

EXE := $(BUILD_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall
LDFLAGS  := -pthread

ifeq ($(OS),Windows_NT)
	RM_DIR = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
    RM_FILE = if exist $(EXE).exe del /f /q $(EXE).exe
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    TARGET_EXT := .exe
else
    RM_DIR = rm -rf $(BUILD_DIR)
    RM_FILE = rm -f $(TARGET)
    MKDIR = mkdir -p $(BUILD_DIR)
    TARGET_EXT :=
endif

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	$(MKDIR)

clean:
	$(RM_DIR)
	$(RM_FILE)

-include $(OBJ:.o=.d)
