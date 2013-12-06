#ifndef    page_local_res_source_SourceRegistry_hpp
#   define page_local_res_source_SourceRegistry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <string>

#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace res
{
	/**
	 * A factory function that will return a new @c Source object, constructed
	 * with the provided arguments.
	 */
	using SourceFactoryFunction = std::function<std::unique_ptr<Source> (const std::string &path)>;

	/**
	 * A function that will return @c true if the source is capable of handling
	 * the specified path.
	 */
	using SourceCompatibleFunction = std::function<bool (const std::string &path)>;

////////// SourceRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c SourceRegistry which contains information
	 * about a source class.
	 */
	struct SourceRegistryRecord
	{
		SourceRegistryRecord(
			SourceFactoryFunction    const& factoryFunction,
			SourceCompatibleFunction const& compatibleFunction,
			int                             priority = 0);

		/**
		 * @copydoc SourceFactoryFunction
		 */
		SourceFactoryFunction factoryFunction;

		/**
		 * @copydoc SourceCompatibleFunction
		 */
		SourceCompatibleFunction compatibleFunction;

		/**
		 * A higher priority means that this source will be tried before other
		 * sources with a lower priority.
		 */
		int priority;
	};

////////// SourceRegistry //////////////////////////////////////////////////////

	/**
	 * A place for registering @c Source classes.
	 *
	 * @addtogroup registry
	 */
	class SourceRegistry : public util::Monostate<SourceRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef SourceRegistryRecord Record;

		public:
		/**
		 * Registers a source class.
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
		 * Searches for a source that is capable of handling the specified path
		 * and returns a new instance of it.
		 */
		std::unique_ptr<Source> Make(const std::string &path) const;

		private:
		std::list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c SourceRegistry.
	 */
#	define REGISTER_SOURCE(T, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::SourceRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "SourceRegistry.tpp"
#endif
