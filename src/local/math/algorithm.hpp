#ifndef    page_math_algorithm_hpp
#   define page_math_algorithm_hpp

#	include "fwd.hpp" // Vector

namespace page { namespace math
{
	void RgbToYcbcr420(
		const unsigned char *rgb,
		unsigned char *y, unsigned char *cb, unsigned char *cr,
		const Vector<2, unsigned> &size,
		unsigned rgbStride, unsigned ycbcrStride);
}}

#endif
