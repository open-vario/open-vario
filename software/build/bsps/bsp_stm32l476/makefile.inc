####################################################################################################
# \file makefile.inc
# \brief  Makefile for the include files of bsp_stm32l476 BSP
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
BSP_DIR := $(ROOT_DIR)/src/bsps
LIBRARY_DIR := $(BSP_DIR)/bsp_stm32l476

# Source directories
SOURCE_DIRS := $(LIBRARY_DIR) \
               $(LIBRARY_DIR)/$(COMPILER_NAME) \
               $(LIBRARY_DIR)/$(TARGET_OS)
              
# Project specific include directories
PROJECT_INC_DIRS := $(PROJECT_INC_DIRS) \
                    $(BSP_DIR) \
                    $(foreach LIB_DIR, $(SOURCE_DIRS), $(LIB_DIR))

