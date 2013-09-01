#include <sstream> // ostringstream
#include <utility> // forward

namespace page { namespace cache
{
	namespace detail
	{
		template <typename Arg>
			void MakeSource(std::ostream &os, Arg &&arg)
		{
			os << std::forward<Arg>(arg);
		}

		template <typename Arg, typename Arg2, typename... Args>
			void MakeSource(std::ostream &os, Arg &&arg, Arg2 &&arg2, Args &&... args)
		{
			os << std::forward<Arg>(arg) << ',';
			MakeSource(os,
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
