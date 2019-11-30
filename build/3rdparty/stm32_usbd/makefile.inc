####################################################################################################
# \file makefile
# \brief  Makefile for stm32_usbd library
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
LIBRARY_DIR := $(ROOT_DIR)/3rdparty/stm32_usbd

# Source directories
SOURCE_DIRS := $(LIBRARY_DIR) \
               $(LIBRARY_DIR)/STM32L4_HAL \
               $(LIBRARY_DIR)/Core \
               $(LIBRARY_DIR)/Core/Inc \
               $(LIBRARY_DIR)/Core/Src \
               $(LIBRARY_DIR)/Class \
               $(LIBRARY_DIR)/Class/CDC \
               $(LIBRARY_DIR)/Class/CDC/Inc \
               $(LIBRARY_DIR)/Class/CDC/Src
          
# Project specific include directories
PROJECT_INC_DIRS := $(PROJECT_INC_DIRS) \
                    $(foreach LIB_DIR, $(SOURCE_DIRS), $(LIB_DIR))

