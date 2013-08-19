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
