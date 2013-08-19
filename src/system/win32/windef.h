#pragma GCC system_header

#ifndef    page_system_win32_windef_hpp
#   define page_system_win32_windef_hpp

#	include_next <windef.h>

	// remove macro pollution
#	undef far
#	undef near

#endif
