# $legal:1594:
# 
# Copyright (c) 2011, Michael Lowell Roberts.  
# All rights reserved. 
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are 
# met: 
# 
#   - Redistributions of source code must retain the above copyright 
#   notice, this list of conditions and the following disclaimer. 
# 
#   - Redistributions in binary form must reproduce the above copyright 
#   notice, this list of conditions and the following disclaimer in the 
#   documentation and/or other materials provided with the distribution.
#  
#   - Neither the name of the copyright holder nor the names of 
#   contributors may be used to endorse or promote products derived 
#   from this software without specific prior written permission. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
# 
# ,$

cmake_minimum_required(VERSION 2.6)
project(nyan)

include(cmakes/cache.cmake)

# toolchain configuration
# -----------------------
# on linux, the intel compiler executable is called *icc*.
if(${CMAKE_C_COMPILER} MATCHES icc)
	set(COMPILER_IS_ICC YES)
endif()
if(CMAKE_COMPILER_IS_GNUCC)
	# -Wmissing-braces and -Wmissing-field-initializers appears
	# to interfere with C89-defined behavior.
	# "-rdynamic" tells the linker to put all symbols into the symbol
	# table.
	set(DEFAULT_CFLAGS "-rdynamic -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers")
elseif(COMPILER_IS_ICC)
	# nothing to do (yet).
elseif(MSVC)
	# C4127 pops up every time a "do {} while (0)" is used.
	set(DEFAULT_CFLAGS "/W4 /wd4127")
else()
	message(WARNING "i don't know how to configure CMAKE_C_FLAGS for this compiler.")
endif()
set_cache_builtin(STRING CMAKE_C_FLAGS ${DEFAULT_CFLAGS})
set_cache_builtin(STRING CMAKE_CXX_FLAGS ${DEFAULT_CFLAGS})

find_package(Threads REQUIRED)

include(cmakes/boost.cmake)
find_boost(1.48.0 REQUIRED COMPONENTS thread regex random)

include(cmakes/yaml-cpp.cmake)

include(cmakes/config.cmake)
generate_config_header(nyan)

# include directory search path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/dep
	)
# [mlr][todo] this should be factored.
install(
	DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/include/nyan
		${CMAKE_CURRENT_BINARY_DIR}/include/nyan
	DESTINATION include)
include_directories(${Boost_INCLUDE_DIRS})

add_library(nyan
	include/nyan/atomic_integer.hpp
	include/nyan/atomic_integer.hpp
	include/nyan/convert.hpp
	include/nyan/convert/convert_number.hpp
	include/nyan/debug.hpp
	include/nyan/debug/backtrace.hpp
	include/nyan/debug/demangle.hpp
	include/nyan/fail.hpp
	include/nyan/fail/empty_fail.hpp
    include/nyan/fail/errno_fail.hpp
	include/nyan/fail/fail_base.hpp
	include/nyan/fail/fail_meta.hpp
	include/nyan/fail/fail_policy.hpp
	include/nyan/fail/fail_record.hpp
	include/nyan/fail/not_found_fail.hpp
	include/nyan/fail/null_fail.hpp
	include/nyan/fail/zero_fail.hpp
	include/nyan/functions/destroy.hpp
	include/nyan/functions/nop.hpp
	include/nyan/panic.hpp
	include/nyan/ptr.hpp
	include/nyan/ptr/const_trusted_smart_ptr.hpp
	include/nyan/ptr/const_untrusted_smart_ptr.hpp
	include/nyan/ptr/ptr_base.hpp
	include/nyan/ptr/reference_counted_object.hpp
	include/nyan/ptr/trusted_smart_ptr.hpp
	include/nyan/ptr/untrusted_smart_ptr.hpp
	include/nyan/text_coordinate.hpp
	include/nyan/yaml.hpp
	src/lib/debug/backtrace.cpp
	src/lib/debug/demangle.cpp
	src/lib/fail/empty_fail.cpp
    src/lib/fail/errno_fail.cpp
	src/lib/fail/fail_base.cpp
	src/lib/fail/fail_policy.cpp
	src/lib/fail/fail_record.cpp
	src/lib/fail/not_found_fail.cpp
	src/lib/fail/null_fail.cpp
	src/lib/fail/zero_fail.cpp
	src/lib/panic.cpp
	src/lib/ptr/ptr_base.cpp
	src/lib/text_coordinate.cpp
	)

add_executable(test src/test/test.cpp)
target_link_libraries(test nyan ${Boost_LIBRARIES} ${YAMLCPP_LIBRARY})

# TODO: i'd like to cannonize this code.
if(WIN32)
	detect_multi_configuration_build(IS_MULTI_CONFIG_BUILD)
	if(IS_MULTI_CONFIG_BUILD)
		foreach(i ${CMAKE_CONFIGURATION_TYPES})
			install(TARGETS nyan DESTINATION lib/${i} CONFIGURATIONS ${i})
		endforeach()
	else()
		install(TARGETS nyan DESTINATION lib/${CMAKE_BUILD_TYPE})
	endif()
else()
	install(TARGETS nyan DESTINATION lib)
endif()

# $vim:23: vim:set sts=3 sw=3 et:,$
