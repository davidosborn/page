/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_system_cxx_bits_string_hpp
#   define page_system_cxx_bits_string_hpp

namespace std
{
	/*-----------------------------------------+
	| numeric conversions (ISO/IEC N3242 21.5) |
	+-----------------------------------------*/

#	ifdef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
	/**
	 * @defgroup numeric-conversions Numeric conversions
	 *
	 * This is a partial implementation of the numeric conversion functions
	 * from <string>, none of which are available in the Standard Library
	 * under MinGW GCC.
	 *
	 * @{
	 */
	int                stoi  (const string &);
	long               stol  (const string &);
	unsigned long      stoul (const string &);
	long long          stoll (const string &);
	unsigned long long stoull(const string &);
	float              stof  (const string &);
	double             stod  (const string &);
	long double        stold (const string &);

	string to_string(int);
	string to_string(unsigned);
	string to_string(long);
	string to_string(unsigned long);
	string to_string(long long);
	string to_string(unsigned long long);
	string to_string(float);
	string to_string(double);
	string to_string(long double);
	///@}
#	endif
}

#endif
