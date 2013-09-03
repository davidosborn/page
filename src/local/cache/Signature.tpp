#include <sstream> // ostringstream
#include <utility> // forward

#include "../util/type_traits/range.hpp" // is_range
#include "../util/type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	namespace detail
	{
		template <typename T>
			void SerializeDependency(std::ostream &os, T x,
				ENABLE_IF(!util::is_range<T>::value))
		{
			os << x;
		}

		template <typename InputRange>
			void SerializeDependency(std::ostream &os, InputRange range,
				ENABLE_IF(util::is_range<InputRange>::value))
		{
			for (const auto &x : range)
				SerializeDependency(os, x);
		}

		template <typename Arg>
			void MakeSource(std::ostream &os, Arg &&arg)
		{
			SerializeDependency(os, std::forward<Arg>(arg));
		}

		template <typename Arg, typename Arg2, typename... Args>
			void MakeSource(std::ostream &os, Arg &&arg, Arg2 &&arg2, Args &&... args)
		{
			SerializeDependency(os, std::forward<Arg>(arg));
			MakeSource(os << ',',
				std::forward<Arg2>(arg2),
				std::forward<Args>(args)...);
		}
	}

	template <typename... Args>
		Signature::Signature(const std::string &type, Args &&... args)
	{
		std::ostringstream ss;
		ss << type << '(';
		detail::MakeSource(ss, std::forward<Args>(args)...);
		ss << ')';
		s = ss.str();
	}
}}
