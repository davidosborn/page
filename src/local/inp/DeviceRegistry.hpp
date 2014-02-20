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

#ifndef    page_local_inp_DeviceRegistry_hpp
#   define page_local_inp_DeviceRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>
#	include <typeinfo>
#	include <vector>

#	include "../util/class/Monostate.hpp"
#	include "../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace wnd { class Window; }}

namespace page { namespace inp
{
	class Device;

	/**
	 * A factory function that will return a new @c Device object, constructed
	 * with the provided arguments.
	 */
	using DeviceFactoryFunction = std::function<std::unique_ptr<Device> (wnd::Window &)>;

	/**
	 * A function that will return @c true if the dervice is compatible with the
	 * specified window.
	 */
	using DeviceCompatibleFunction = std::function<bool (wnd::Window &)>;

////////// DeviceRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c DeviceRegistry which contains information
	 * about a device class.
	 */
	struct DeviceRegistryRecord
	{
		DeviceRegistryRecord(
			std::type_info           const& type,
			DeviceFactoryFunction    const& factoryFunction,
			DeviceCompatibleFunction const& compatibleFunction,
			std::string              const& name);

		/**
		 * The type of the device.
		 */
		const std::type_info &type;

		/**
		 * @copydoc DeviceFactoryFunction
		 */
		DeviceFactoryFunction factoryFunction;

		/**
		 * @copydoc DeviceCompatibleFunction
		 */
		DeviceCompatibleFunction compatibleFunction;

		/**
		 * The name of the device.
		 */
		std::string name;
	};

////////// DeviceRegistry /////////////////////////////////////////////////////

	/**
	 * A place for registering @c Device classes.
	 *
	 * @addtogroup registry
	 */
	class DeviceRegistry : public util::Monostate<DeviceRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef DeviceRegistryRecord Record;

		public:
		/**
		 * Registers a device class.
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
		 * Searches for all of the devices that are compatible with the
		 * specified window and returns a new instance of each.
		 */
		std::vector<std::unique_ptr<Device>> MakeAll(wnd::Window &) const;

		private:
		std::list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c DeviceRegistry.
	 *
	 * @note @a WINDOW must be a fully-defined type; a forward declaration is
	 * not sufficient, as @c dynamic_cast will be applied to it.
	 */
#	define REGISTER_DEVICE(T, WINDOW, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::inp::DeviceRegistry).Register<T, WINDOW>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "DeviceRegistry.tpp"
#endif
