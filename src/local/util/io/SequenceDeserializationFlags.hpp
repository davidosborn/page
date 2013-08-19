#ifndef    page_local_util_io_SequenceDeserializationFlags_hpp
#   define page_local_util_io_SequenceDeserializationFlags_hpp

#	include "../Flags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A collection of bit-flags for modifying the behaviour of the sequence
		 * deserialization functions.
		 */
		struct SequenceDeserializationFlags : Flags
		{
			enum
			{
				/**
				 * Indicates that consecutive delimiters should be viewed as a
				 * single delimiter.
				 */
				collapse = 0x01,

				/**
				 * Indicates that consecutive leading and trailing delimiters
				 * should be ignored.
				 */
				trim = 0x02,

				/**
				 * A default flag for whitespace delimiters.
				 */
				defaultForSpaceDelimiter = collapse | trim
			};
		};
	}
}

#endif
