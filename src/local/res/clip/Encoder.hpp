#ifndef    page_local_res_clip_Encoder_hpp
#   define page_local_res_clip_Encoder_hpp

#	include <vector>

#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "EncoderRegistry.hpp"

namespace page { namespace res { namespace clip
{
	/**
	 * The base class for a clip encoder.
	 */
	class Encoder : public util::Uncopyable<Encoder>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		protected:
		Encoder(const EncoderCallback &, unsigned frameSize);

		public:
		virtual ~Encoder() = default;

		/*----------+
		| raw input |
		+----------*/

		public:
		/**
		 *
		 *
		 * @note Expects packed RGB8 data.
		 */
		void Write(const void *, unsigned);

		/*---------------+
		| encoded output |
		+---------------*/

		protected:
		/**
		 *
		 */
		void Return(const void *, unsigned) const;

		/*---------+
		| encoding |
		+---------*/

		private:
		/**
		 *
		 *
		 * @note Expects one complete frame of data.
		 */
		virtual void Encode(const void *) = 0;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 *
		 */
		Callback cb;

		/**
		 *
		 */
		unsigned frameSize;

		/**
		 *
		 */
		std::vector<char> buffer;
	};
}}}

#endif
