# Compiler and flags
CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O3
LDFLAGS := -L/usr/local/lib -lboost_system -lboost_thread-mt -lssl -lcrypto

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Source files and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Output binary name
TARGET := $(BIN_DIR)/ticker_app

# Default target
all: $(TARGET)

# Linking the binary from object files
$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compiling the source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean command to remove generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets to avoid conflicts with filenames
.PHONY: all clean
