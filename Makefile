# ==============================
#          VARIABLES
# ==============================

# Project name
PROJECT := campo_de_batalha.exe

# Build directory
BUILD_DIR := build

# CMake generator
CMAKE_GENERATOR := "MinGW Makefiles"

# CMake executable
CMAKE := cmake

# Make executable
MAKE_CMD := make

# ==============================
#           RULES
# ==============================

.PHONY: all build clean

# Default target
all: build

# Build target
build:
	@echo "Starting build process..."
	@mkdir $(BUILD_DIR) 2> NUL || echo "Build directory already exists."
	@cd $(BUILD_DIR) && $(CMAKE) -G $(CMAKE_GENERATOR) .. && $(MAKE_CMD) --always-make

# Clean target
clean:
	@echo "Cleaning project..."
	@rmdir /s /q $(BUILD_DIR) 2> NUL || echo "Build directory does not exist."
	@if exist $(PROJECT) del /f /q $(PROJECT)

# Format target for Windows
format:
	@echo "Formatting source files..."
	@for /r %%f in (src\*.cpp src\*.hpp) do clang-format -i "%%f"

# Phony Targets
.PHONY: all build clean format