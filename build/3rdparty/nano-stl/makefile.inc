####################################################################################################
# \file makefile
# \brief  Makefile for nano-stl library
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


# Library directory
LIBRARY_DIR := $(ROOT_DIR)/3rdparty/nano-stl/src/libs/nano-stl

# Source directories
SOURCE_DIRS := $(LIBRARY_DIR) \
               $(LIBRARY_DIR)/containers \
               $(LIBRARY_DIR)/error \
               $(LIBRARY_DIR)/events \
               $(LIBRARY_DIR)/helpers \
               $(LIBRARY_DIR)/interfaces \
               $(LIBRARY_DIR)/libc \
               $(LIBRARY_DIR)/memory \
               $(LIBRARY_DIR)/strings \
              
# Project specific include directories
PROJECT_INC_DIRS := $(PROJECT_INC_DIRS) \
                    $(foreach LIB_DIR, $(SOURCE_DIRS), $(LIB_DIR)) \
                    $(NANO_STL_CONFIG_FILE_DIR)

