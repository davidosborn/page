/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "Color.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace page
{
	namespace math
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
	}
}
