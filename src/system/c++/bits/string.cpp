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
