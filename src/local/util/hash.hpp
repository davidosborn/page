#ifndef    page_local_util_hash_hpp
#   define page_local_util_hash_hpp

#	include <cstddef> // size_t
#	include <functional> // binary_function

namespace page
{
	namespace util
	{
		// hash functions
		template <typename T> std::size_t make_hash(const T &);
		std::size_t combine_hash(std::size_t, std::size_t);
		template <typename InputIterator> std::size_t hash_range(InputIterator first, InputIterator last);

		// hash function object wrappers
		struct combine_hash_function : std::binary_function<std::size_t, std::size_t, std::size_t>
		{
			std::size_t operator ()(std::size_t, std::size_t) const;
		};

		// Paul Hsieh's fast hash function
		std::size_t SuperFastHash(const char *, unsigned n, std::size_t seed);
		std::size_t SuperFastHash(const char *, unsigned n);
	}
}

#	include "hash.tpp"
#endif
