#ifndef    page_local_inp_DriverRegistry_hpp
#   define page_local_inp_DriverRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>
#	include <typeinfo>

#	include "../util/class/Monostate.hpp"
#	include "../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace wnd { class Window; }}

namespace page { namespace inp
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
					GLOBAL(::page::inp::DriverRegistry).Register<T, WINDOW>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "DriverRegistry.tpp"
#endif
