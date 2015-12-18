# =================================
# rufus
#
# tools/CMakeLists.txt
# =================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

# set header and source files
set(RUFUS_TOOLS_SOURCES
	rufus.cpp
)

#ADD_DEFINITIONS(-DBOOST_FALLTHROUGH)
INCLUDE_DIRECTORIES(${SCI_BOOST_INCLUDE})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#set(CMAKE_BUILD_TYPE Debug) # uncomment this when you need a debug build

#create binary
add_executable(rufus
  ${RUFUS_TOOLS_SOURCES}
)

add_dependencies(rufus ${BAMTOOLS_PROJECT} ${JELLYFISH_PROJECT} ${BWA_PROJECT} Boost_external)

# link libraries to executables
target_link_libraries(rufus
    ${BAMTOOLS_LIB}
   	${BAMTOOLS_UTIL_LIB}
   	${BWA_LIB}
	${SCI_BOOST_LIBRARY}
	${ZLIB_LIB}
	#${FASTAHACK_LIB}
	#${CORE_LIB}
)
