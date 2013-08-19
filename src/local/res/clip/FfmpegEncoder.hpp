#ifndef    page_local_res_clip_FfmpegEncoder_hpp
#   define page_local_res_clip_FfmpegEncoder_hpp

#	include "Encoder.hpp"

namespace page { namespace res { namespace clip
{
	/**
	 * An encoder for the formats supported by FFmpeg.
	 */
	class FfmpegEncoder : public Encoder
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		FfmpegEncoder(
			const Callback &,
			const math::Vec2u &size,
			float frameRate,
			float quality);

		~FfmpegEncoder();

		/*---------+
		| encoding |
		+---------*/

		private:
		void Encode(const void *) override;

		/**
		 *
		 */
		void EncodeBuffer(bool lastFrame = false);
	};
}}}

#endif
