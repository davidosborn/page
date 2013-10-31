#ifndef    page_local_inp_Modifier_hpp
#   define page_local_inp_Modifier_hpp

namespace page { namespace inp
{
	/**
	 * Control modifiers.
	 */
	struct Modifier
	{
		enum BitSet
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
