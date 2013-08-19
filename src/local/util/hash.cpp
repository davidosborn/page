#include <cassert>
#include <cstdint> // uint{16,8}_t

#include "hash.hpp" // combine_hash_function::operator ()

// for SuperFastHash
#ifdef WORDS_BIGENDIAN
#	define GET_16_BITS(x) (( \
		static_cast<std::uint16_t>(reinterpret_cast<const std::uint8_t *>(x)[1]) << 8) + \
		static_cast<std::uint16_t>(reinterpret_cast<const std::uint8_t *>(x)[0]))
#else
#	define GET_16_BITS(x) (*reinterpret_cast<const std::uint16_t *>(x))
#endif

namespace page
{
	namespace util
	{
		// hash functions
		std::size_t combine_hash(std::size_t a, std::size_t b)
		{
			// algorithm from Boost.Functional/Hash by Daniel James
			return a ^ ((a << 6) + (a >> 2) + 0x9e3779b9 + b);
		}

		// hash function object wrappers
		std::size_t combine_hash_function::operator ()(std::size_t a, std::size_t b) const
		{
			return combine_hash(a, b);
		}

		// Paul Hsieh's fast hash function
		// http://www.azillionmonkeys.com/qed/hash.html
		std::size_t SuperFastHash(const char *s, unsigned n, std::size_t hash)
		{
			if (!n) return hash;
			assert(s);
			for (unsigned i = n >> 2; i; --i)
			{
				hash += GET_16_BITS(s);
				hash = (hash << 16) ^ ((GET_16_BITS(s + 2) << 11) ^ hash);
				s += sizeof(std::uint16_t) * 2;
				hash += hash >> 11;
			}
			switch (n & 3)
			{
				case 3:
				hash += GET_16_BITS(s);
				hash ^= hash << 16;
				hash ^= s[sizeof(std::uint16_t)] << 18;
				hash += hash >> 11;
				break;
				case 2:
				hash += GET_16_BITS(s);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
				case 1:
				hash += *s;
				hash ^= hash << 10;
				hash += hash >> 1;
				break;
			}
			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;
			return hash;
		}
		std::size_t SuperFastHash(const char *s, unsigned n)
		{
			SuperFastHash(s, n, n);
		}
	}
}
