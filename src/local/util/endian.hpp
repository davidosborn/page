#ifndef    page_local_util_endian_hpp
#   define page_local_util_endian_hpp

#	include <string>

namespace page
{
	namespace util
	{
		enum Endian
		{
			bigEndian,
			littleEndian,
			nativeEndian =
#	ifdef WORDS_BIGENDIAN
				bigEndian
#	else
				littleEndian
#	endif
		};

		void SwitchEndian(void *data, unsigned size, Endian source, Endian destination = nativeEndian);
		void TransformEndian(void *data, const std::string &format, Endian source, Endian destination = nativeEndian);
		void TransformEndianArray(void *data, unsigned n, const std::string &format, Endian source, Endian destination = nativeEndian);
	}
}

#endif
