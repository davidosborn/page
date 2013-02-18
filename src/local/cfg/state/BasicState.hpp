/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_cfg_state_BasicState_hpp
#   define page_local_cfg_state_BasicState_hpp

	// C++
#	include <list>

	// Boost
#	include <boost/range/adaptor/indirected.hpp>

	// local
#	include "../../util/class/copy_move.hpp" // DEFINE_{COPY,MOVE}

namespace page
{
	namespace cfg
	{
		class BasicVar;

		/**
		 * The base class for a manager of a set of configuration variables.
		 */
		class BasicState
		{
			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			public:
			BasicState() = default;

			DEFINE_COPY(BasicState, delete)
			DEFINE_MOVE(BasicState, delete)

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

			public:
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
	}
}

#endif
