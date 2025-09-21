# Compiler and flags
CXX := g++
CXXFLAGS := -fPIC -Wall -Wextra -O2 -std=c++17
LDFLAGS := -shared

# Directories
SRC_DIR := src
BUILD_DIR := build

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Targets
TARGET_SO := $(BUILD_DIR)/liblog.so
TARGET_A  := $(BUILD_DIR)/liblog.a

# Default target builds both
all: $(TARGET_SO) $(TARGET_A)

# Shared library
$(TARGET_SO): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Static library
$(TARGET_A): $(OBJS)
	ar rcs $@ $^

# Compile .cpp to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
