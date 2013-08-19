#include "Color.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace page { namespace math
{
	namespace
	{
		const Matrix<3, 3, int> rgbToYcbcrColorMatrix(
			RgbToYcbcrColorMatrix() *
			ScaleMatrix(Vector<3>(YcbcrScaleColor())) *
			(1u << 24));
	}

	void RgbToYcbcr420(
		const unsigned char *rgb,
		unsigned char *y, unsigned char *cb, unsigned char *cr,
		const Vector<2, unsigned> &size,
		unsigned rgbStride, unsigned ycbcrStride)
	{
		// FIXME: provide SIMD implementation
		for (unsigned i = 0; i < size.y; ++i)
		{
			const unsigned char *rgbRow = rgb;
			unsigned char *yRow = y;
			if (!(i % 2))
			{
				unsigned char
					*cbRow = cb,
					*crRow = cr;
				for (unsigned i = 0; i < size.x; ++i)
				{
					Vector<3, int> color(rgb[0], rgb[1], rgb[2]);
					*y++ = unsigned(Dot(Vector<3, int>(rgbToYcbcrColorMatrix.Row(0)), color) + (16u << 24)) >> 24;
					if (!(i % 2))
					{
						*cb++ = unsigned(Dot(Vector<3, int>(rgbToYcbcrColorMatrix.Row(1)), color) + (128u << 24)) >> 24;
						*cr++ = unsigned(Dot(Vector<3, int>(rgbToYcbcrColorMatrix.Row(2)), color) + (128u << 24)) >> 24;
					}
					rgb += 3;
				}
				cb = cbRow + ycbcrStride / 2;
				cr = crRow + ycbcrStride / 2;
			}
			else
			{
				for (unsigned i = 0; i < size.x; ++i)
				{
					Vector<3, int> color(rgb[0], rgb[1], rgb[2]);
					*y++ = unsigned(Dot(Vector<3, int>(rgbToYcbcrColorMatrix.Row(0)), color) + (16u << 24)) >> 24;
					rgb += 3;
				}
			}
			rgb = rgbRow + rgbStride;
			y   = yRow   + ycbcrStride;
		}
	}
}}
