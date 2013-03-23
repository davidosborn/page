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

#ifndef    page_local_wnd_ConsoleRegistry_hpp
#   define page_local_wnd_ConsoleRegistry_hpp

#	include <forward_list>
#	include <functional>
#	include <memory> // unique_ptr
#	include <string>

#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace res
{
	/**
	 * A pointer to a factory function that, when called, will return a new
	 * @c Console object, constructed with the given arguments.
	 */
	using ConsoleFactoryFunction = std::function<std::unique_ptr<Console> (const std::string &title)>;

////////// ConsoleRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c ConsoleRegistry which contains information
	 * about a console class.
	 */
	struct ConsoleRegistryRecord
	{
		ConsoleRegistryRecord(
			std::type_info         const& type,
			ConsoleFactoryFunction const& factoryFunction,
			std::string            const& name,
			int                           priority = 0);

		/**
		 * The type of the console.
		 */
		const std::type_info &type;

		/**
		 * @copydoc ConsoleFactoryFunction
		 */
		ConsoleFactoryFunction factoryFunction;

		/**
		 * The name of the console.
		 */
		std::string name;

		/**
		 * A higher priority means that this console will be tried before other
		 * consoles with a lower priority.
		 */
		int priority;
	};

////////// ConsoleRegistry /////////////////////////////////////////////////////

	/**
	 * A place for registering @c Console classes.
	 */
	class ConsoleRegistry : public util::Monostate<ConsoleRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef ConsoleRegistryRecord Record;

		public:
		/**
		 * Registers a console class.
		 */
		template <typename T, typename... RecordArgs>
			void Register(RecordArgs &&...);

		private:
		/**
		 * @copydoc Register
		 */
		void Register(const Record &);

		public:
		/**
		 * Searches for a console that is capable of handling the specified path
		 * and returns a new instance of it.
		 */
		std::unique_ptr<Console> Make(const std::string &title) const;

		private:
		std::forward_list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c ConsoleRegistry.
	 */
#	define REGISTER_CONSOLE(T, ...) \
		namespace \
		{ \
			struct Initializer() \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::wnd::ConsoleRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "ConsoleRegistry.tpp"
#endif
