# ==============================
#          VARIABLES
# ==============================

# Project name
PROJECT := campo_de_batalha

# Build directory
BUILD_DIR := build

# CMake generator for Windows
CMAKE_GENERATOR_WIN := "MinGW Makefiles"

# CMake generator for Linux (usually not needed)
CMAKE_GENERATOR_LINUX :=

# CMake executable
CMAKE := cmake

# Make executable
MAKE_CMD := make

# ==============================
#           RULES
# ==============================

.PHONY: all build build-linux clean format

# Default target
all: build

# Build target for Windows
build-win:
	@echo "Starting build process on Windows..."
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -G $(CMAKE_GENERATOR_WIN) .. && $(MAKE_CMD) --always-make

# Build target for Linux
build-linux:
	@echo "Starting build process on Linux..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_GENERATOR_LINUX) .. && $(MAKE_CMD)

# Clean target
clean:
	@echo "Cleaning project..."
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(PROJECT) $(PROJECT).exe

# Format target for Windows
format:
	@echo "Formatting source files..."
	@for file in $(wildcard src/*.[ch]pp); do clang-format -i "$$file"; done

# Phony Targets
.PHONY: all build build-linux clean format