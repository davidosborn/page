#ifndef    page_local_res_save_SaverRegistry_hpp
#   define page_local_res_save_SaverRegistry_hpp

#	include <functional> // function
#	include <iosfwd> // ostream
#	include <list>
#	include <memory> // unique_ptr
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

////////// Saver ///////////////////////////////////////////////////////////////

	/**
	 * A function that forms the implementation of a saver.
	 *
	 * @note If the function is initialized to @c nullptr, calling it will do
	 * nothing.
	 */
	class Saver
	{
		public:
		typedef void result_type;

		template <typename T>
			Saver(const std::function<void (const T &, std::ostream &)> &);

		public:
		template <typename T>
			void operator ()(const T &, std::ostream &) const;

		void operator ()(const void *, std::ostream &) const;

		explicit operator bool() const;

		private:
		class Referenceable;
		std::function<void (const Referenceable &, std::ostream &)> f;
	};

////////// SaverRegistryRecord /////////////////////////////////////////////////

	/**
	 * A data structure containing information about a saver.
	 */
	struct SaverRegistryRecord
	{
		/**
		 * Constructor.
		 *
		 * @note The first extension in @c extensions will become the value of
		 *       @c defaultExtension.
		 */
		SaverRegistryRecord(
			std::string              const& name,
			Saver                    const& saver,
			std::vector<std::string> const& formats          = {},
			std::vector<std::string> const& extensions       = {},
			bool                            binary           = true,
			int                             priority         = 0);

		/**
		 * The name of the saver.
		 */
		std::string name;

		/**
		 * @copydoc Saver
		 */
		Saver saver;

		/**
		 * The formats that this saver recognizes.
		 */
		std::unordered_set<std::string> formats;

		/**
		 * The extensions that this saver recognizes.
		 */
		std::unordered_set<std::string> extensions;

		/**
		 * The default extension to append to a path when it doesn't already
		 * have a matching extension.
		 */
		std::string defaultExtension;

		/**
		 * @c true if the file should be opened in binary mode, as opposed to
		 * text mode.
		 */
		bool binary;

		/**
		 * A higher priority means that this saver will be selected before other
		 * savers of the same level with a lower priority.
		 */
		int priority;
	};

////////// SaverRegistry ///////////////////////////////////////////////////////

	/**
	 * A place for registering savers.
	 *
	 * @addtogroup registry
	 */
	class SaverRegistry : public util::Monostate<SaverRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef SaverRegistryRecord Record;

		public:
		/**
		 * Registers a saver.
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
		 * Selects the saver that best matches the given arguments, and uses it
		 * to write the resource to a file.
		 *
		 * @return The path of the file that the resource was written to.
		 */
		template <typename T>
			std::string Save(
				T           const& resource,
				std::string        path,
				std::string const& format    = "",
				bool               bestMatch = true) const;

		private:
		/**
		 * @copydoc Save
		 */
		std::string Save(
			std::type_info const& type,
			void           const* resource,
			std::string           path,
			std::string    const& format    = "",
			bool                  bestMatch = true) const;

		public:
		/**
		 * Returns the saver that best matches the given arguments.
		 *
		 * If @c extension or @c format is empty, it will not be considered as
		 * criteria during the search.
		 *
		 * @note If @c bestMatch is @c true, the @em closest-matching saver will
		 *       be returned, even if it doesn't exactly match any of the given
		 *       arguments.  Otherwise, only those savers which exactly match
		 *       the given arguments will be considered.
		 */
		template <typename T>
			const Record &GetSaver(
				std::string const& format    = "",
				std::string const& extension = "",
				bool               bestMatch = true) const;

		private:
		/**
		 * @copydoc GetSaver
		 */
		const Record &GetSaver(
			std::type_info const& type,
			std::string    const& format    = "",
			std::string    const& extension = "",
			bool                  bestMatch = true) const;

		private:
		struct TypeRecord
		{
			std::list<Record> records;
			std::unordered_map<std::string, std::list<typename decltype(records)::const_iterator>> formats;
			std::unordered_map<std::string, std::list<typename decltype(records)::const_iterator>> extensions;
		};
		std::unordered_map<std::type_index, TypeRecord> types;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a saver with @c SaverRegistry.
	 */
#	define REGISTER_SAVER(T, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::SaverRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "SaverRegistry.tpp"
#endif
