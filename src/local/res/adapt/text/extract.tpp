#include <algorithm> // fill
#include <iterator> // iterator_traits

#include <boost/lexical_cast.hpp>

#include "../../../err/Exception.hpp"
#include "../../../util/string/operations.hpp" // Partition

namespace page
{
	namespace res
	{
		template <typename OutputIterator>
			void Extract(const std::string &value, OutputIterator iter, OutputIterator end)
		{
			if (iter == end)
			{
				if (!value.empty())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("too many arguments")))
				return;
			}
			typedef typename std::iterator_traits<OutputIterator>::value_type Type;
			util::Partitioned part(util::Partition(value));
			if (!part.second.empty())
			{
				for (;; part = util::Partition(part.second))
				{
					*iter = boost::lexical_cast<Type>(part.first);
					if (++iter == end) break;
					if (part.second.empty())
						THROW((err::Exception<err::ResModuleTag, err::FormatTag>("not enough arguments")))
				}
				if (!part.second.empty())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("too many arguments")))
			}
			else std::fill(iter, end, boost::lexical_cast<Type>(part.first));
		}
		template <typename Container> void Extract(const std::string &value, Container &container)
		{
			return Extract(value, container.begin(), container.end());
		}
	}
}
