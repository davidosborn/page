#ifndef    page_local_wnd_WindowRegistry_hpp
#   define page_local_wnd_WindowRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>

#	include "../util/class/Monostate.hpp"
#	include "../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace wnd
{
	class Window;

	/**
	 * A factory function that will return a new @c Window object, constructed
	 * with the provided arguments.
	 */
	using WindowFactoryFunction = std::function<std::unique_ptr<Window> (const std::string &title)>;

////////// WindowRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c WindowRegistry which contains information
	 * about a window class.
	 */
	struct WindowRegistryRecord
	{
		WindowRegistryRecord(
			std::type_info        const& type,
			WindowFactoryFunction const& factoryFunction,
			std::string           const& name,
			int                          priority = 0);

		/**
		 * The type of the window.
		 */
		const std::type_info &type;

		/**
		 * @copydoc WindowFactoryFunction
		 */
		WindowFactoryFunction factoryFunction;

		/**
		 * The name of the window.
		 */
		std::string name;

		/**
		 * A higher priority means that this window will be tried before other
		 * windows with a lower priority.
		 */
		int priority;
	};

////////// WindowRegistry /////////////////////////////////////////////////////

	/**
	 * A place for registering @c Window classes.
	 *
	 * @addtogroup registry
	 */
	class WindowRegistry : public util::Monostate<WindowRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef WindowRegistryRecord Record;

		public:
		/**
		 * Registers a window class.
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
		 * Searches for a window that is capable of handling the specified path
		 * and returns a new instance of it.
		 */
		std::unique_ptr<Window> Make(const std::string &title) const;

		private:
		std::list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c WindowRegistry.
	 */
#	define REGISTER_WINDOW(T, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::wnd::WindowRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "WindowRegistry.tpp"
#endif
