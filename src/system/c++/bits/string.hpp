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
