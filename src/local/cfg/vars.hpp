#ifndef    page_local_cfg_vars_hpp
#   define page_local_cfg_vars_hpp

#	include "state/State.hpp"

	/**
	 * @return The value of the specified configuration variable from the
	 *         global @c State.
	 *
	 * @note To access the value of a variable, use:
	 *       @code *CVAR(name) @endcode
	 *
	 * @note To change the value of a variable, use:
	 *       @code CVAR(name)[source] = value @endcode
	 */
#	define CVAR(x) (GLOBAL(::page::cfg::State).x)

#endif
