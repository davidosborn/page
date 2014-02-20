/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_cfg_state_State_hpp
#   define page_local_cfg_state_State_hpp

#	include <list>
#	include <memory> // shared_ptr
#	include <ostream> // basic_ostream

#	include "../../util/class/Monostate.hpp"
#	include "CommonState.hpp"

#	ifdef USE_OPENGL
#		include "opengl/RenderState.hpp"
#	endif

namespace page { namespace cfg
{
	class Source;

	/**
	 * Manages a set of configuration sources, providing fall-through access to
	 * the variables contained in them.
	 */
	class State :
		public CommonState,
#	ifdef USE_OPENGL
		public opengl::RenderState,
#	endif
		public util::Monostate<State>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * Uses the default source configuration.
		 */
		State();

		/**
		 * Calls Commit().
		 */
		~State();

		/*-----------------+
		| revision control |
		+-----------------*/

		/**
		 * Updates the configuration state by reading the unmodified
		 * configuration variables from disk.  The modified configuration
		 * variables are left alone.
		 */
		void Update();

		/**
		 * Saves the configuration state by writing the modified configuration
		 * variables to disk.
		 */
		void Commit();

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * An ordered list of sources, used to initialize configuration
		 * variables in a fall-through manner.
		 */
		std::list<std::shared_ptr<Source>> sources;
	};

	/*-----------------------+
	| user-friendly printing |
	+-----------------------*/

	/**
	 * Writes a list of the registered configuration variables and their values,
	 * contained in the given instance of @c State, to the given stream.
	 */
	template <typename Char, typename CharTraits>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const State &);

	/**
	 * If verbose logging is enabled, write a list of the registered
	 * configuration variables and their values, contained in the given instance
	 * of @c State.
	 */
	void Print(const State &);
}}

#endif
