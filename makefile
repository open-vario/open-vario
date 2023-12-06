######################################################
#                  Helper makefile                   #
######################################################

# Default target
default: all

# Default platform
TARGET_PLATFORM?=stm32wb5mm-dk

# Root directory containing the top level CMakeLists.txt file
ROOT_DIR:=$(PWD)

# Build directory
BUILD_DIR:=$(ROOT_DIR)/build_$(TARGET_PLATFORM)
TARGET_TOOLCHAIN:=$(ROOT_DIR)/cmake/targets/CMakeLists_$(TARGET_PLATFORM).txt

# Generated binary directory
BIN_DIR:=$(ROOT_DIR)/bin_$(TARGET_PLATFORM)

# Make options
#VERBOSE="VERBOSE=1"
PARALLEL_BUILD?=-j 4

# Build type can be either Debug or Release
BUILD_TYPE?=Debug

# Disable build with docker when set to 1
DISABLE_DOCKER?=0
ifeq ($(DISABLE_DOCKER),0)
include makefile.docker
endif

# Silent makefile
.SILENT:

# Format code
format:
	@echo "Formatting source code..."
	@find ./src/firmware -name '*.h' -or -name '*.cpp' | xargs clang-format -i
	@echo "Formatting done!"

# Build/clean all targets
all: build
clean:
	@-rm -rf $(BIN_DIR)
	@-rm -rf $(BUILD_DIR)
	@echo "$(TARGET_PLATFORM) build cleaned!"

# Build targets
build: $(BUILD_DIR)/Makefile
	@echo "Starting build for $(TARGET_PLATFORM)..."
	@mkdir -p $(BIN_DIR)
	@@$(DOCKER_RUN) make --silent -C $(BUILD_DIR) $(VERBOSE) $(PARALLEL_BUILD)
	@echo "$(TARGET_PLATFORM) build done!"

$(BUILD_DIR)/Makefile:
	@echo "Generating $(TARGET_PLATFORM) makefiles..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@@$(DOCKER_RUN) eval "cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMAKE_TOOLCHAIN_FILE=$(TARGET_TOOLCHAIN) -D BIN_DIR=$(BIN_DIR) -D TARGET_PLATFORM=$(TARGET_PLATFORM) $(ROOT_DIR)"
