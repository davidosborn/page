#ifndef    page_local_res_Index_hpp
#   define page_local_res_Index_hpp

#	include <array>
#	include <deque>
#	include <functional> // reference_wrapper
#	include <memory> // shared_ptr
#	include <string>
#	include <typeinfo> // type_info

#	include "../util/class/Monostate.hpp"

namespace page { namespace res
{
	class Source;
	class Stream;

	/**
	 * Provides access to resources through an ordered list of sources.
	 *
	 * @sa Source
	 */
	class Index : public util::Monostate<Index>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * Builds the initial index by scanning the configured sources.
		 */
		Index();

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Adds a new source to the index.
		 *
		 * The source is scanned and any resources found will take precedence
		 * over the existing ones if they share the same path.
		 */
		void AddSource(const std::string &path);

		/**
		 * Updates the index to match any changes that may have occurred on
		 * disk.
		 */
		void Refresh();

		/*----------------+
		| resource access |
		+----------------*/

		/**
		 * Provides stream-level access to the indexed resources.
		 */
		Stream *Open(const std::string &path) const;

		/**
		 * Attempts to load a resource of the specified type at the given path.
		 */
		std::shared_ptr<const void> Load(const std::type_info &, const std::string &path) const;

		/**
		 * Attempts to load a resource of the specified type at the given path.
		 */
		template <typename T>
			std::shared_ptr<const T> Load(const std::string &path) const;

		/**
		 * Returns the raw content of a resource as a string.
		 */
		std::string LoadString(const std::string &path) const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		typedef std::deque<std::shared_ptr<Source>> Sources;
		Sources sources;
	};
}}

#	include "Index.tpp"
#endif
