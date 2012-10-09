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

#include <algorithm> // copy, max
#include <array>
#include <cctype> // tolower, toupper
#include <climits> // CHAR_BIT
#include <cmath> // copysign, signbit
#include <cstdint> // uintptr_t
#include <cstdlib> // abs
#include <locale> // locale, num_get, num_put
#include "../math/float.hpp" // {,Is}{Inf,Nan}
#include "ios.hpp" // IosBaseFmtSaver
#include "sstream.hpp" // make_istringstream

namespace page
{
	namespace util
	{
		namespace
		{
			// numeric atom testing
			// as in num_get stage 2 (22.2.2.1.2/8)
			inline bool IsNumeric(char c)
			{
				return std::isalnum(c) || c == '+' || c == '-' || c == '.';
			}

			// numeric string extraction
			// advances "in" iterator as a side-effect
			template <typename InputIterator>
				inline std::string AccumNum(InputIterator &in, InputIterator end)
			{
				std::string s;
				while (in != end && IsNumeric(*in))
					s.push_back(std::tolower(*in++));
				return s;
			}

			// symbolic target matching
			template <typename T> struct MatchTarget
			{
				std::string name;
				T val;
			};
			template <typename TargetIterator, typename T> inline bool Match(
				TargetIterator firstTarget, TargetIterator lastTarget,
				const std::string &in, std::ios_base::iostate &err, T &val)
			{
				for (TargetIterator target(firstTarget); target != lastTarget; ++target)
				{
					if (target->name.size() <= in.size() &&
						std::equal(target->name.begin(), target->name.end(), in.begin()))
					{
						val = target->val;
						return true;
					}
				}
				return false;
			}

			// numeric input/output
			// NOTE: our facets break certain semantic guarantees to allow for
			// additional features and flexibility
			struct num_get : std::num_get<char>
			{
				typedef std::num_get<char> Base;
				// HACK: GCC doesn't support using typename (bug #14258)
				typedef typename Base::iter_type iter_type;

				protected:
				iter_type do_get(iter_type, iter_type, std::ios_base &, std::ios_base::iostate &, bool &) const;
				iter_type do_get(iter_type, iter_type, std::ios_base &, std::ios_base::iostate &, float &) const;
				iter_type do_get(iter_type, iter_type, std::ios_base &, std::ios_base::iostate &, double &) const;
				iter_type do_get(iter_type, iter_type, std::ios_base &, std::ios_base::iostate &, long double &) const;

				private:
				template <typename T> inline iter_type get_float(iter_type, iter_type, std::ios_base &, std::ios_base::iostate &, T &) const;
			};
			struct num_put : std::num_put<char>
			{
				typedef std::num_put<char> Base;
				// HACK: GCC doesn't support using typename (bug #14258)
				typedef typename Base::iter_type iter_type;

				protected:
				iter_type do_put(iter_type, std::ios_base &, char_type, bool) const;
				iter_type do_put(iter_type, std::ios_base &, char_type, long) const;
				iter_type do_put(iter_type, std::ios_base &, char_type, unsigned long) const;
				iter_type do_put(iter_type, std::ios_base &, char_type, double) const;
				iter_type do_put(iter_type, std::ios_base &, char_type, long double) const;
				iter_type do_put(iter_type, std::ios_base &, char_type, const void *) const;

				private:
				template <typename T> inline iter_type put_float(iter_type, std::ios_base &, char_type, T) const;
			};

			// numeric input
			num_get::iter_type num_get::do_get(iter_type in, iter_type end, std::ios_base &str, std::ios_base::iostate &err, bool &val) const
			{
				// accept multiple symbolic targets
				if (str.flags() & std::ios_base::boolalpha)
				{
					std::string seq(AccumNum(in, end));
					typedef std::array<MatchTarget<bool>, 8> Targets;
					static const Targets targets =
					{
						"true",    true,
						"false",   false,
						"yes",     true,
						"no",      false,
						"on",      true,
						"off",     false,
						"enable",  true,
						"disable", false
					};
					if (!Match(targets.begin(), targets.end(), seq, err, val))
					{
						MAKE_ISTRINGSTREAM_1(char, seq)//auto ss(make_istringstream<char>(seq));
						Base::do_get(iter_type(ss.rdbuf()), iter_type(), str, err, val);
						if (err != std::ios_base::goodbit)
						{
							err = std::ios_base::goodbit;
							IosBaseFmtSaver iosFormatSaver(str);
							str.unsetf(std::ios_base::boolalpha);
							Base::do_get(iter_type(ss.rdbuf()), iter_type(), str, err, val);
						}
					}
					return in;
				}
				return Base::do_get(in, end, str, err, val);
			}
			num_get::iter_type num_get::do_get(iter_type in, iter_type end, std::ios_base &str, std::ios_base::iostate &err, float &val) const
			{
				return get_float(in, end, str, err, val);
			}
			num_get::iter_type num_get::do_get(iter_type in, iter_type end, std::ios_base &str, std::ios_base::iostate &err, double &val) const
			{
				return get_float(in, end, str, err, val);
			}
			num_get::iter_type num_get::do_get(iter_type in, iter_type end, std::ios_base &str, std::ios_base::iostate &err, long double &val) const
			{
				return get_float(in, end, str, err, val);
			}
			template <typename T> num_get::iter_type num_get::get_float(iter_type in, iter_type end, std::ios_base &str, std::ios_base::iostate &err, T &val) const
			{
				// accept multiple symbolic targets
				float sign = 0;
				if (*in == '+') ++in, sign = 1;
				if (*in == '-') ++in, sign = -1;
				std::string seq(AccumNum(in, end));
				typedef std::array<MatchTarget<T>, 5> Targets;
				static const Targets targets =
				{
					"infinity", math::Inf<T>(),
					"inf",      math::Inf<T>(),
					"1.#inf",   math::Inf<T>(),
					"nan",      math::Nan<T>(),
					"1.#nan",   math::Nan<T>()
				};
				if (!Match(targets.begin(), targets.end(), seq, err, val))
				{
					MAKE_ISTRINGSTREAM_1(char, seq)//auto ss(make_istringstream(seq));
					Base::do_get(iter_type(ss.rdbuf()), iter_type(), str, err, val);
				}
				val = std::copysign(val, sign);
				return in;
			}

			// numeric output
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, bool val) const
			{
				// use friendly names for boolean values
				if (str.flags() & std::ios_base::boolalpha)
				{
					std::string s(val ? "true" : "false");
					return std::copy(s.begin(), s.end(), out);
				}
				return Base::do_put(out, str, fill, val);
			}
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, long val) const
			{
				// prefix signed hexadecimal numbers with their sign
				std::ios_base::fmtflags flags = str.flags();
				if ((flags & std::ios_base::basefield) == std::ios_base::hex)
				{
					if (val < 0)
					{
						val = std::abs(val);
						str.width(std::max(str.width() - 1, 0));
						// add left fill characters before sign
						if (((flags & std::ios_base::adjustfield) == std::ios_base::right ||
							(flags & std::ios_base::adjustfield) == std::ios_base::internal &&
							!(flags & std::ios_base::showbase)) && str.width())
						{
							std::streamsize filln = str.width(0);
							for (unsigned long v = val; v; v >>= 4) --filln;
							if (flags & std::ios_base::showbase) filln -= 2;
							filln = std::max(filln, 0);
							while (filln--) *out++ = fill;
						}
						*out++ = '-';
					}
					return do_put(out, str, fill, static_cast<unsigned long>(val));
				}
				return Base::do_put(out, str, fill, val);
			}
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, unsigned long val) const
			{
				// support internal adjustment in hexadecimal numbers
				std::ios_base::fmtflags flags = str.flags();
				if ((flags & std::ios_base::basefield) == std::ios_base::hex &&
					(flags & std::ios_base::showbase))
				{
					// add numeric base prefix
					*out++ = '0';
					char sep = 'x';
					if (flags & std::ios_base::uppercase) sep = std::toupper(sep);
					*out++ = sep;
					str.width(std::max(str.width() - 2, 0));
					// add internal fill characters
					if ((flags & std::ios_base::adjustfield) == std::ios_base::internal)
					{
						std::streamsize filln = str.width(0);
						for (unsigned long v = val; v; v >>= 4) --filln;
						filln = std::max(filln, 0);
						while (filln--) *out++ = fill;
					}
					// remove redundant base prefix
					IosBaseFmtSaver iosFormatSaver(str);
					str.unsetf(std::ios_base::showbase);
					return Base::do_put(out, str, fill, val);
				}
				return Base::do_put(out, str, fill, val);
			}
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, double val) const
			{
				return put_float(out, str, fill, val);
			}
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, long double val) const
			{
				return put_float(out, str, fill, val);
			}
			num_put::iter_type num_put::do_put(iter_type out, std::ios_base &str, char_type fill, const void *val) const
			{
				// force fixed-length zero padding for pointers
				const unsigned ptrSize = sizeof val * CHAR_BIT / 4;
				std::ios_base::fmtflags flags = str.flags();
				// account for fill characters
				std::streamsize filln = str.width(0);
				if (filln)
				{
					filln -= ptrSize;
					if (flags & std::ios_base::showbase) filln -= 2;
					filln = std::max(filln, 0);
				}
				// add left fill characters
				if (filln && (flags & std::ios_base::adjustfield) == std::ios_base::right)
					while (filln--) *out++ = fill;
				// add numeric base prefix
				if (flags & std::ios_base::showbase)
				{
					*out++ = '0';
					char sep = 'x';
					if (flags & std::ios_base::uppercase) sep = std::toupper(sep);
					*out++ = sep;
				}
				// add internal fill characters
				if (filln && (flags & std::ios_base::adjustfield) == std::ios_base::internal)
					while (filln--) *out++ = fill;
				// add pointer value
				IosBaseFmtSaver iosFormatSaver(str);
				str.setf(std::ios_base::hex, std::ios_base::basefield);
				str.unsetf(std::ios_base::showbase);
				str.setf(std::ios_base::right, std::ios_base::adjustfield);
				str.width(ptrSize);
				Base::do_put(out, str, '0', static_cast<unsigned long>(reinterpret_cast<std::uintptr_t>(val)));
				// add right fill characters
				if (filln && (flags & std::ios_base::adjustfield) == std::ios_base::left)
					while (filln--) *out++ = fill;
				return out;
			}
			template <typename T> num_put::iter_type num_put::put_float(iter_type out, std::ios_base &str, char_type fill, T val) const
			{
				// use friendly names for special floating-point values
				std::string s;
				if (math::IsInf(val)) s = "infinity";
				else if (math::IsNan(val)) s = "NaN";
				else return Base::do_put(out, str, fill, val);
				if (std::signbit(val)) s.insert(s.begin(), '-');
				return std::copy(s.begin(), s.end(), out);
			}
		}

		// locale initialization
		void InitLocale()
		{
			std::locale::global(std::locale(std::locale(std::locale(), new num_get), new num_put));
		}
	}
}
