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

# Vcpkg variables
VCPKG_ROOT := C:/Users/Arthur/vcpkg
VCPKG_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake
VCPKG_TRIPLET := x64-windows
VCPKG := $(VCPKG_ROOT)/vcpkg.exe

# ==============================
#           RULES
# ==============================

.PHONY: all build build-linux clean format

# Default target
all: build

.PHONY: vcpkg-install
vcpkg-install:
	@echo "Installing vcpkg dependencies..."
	@$(VCPKG) install $(VCPKG_PACKAGES) --triplet $(VCPKG_TRIPLET)

# Build target for Windows
build-win: vcpkg-install
	@echo "Starting build process on Windows..."
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@cd "$(BUILD_DIR)" && \
	$(CMAKE) -G $(CMAKE_GENERATOR_WIN) \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN) \
		-DVCPKG_TARGET_TRIPLET=$(VCPKG_TRIPLET) \
		-DCMAKE_BUILD_TYPE=Release \
		.. && \
	$(MAKE_CMD) --always-make
	@echo "Copying DLLs to the project root directory..."
	@for %%f in ($(DLLS)) do (
		copy "$(VCPKG_ROOT)/installed/$(VCPKG_TRIPLET)/bin/%%f" "."
	)
	@echo "Build and copy completed successfully."

# Vcpkg package installation

# Build target for Linux
build-linux:
	@echo "Starting build process on Linux..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_GENERATOR_LINUX) .. && $(MAKE_CMD)
	@mv $(BUILD_DIR)/$(PROJECT) .

build-linux-release:
	@echo "Starting release build process on Linux..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_GENERATOR_LINUX) .. && $(MAKE_CMD)
	@rm -rf release
	@mkdir release
	@cp $(BUILD_DIR)/$(PROJECT) release/
	@cp -r resources release/

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