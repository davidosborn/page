#ifndef    page_local_res_type_TypeRegistry_hpp
#   define page_local_res_type_TypeRegistry_hpp

#	include <functional> // function
#	include <string>
#	include <type_traits> // {false,true}_type
#	include <typeindex>
#	include <typeinfo>
#	include <unordered_map>

#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY
#	include "../../util/memory/DefaultDeleter.hpp"
#	include "../../util/memory/Deleter.hpp"

namespace page { namespace res
{
////////// PostLoader //////////////////////////////////////////////////////////

	/**
	 * A function that is called after loading a type, which will perform
	 * whatever additional initialization is necessary.
	 *
	 * @note If the function is initialized to @c nullptr, calling it will do
	 * nothing.
	 */
	class PostLoader
	{
		friend class TypeRegistryRecord;

		public:
		typedef void result_type;

		private:
		template <typename T>
			PostLoader(const std::function<void (T &)> &);

		public:
		template <typename T>
			void operator ()(T &) const;

		void operator ()(void *) const;

		explicit operator bool() const;

		private:
		class Referenceable;
		std::function<void (Referenceable &)> f;
	};

////////// TypeRegistryRecord //////////////////////////////////////////////////

	/**
	 * A data structure containing information about a type.
	 */
	struct TypeRegistryRecord
	{
		TypeRegistryRecord(
			std::string         const& name,
			PostLoader          const& postLoader,
			util::Deleter<void> const& deleter);

		/**
		 * The name of the type.
		 */
		std::string name;

		/**
		 * @copydoc PostLoader
		 */
		PostLoader postLoader;

		/**
		 * @copydoc util::Deleter
		 */
		util::Deleter<void> deleter;
	};

////////// TypeRegistry ////////////////////////////////////////////////////////

	/**
	 * A place for registering types.
	 *
	 * @addtogroup registry
	 */
	class TypeRegistry : public util::Monostate<TypeRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef TypeRegistryRecord Record;

		public:
		/**
		 * Registers a type.
		 */
		template <typename T>
			void Register(
				std::string                     const& name,
				std::function<void (T &)>       const& postLoader = nullptr,
				std::function<void (const T *)> const& deleter    = util::DefaultDeleter<T>());

		private:
		/**
		 * @copydoc Register
		 */
		void Register(const std::type_info &, const Record &);

		public:
		/**
		 * Returns the record associated with the specified type.
		 */
		template <typename T>
			const Record &Query() const;

		/**
		 * @copydoc Query
		 */
		const Record &Query(const std::type_info &) const;

		private:
		std::unordered_map<std::type_index, Record> records;
	};

////////////////////////////////////////////////////////////////////////////////

	template <typename T>
		struct is_resource : std::false_type {};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c Registry.
	 */
#	define REGISTER_TYPE(T, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::TypeRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		} \
		template <> \
			struct is_resource<T> : std::true_type {};

#	include "TypeRegistry.tpp"
#endif
