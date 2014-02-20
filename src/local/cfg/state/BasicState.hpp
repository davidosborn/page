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

#ifndef    page_local_cfg_state_BasicState_hpp
#   define page_local_cfg_state_BasicState_hpp

#	include <list>

#	include <boost/range/adaptor/indirected.hpp>

#	include "../../util/class/special_member_functions.hpp" // Unmovable

namespace page { namespace cfg
{
	class BasicVar;

	/**
	 * The base class for a set of configuration variables.
	 *
	 * @note This class has been made unmovable to prevent the configuration-
	 *       variable pointers from being invalidated, since these variables
	 *       will be members of the derived class.  It would be possible to make
	 *       this class movable by having the move functions update these
	 *       pointers.
	 */
	class BasicState : public util::Unmovable<BasicState>
	{
		/*------------------+
		| cvar registration |
		+------------------*/

		public:
		/**
		 * Register a configuration variable.
		 */
		void Register(BasicVar *);

		/**
		 * Unregister a configuration variable.
		 */
		void Unregister(BasicVar *);

		/*---------------+
		| cvar iteration |
		+---------------*/

		/**
		 * @return a range providing iterable access to all registered
		 * configuration variables, ordered by key.
		 */
		boost::indirected_range<std::list<BasicVar *>> GetVars() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * A list of registered configuration variables, ordered by key
		 */
		std::list<BasicVar *> vars;
	};
}}

#endif
