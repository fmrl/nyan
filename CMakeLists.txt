# This file is part of the *nyan* project at <http://fmrl.org>.
# Copyright (c) 2011, Michael Lowell Roberts.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  - Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#  - Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#
#  - Neither the name of the copyright holder nor the names of
#  contributors may be used to endorse or promote products derived
#  from this software without specific prior written permission.
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

cmake_minimum_required(VERSION 2.6)
project(nyan)

include(cmake/cache.cmake)

# toolchain configuration
# -----------------------
if(CMAKE_COMPILER_IS_GNUCC)
	# -Wmissing-braces and -Wmissing-field-initializers appears
	# to interfere with C89-defined behavior.
	# "-rdynamic" tells the linker to put all symbols into the symbol
	# table.
	set(DEFAULT_CFLAGS "-rdynamic -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers")
elseif(MSVC)
	# C4127 pops up every time a "do {} while (0)" is used.
	set(DEFAULT_CFLAGS "/W4 /wd4127")
else()
	message(WARNING "i don't know how to configure CMAKE_C_FLAGS for this compiler.")
endif()
set_cache_builtin(STRING CMAKE_C_FLAGS ${DEFAULT_CFLAGS})
set_cache_builtin(STRING CMAKE_CXX_FLAGS ${DEFAULT_CFLAGS})

find_package(Threads REQUIRED)

include(cmake/boost.cmake)
find_boost(1.42.0 REQUIRED COMPONENTS thread regex)

# include directory search path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/dep
	)
include_directories(${Boost_INCLUDE_DIRS})

add_library(nyan
	include/nyan/atomic_integer.hpp
	include/nyan/backtrace.hpp
	include/nyan/fail.hpp
	include/nyan/fail/empty_fail.hpp
	include/nyan/fail/fail_base.hpp
	include/nyan/fail/fail_meta.hpp
	include/nyan/fail/fail_record.hpp
	include/nyan/fail/not_found_fail.hpp
	include/nyan/fail/null_fail.hpp
	include/nyan/fail/zero_fail.hpp
	include/nyan/panic.hpp
	include/nyan/ptr.hpp
	include/nyan/ptr/const_maybe_ptr.hpp
	include/nyan/ptr/const_surely_ptr.hpp
	include/nyan/ptr/maybe_ptr.hpp
	include/nyan/ptr/ptr_base.hpp
	include/nyan/ptr/reference_counted_object.hpp
	include/nyan/ptr/surely_ptr.hpp
	include/nyan/source_coordinate.hpp
	src/lib/backtrace.cpp
	src/lib/fail/empty_fail.cpp
	src/lib/fail/fail_base.cpp
	src/lib/fail/fail_record.cpp
	src/lib/fail/not_found_fail.cpp
	src/lib/fail/null_fail.cpp
	src/lib/fail/zero_fail.cpp
	src/lib/panic.cpp
	src/lib/ptr/ptr_base.cpp
	src/lib/source_coordinate.cpp
	)

add_executable(test src/test/test.cpp)
target_link_libraries(test nyan ${Boost_LIBRARIES})

