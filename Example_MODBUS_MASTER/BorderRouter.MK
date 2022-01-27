#*****************************************************************************
# * Copyright ©2019. Radiocrafts AS (Radiocrafts).  All Rights Reserved. 
# * Permission to use, copy, modify, and distribute this software and 
# * its documentation, without fee and without a signed licensing 
# * agreement, is hereby granted, provided that the above copyright 
# * notice, this paragraph and the following two paragraphs appear in 
# * all copies, modifications, and distributions.
# * 
# * IN NO EVENT SHALL RADIOCRFTS BE LIABLE TO ANY PARTY FOR DIRECT, 
# * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING 
# * LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
# * DOCUMENTATION, EVEN IF RADIOCRAFTS HAS BEEN ADVISED OF THE 
# * POSSIBILITY OF SUCH DAMAGE. 
# * 
# * RADIOCRAFTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT 
# * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
# * FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING 
# * DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". 
# * RADIOCRAFTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, 
# * UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
#****************************************************************************

# Default values. You can override them at the command line like for
# instance this if you are running on a windows computer:
# make uploadImage PORT=COM3

ifdef ComSpec
# Windows:
Framework_Path=..\..\Framework
else
# Linux:
Framework_Path=../../Framework
endif

#This makefile must be included on top. It defines compiler + linker setup
include $(Framework_Path)/Tools/Compile.mk

SOURCE_FILES := \
  SRC/App_TransparentUART_BR.c
  
OUTPUT_FILENAME=Output/BorderRouter


AKEY=Application.key
NKEY=Network.key


# The "all" rule includes what to be done. See Framework/Tools/RIIM.mk
# for additional rules (e.g. generateEncBin, uploadAppKey)
all: prerequisites buildAll generateBin uploadImage

# The buildAll rule compiles, links, generates hex-files and finally
# prints out the size of the application
buildAll:
   # Build and link
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCE_FILES) -o $(OUTPUT_FILENAME).elf
   # Generate HEX-file
	$(OC) -O ihex "$(OUTPUT_FILENAME).elf" "$(OUTPUT_FILENAME).hex"
   # Print application size
	$(SZ) "$(OUTPUT_FILENAME).elf"

prerequisites:
ifndef SOURCE_FILE
	@echo "MISSING SOURCE FILE(S). Set the SOURCE_FILE, e.g:"
	@echo "make SOURCE_FILE=SRC/YourAppFile.c"
	@echo
	@false
endif

# This makefile defines additional rules
include $(Framework_Path)/Tools/RIIM.mk