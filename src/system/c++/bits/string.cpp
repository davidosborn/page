/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#include <string> // _GLIBCXX_HAVE_BROKEN_VSWPRINTF

#ifdef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
#	include <sstream> // [io]stringstream
#	include <stdexcept> // invalid_argument
#endif

namespace std
{
	/*====================+
	| numeric conversions |
	+====================*/

#ifdef _GLIBCXX_HAVE_BROKEN_VSWPRINTF

	/*-----+
	| sto* |
	+-----*/

	namespace
	{
		template <typename T>
			T stox(const string &s)
		{
			istringstream ss(s);
			T value;
			if (!(ss >> value))
				throw invalid_argument("conversion failed");
			return value;
		}
	}

	int stoi(const string &s)
	{
		return stox<int>(s);
	}

	long stol(const string &s)
	{
		return stox<long>(s);
	}

	unsigned long stoul(const string &s)
	{
		return stox<unsigned long>(s);
	}

	long long stoll(const string &s)
	{
		return stox<long long>(s);
	}

	unsigned long long stoull(const string &s)
	{
		return stox<unsigned long long>(s);
	}

	float stof(const string &s)
	{
		return stox<float>(s);
	}

	double stod(const string &s)
	{
		return stox<double>(s);
	}

	long double stold(const string &s)
	{
		return stox<long double>(s);
	}

	/*----------+
	| to_string |
	+----------*/

	namespace
	{
		template <typename T>
			string to_string_impl(T x)
		{
			ostringstream ss;
			ss << x;
			return ss.str();
		}
	}

	string to_string(int x)
	{
		return to_string_impl(x);
	}

	string to_string(unsigned x)
	{
		return to_string_impl(x);
	}

	string to_string(long x)
	{
		return to_string_impl(x);
	}

	string to_string(unsigned long x)
	{
		return to_string_impl(x);
	}

	string to_string(long long x)
	{
		return to_string_impl(x);
	}

	string to_string(unsigned long long x)
	{
		return to_string_impl(x);
	}

	string to_string(float x)
	{
		return to_string_impl(x);
	}

	string to_string(double x)
	{
		return to_string_impl(x);
	}

	string to_string(long double x)
	{
		return to_string_impl(x);
	}
#endif
}
