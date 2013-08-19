#include <functional> // hash
#include <numeric> // accumulate

namespace page
{
	namespace util
	{
		// hash functions
		template <typename T> std::size_t make_hash(const T &val)
		{
			return std::hash<T>(val);
		}
		template <typename InputIterator> std::size_t hash_range(InputIterator first, InputIterator last)
		{
			return std::accumulate(first, last, 0, combine_hash_function());
		}
	}
}
