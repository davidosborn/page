#ifndef    page_local_res_clip_Stream_hpp
#   define page_local_res_clip_Stream_hpp

#	include <fstream> // ofstream
#	include <memory> // unique_ptr
#	include <string>
#	include <vector>

#	include "../../math/Vector.hpp"
#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page { namespace res { class Image; }}

namespace page { namespace res { namespace clip
{
	class Encoder;

	/**
	 * An output stream for encoding and writing a clip to a file.
	 *
	 * @fixme This class could be made copyable by storing the encoder in a @c
	 *        std::shared_ptr, which would only make sense if the encoder didn't
	 *        maintain any output state.
	 */
	struct Stream : public util::Uncopyable<Stream>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		Stream(
			const std::string &path,
			const std::string &format,
			const math::Vec2u &size,
			float frameRate,
			float quality);

		~Stream();

		/*-----------+
		| attributes |
		+-----------*/

		public:
		const math::Vec2u &GetSize() const;

		/*------------------+
		| audio/video input |
		+------------------*/

		public:
		void Write(const res::Image &);

		/*---------------+
		| encoded output |
		+---------------*/

		private:
		void WriteEncoded(const void *, unsigned);

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::Vec2u size;
		std::ofstream fs;
		std::unique_ptr<Encoder> encoder;
	};
}}}

#endif
