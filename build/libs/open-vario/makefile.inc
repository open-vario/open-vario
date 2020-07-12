####################################################################################################
# \file makefile.inc
# \brief  Makefile for the include files of open-vario library
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
LIBRARY_DIR := $(ROOT_DIR)/src/libs/open-vario

# Source directories
SOURCE_DIRS := $(LIBRARY_DIR) \
               $(LIBRARY_DIR)/alarms \
               $(LIBRARY_DIR)/application \
               $(LIBRARY_DIR)/application/$(TARGET_OS) \
               $(LIBRARY_DIR)/blackbox \
               $(LIBRARY_DIR)/ble \
               $(LIBRARY_DIR)/config \
               $(LIBRARY_DIR)/device \
               $(LIBRARY_DIR)/diagnostic \
               $(LIBRARY_DIR)/fault \
               $(LIBRARY_DIR)/hmi \
               $(LIBRARY_DIR)/log \
               $(LIBRARY_DIR)/modes \
               $(LIBRARY_DIR)/navigation \
               $(LIBRARY_DIR)/profile \
               $(LIBRARY_DIR)/sensors \
               $(LIBRARY_DIR)/storage \
               $(LIBRARY_DIR)/time
              
# Project specific include directories
PROJECT_INC_DIRS := $(PROJECT_INC_DIRS) \
                    $(foreach LIB_DIR, $(SOURCE_DIRS), $(LIB_DIR))

