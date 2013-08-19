#include <algorithm> // reverse

#include "endian.hpp" // Endian

namespace page
{
	namespace util
	{
		namespace
		{
			void Switch(void *data, unsigned size)
			{
				std::reverse(
					static_cast<unsigned char *>(data),
					static_cast<unsigned char *>(data) + size);
			}
			unsigned Transform(void *data, const std::string &fmt)
			{
				char *ptr = static_cast<char *>(data);
				for (std::string::const_iterator unit(fmt.begin()); unit != fmt.end(); ++unit)
				{
					unsigned size;
					switch (*unit)
					{
						case 'b': size = 1; break; // byte
						case 'w': size = 2; break; // word
						case 't': size = 3; break; // tbyte
						case 'd': size = 4; break; // dword
						case 'T': size = 6; break; // tword
						case 'q': size = 8; break; // qword
					}
					Switch(ptr, size);
					ptr += size;
				}
				return ptr - static_cast<char *>(data);
			}
		}

		void SwitchEndian(void *data, unsigned size, Endian src, Endian dest)
		{
			if (src != dest) Switch(data, size);
		}
		void TransformEndian(void *data, const std::string &fmt, Endian src, Endian dest)
		{
			if (src != dest) Transform(data, fmt);
		}
		void TransformEndianArray(void *data, unsigned n, const std::string &fmt, Endian src, Endian dest)
		{
			if (src == dest) return;
			for (unsigned i = 0; i < n; ++i)
				data = static_cast<char *>(data) + Transform(data, fmt);
		}
	}
}
