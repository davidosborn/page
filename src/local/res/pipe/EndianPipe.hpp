#ifndef    page_local_res_pipe_EndianPipe_hpp
#   define page_local_res_pipe_EndianPipe_hpp

#	include <memory> // shared_ptr
#	include <string>

#	include "../../util/endian.hpp" // Endian
#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct EndianPipe : Pipe
		{
			explicit EndianPipe(const std::shared_ptr<const Pipe> &,
				const std::string &format, util::Endian source,
				util::Endian destination = util::nativeEndian);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			std::shared_ptr<const Pipe> super;
			std::string format;
			util::Endian srcEndian, destEndian;
		};
	}
}

#endif
