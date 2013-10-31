#ifndef    page_local_inp_Modifier_hpp
#   define page_local_inp_Modifier_hpp

#	include "../util/Flags.hpp"

namespace page { namespace inp
{
	/**
	 * Control modifiers.
	 */
	struct Modifier : util::Flags
	{
		enum
		{
			/**
			 * A modifier that quickens the controlled entity.
			 */
			run = 0x01,

			/**
			 * A modifier that enables alternate behaviour.
			 */
			alt = 0x02
		};
	};
}}

#endif
