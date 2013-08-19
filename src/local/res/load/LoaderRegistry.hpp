#ifndef    page_local_res_load_LoaderRegistry_hpp
#   define page_local_res_load_LoaderRegistry_hpp

#	include <forward_list>
#	include <functional> // function
#	include <memory> // {shared,unique}_ptr
#	include <string>
#	include <typeinfo> // type_info
#	include <unordered_map>
#	include <unordered_set>

#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace res
{
	class Node;
	class Pipe;

	/**
	 * A pointer to a function that forms the implementation of a loader.
	 */
	using Loader = std::function<std::unique_ptr<void> (const std::shared_ptr<const Pipe> &)>;

	/**
	 * A pointer to a function that, when called, will return @c true if the
	 * format of the specified pipe is compatible with the loader.
	 */
	using LoaderCompatibleFunction = std::function<bool (const Pipe &)>;

////////// LoaderRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure containing information about a loader.
	 */
	struct LoaderRegistryRecord
	{
		/**
		 * Permits @c LoaderRegistry to access @c tried.
		 */
		friend class LoaderRegistry;

		LoaderRegistryRecord(
			std::string              const& name,
			Loader                   const& loader,
			LoaderCompatibleFunction const& compatibleFunction,
			std::vector<std::string> const& mimeTypes  = {},
			std::vector<std::string> const& extensions = {},
			bool                            inspect    = true,
			int                             priority   = 0);

		/**
		 * The name of the loader.
		 */
		std::string name;

		/**
		 * @copydoc Loader
		 */
		Loader loader;

		/**
		 * @copydoc LoaderCompatibleFunction
		 */
		LoaderCompatibleFunction compatibleFunction;

		/**
		 * The mime types that this loader recognizes.
		 */
		std::unordered_set<std::string> mimeTypes;

		/**
		 * The extensions that this loader recognizes.
		 */
		std::unordered_set<std::string> extensions;

		/**
		 * @c true if this loader should be tried as a final resort, even if
		 * none of its mime types or extensions match the node.
		 */
		bool inspect;

		/**
		 * A higher priority means that this loader will be tried before other
		 * loaders of the same level with a lower priority.
		 */
		int priority;

		private:
		/**
		 * A marker used when searching for a compatible loader, which will be
		 * @c true if this loader has already been tried.
		 *
		 * @note This member has no definite value outside
		 *       @c LoaderRegistry::GetLoader.
		 */
		bool mutable tried;
	};

////////// LoaderRegistry //////////////////////////////////////////////////////

	/**
	 * A place for registering loaders.
	 */
	class LoaderRegistry : public util::Monostate<LoaderRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef LoaderRegistryRecord Record;

		public:
		/**
		 * Registers a loader.
		 */
		template <typename T, typename... RecordArgs>
			void Register(RecordArgs &&...);

		private:
		/**
		 * @copydoc Register
		 */
		void Register(const std::type_info &, const Record &);

		public:
		/**
		 * Searches for a loader that is compatible with the given node.
		 *
		 * @return A compatible loader, or @c nullptr if none was found.
		 */
		template <typename T>
			const Loader &GetLoader(const Node &) const;

		/**
		 * @copydoc GetLoader
		 */
		const Loader &GetLoader(const std::type_info &, const Node &) const;

		private:
		struct TypeRecord
		{
			std::forward_list<Record> records;
			std::unordered_map<std::string, std::forward_list<typename decltype(records)::const_iterator>> mimeTypes;
			std::unordered_map<std::string, std::forward_list<typename decltype(records)::const_iterator>> extensions;
		};
		std::unordered_map<std::type_index, TypeRecord> types;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a loader with @c LoaderRegistry.
	 */
#	define REGISTER_LOADER(T, ...) \
		namespace \
		{ \
			struct Initializer() \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::LoaderRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "LoaderRegistry.tpp"
#endif
