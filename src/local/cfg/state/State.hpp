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
		 * Reads configuration variables from disk, incorporating the changes to
		 * unmodified variables while retaining the modified ones.
		 */
		void Update();

		/**
		 * Writes modified configuration variables to disk.
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
