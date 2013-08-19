#ifndef    page_local_res_clip_TheoraEncoder_hpp
#   define page_local_res_clip_TheoraEncoder_hpp

#	include <vector>

#	include <theora/theora.h>

#	include "../../math/Vector.hpp"
#	include "Encoder.hpp"

namespace page { namespace res { namespace clip
{
	/**
	 * An encoder for the Theora format.
	 */
	class TheoraEncoder : public Encoder
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		TheoraEncoder(
			const Callback &,
			const math::Vec2u &size,
			float frameRate,
			float quality);

		~TheoraEncoder();

		/*---------+
		| encoding |
		+---------*/

		private:
		void Encode(const void *) override;

		/**
		 *
		 */
		void EncodeBuffer(bool lastFrame = false);

		/*-------------+
		| data members |
		+-------------*/

		private:
		theora_state ts;
		ogg_stream_state os;
		yuv_buffer tb;
		std::vector<unsigned char> tbData;
		math::Vec2u size, size16, offset;
	};
}}}

#endif
