#ifndef    page_local_util_container_ScratchBuffer_hpp
#   define page_local_util_container_ScratchBuffer_hpp

#	include <vector>

#	include "../class/Monostate.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A static-size temporary-data container.
		 */
		class ScratchBuffer :
			public Monostate<ScratchBuffer>
		{
			public:
			ScratchBuffer();

			// capacity
			size_type size() const noexcept;

			// data access
			value_type *data() noexcept;
			const value_type *data() const noexcept;

			private:
			std::vector<char> v;
		};
	}
}

#endif
