####################################################################################################
# \file makefile
# \brief  Makefile for the include files of simulator library
# \author C. Jimenez
# \copyright Copyright(c) 2017 Cedric Jimenez
#
# This file is part of Open-Vario.
#
# Open-Vario is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Open-Vario is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
####################################################################################################


# Locating the root directory
ROOT_DIR := ../../..

# Project name
PROJECT_NAME := simulator

# Build type
BUILD_TYPE := LIB

# Projects that need to be build before the project or containing necessary include paths
PROJECT_DEPENDENCIES = libs/cpu \
                       libs/peripherals \
					   libs/os \
                       $(TARGET_BSP)

			  
# Including common makefile definitions
include $(ROOT_DIR)/build/make/generic_makefile


# Rules for building the source files
$(BIN_DIR)/$(OUTPUT_NAME): protoc_gen $(BIN_DEPENDENCIES)
	@echo "Creating archive $(notdir $@)..."
	$(DISP)$(AR) $(ARFLAGS) $@ $(OBJECT_FILES)

	

# Directory which contain protocol buffer description files
PROTOC_DIR = $(ROOT_DIR_ABS)/3rdparty/open-vario-simu/src/api

# Ouput directory for generated files
PROTOC_OUT_DIR = $(ROOT_DIR_ABS)/src/libs/simulator/api

# Rule for generating protocol buffer source files
protoc_gen:
	$(PROTOC_COMP) --cpp_out=$(PROTOC_OUT_DIR) -I$(PROTOC_DIR) $(PROTOC_DIR)/requests.proto
	$(PROTOC_COMP) --cpp_out=$(PROTOC_OUT_DIR) -I$(PROTOC_DIR) $(PROTOC_DIR)/responses.proto
	$(PROTOC_COMP) --cpp_out=$(PROTOC_OUT_DIR) -I$(PROTOC_DIR) $(PROTOC_DIR)/notifications.proto
	mv $(PROTOC_OUT_DIR)/requests.pb.cc $(PROTOC_OUT_DIR)/requests.pb.cpp
	mv $(PROTOC_OUT_DIR)/responses.pb.cc $(PROTOC_OUT_DIR)/responses.pb.cpp
	mv $(PROTOC_OUT_DIR)/notifications.pb.cc $(PROTOC_OUT_DIR)/notifications.pb.cpp

