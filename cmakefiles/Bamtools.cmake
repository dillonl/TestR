#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2015 Scientific Computing and Imaging Institute,
#  University of Utah.
# 
#  
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software. 
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.

# Borrowed CMake code from the SCI at the U of U
# found at https://github.com/SCIInstitute/SCIRun/blob/43bf24730c6f0f505268aea91267c8db2e87895e/Superbuild/BoostExternal.cmake

include(ExternalProject)

# Setting up external library bamtools, we don't build it because we only need the include directories
SET(BAMTOOLS_PROJECT bamtools_project CACHE INTERNAL "bamtools project name")
SET(BAMTOOLS_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals/bamtools CACHE INTERNAL "bamtools project directory")
ExternalProject_Add(${BAMTOOLS_PROJECT}
	GIT_REPOSITORY https://github.com/pezmaster31/bamtools.git
	GIT_TAG 02c2be8866f9e43b1b5f0f80e4daae5ff352c06b #lock in the commit id so we don't this doesn't break in the future
	INSTALL_COMMAND ""
	PREFIX ${BAMTOOLS_DIR}
)