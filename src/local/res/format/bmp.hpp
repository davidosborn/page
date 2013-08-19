#ifndef    page_local_res_format_bmp_hpp
#   define page_local_res_format_bmp_hpp

#	include <cstdint> // int32_t, uint{16,32}_t

namespace page { namespace res
{
	namespace format
	{
		namespace bmp
		{
			const std::uint16_t type = 0x4d42;

#	pragma pack(push, 1)
			struct BITMAPFILEHEADER
			{
				std::uint16_t bfType;
				std::uint32_t bfSize;
				std::uint16_t bfReserved1, bfReserved2;
				std::uint32_t bfOffBits;
			};
			struct BITMAPINFOHEADER
			{
				std::uint32_t bfSize;
				std::int32_t  biWidth, biHeight;
				std::uint16_t biPlanes, biBitCount;
				std::uint32_t biCompression, biSizeImage;
				std::int32_t  biXPelsPerMeter, biYPelsPerMeter;
				std::uint32_t biClrUsed, biClrImportant;
			};
#	pragma pack(pop)

			const char fileHeaderFormat[] = "wdwwd";
			const char infoHeaderFormat[] = "dddwwdddddd";
		}

		using namespace bmp;
	}

	typedef format::BITMAPFILEHEADER BITMAPFILEHEADER;
	typedef format::BITMAPINFOHEADER BITMAPINFOHEADER;
}}

#endif
