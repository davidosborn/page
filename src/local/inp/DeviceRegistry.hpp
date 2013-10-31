#ifndef    page_local_inp_DeviceRegistry_hpp
#   define page_local_inp_DeviceRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>
#	include <typeindex>
#	include <typeinfo>
#	include <unordered_map>
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

////////// DeviceRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c DeviceRegistry which contains information
	 * about a device class.
	 */
	struct DeviceRegistryRecord
	{
		DeviceRegistryRecord(
			std::type_info        const& type,
			DeviceFactoryFunction const& factoryFunction,
			std::string           const& name);

		/**
		 * The type of the device.
		 */
		const std::type_info &type;

		/**
		 * @copydoc DeviceFactoryFunction
		 */
		DeviceFactoryFunction factoryFunction;

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
		void Register(const std::type_info &windowType, const Record &);

		public:
		/**
		 * Searches for all of the devices that are compatible with the
		 * specified window and returns a new instance of each.
		 */
		std::vector<std::unique_ptr<Device>> MakeAll(wnd::Window &) const;

		private:
		struct WindowTypeRecord
		{
			std::list<Record> records;
		};
		std::unordered_map<std::type_index, WindowTypeRecord> windowTypes;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c DeviceRegistry.
	 */
#	define REGISTER_DEVICE(T, WINDOW, ...) \
		namespace \
		{ \
			struct Initializer() \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::wnd::DeviceRegistry).Register<T, WINDOW>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "DeviceRegistry.tpp"
#endif
