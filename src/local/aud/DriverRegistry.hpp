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

#ifndef    page_local_aud_DriverRegistry_hpp
#   define page_local_aud_DriverRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>
#	include <typeinfo>

#	include "../util/class/Monostate.hpp"
#	include "../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace wnd { class Window; }}

namespace page { namespace aud
{
	class Driver;

	/**
	 * A factory function that will return a new @c Driver object, constructed
	 * with the provided arguments.
	 */
	using DriverFactoryFunction = std::function<std::unique_ptr<Driver> (wnd::Window &)>;

	/**
	 * A function that will return @c true if the driver is compatible with the
	 * specified window.
	 */
	using DriverCompatibleFunction = std::function<bool (wnd::Window &)>;

////////// DriverRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c DriverRegistry which contains information
	 * about a driver class.
	 */
	struct DriverRegistryRecord
	{
		DriverRegistryRecord(
			std::type_info           const& type,
			DriverFactoryFunction    const& factoryFunction,
			DriverCompatibleFunction const& compatibleFunction,
			std::string              const& name,
			int                             priority = 0);

		/**
		 * The type of the driver.
		 */
		const std::type_info &type;

		/**
		 * @copydoc DriverFactoryFunction
		 */
		DriverFactoryFunction factoryFunction;

		/**
		 * @copydoc DriverCompatibleFunction
		 */
		DriverCompatibleFunction compatibleFunction;

		/**
		 * The name of the driver.
		 */
		std::string name;

		/**
		 * A higher priority means that this driver will be tried before other
		 * drivers with a lower priority.
		 */
		int priority;
	};

////////// DriverRegistry /////////////////////////////////////////////////////

	/**
	 * A place for registering @c Driver classes.
	 *
	 * @addtogroup registry
	 */
	class DriverRegistry : public util::Monostate<DriverRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef DriverRegistryRecord Record;

		public:
		/**
		 * Registers a driver class.
		 */
		template <typename T, typename Window, typename... RecordArgs>
			void Register(RecordArgs &&...);

		private:
		/**
		 * @copydoc Register
		 */
		void Register(const Record &);

		public:
		/**
		 * Searches for a driver that is compatible with the specified window
		 * and returns a new instance of it.
		 */
		std::unique_ptr<Driver> Make(wnd::Window &) const;

		private:
		std::list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c DriverRegistry.
	 *
	 * @note @a WINDOW must be a fully-defined type; a forward declaration is
	 * not sufficient, as @c dynamic_cast will be applied to it.
	 */
#	define REGISTER_DRIVER(T, WINDOW, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::aud::DriverRegistry).Register<T, WINDOW>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "DriverRegistry.tpp"
#endif
