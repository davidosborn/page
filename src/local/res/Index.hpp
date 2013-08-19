#ifndef    page_local_res_Index_hpp
#   define page_local_res_Index_hpp

#	include <array>
#	include <deque>
#	include <functional> // reference_wrapper
#	include <memory> // shared_ptr
#	include <string>
#	include <typeinfo> // type_info

namespace page
{
	namespace res
	{
		class Source;
		class Stream;

		/**
		 * Provides access to resources through an ordered list of sources.
		 *
		 * @sa Source
		 */
		struct Index
		{
			// constructors
			/**
			 * Build the initial index by scanning the configured sources.
			 */
			Index();

			// modifiers
			/**
			 * Add a new source to the index.
			 *
			 * The source is scanned and any resources found will take
			 * precedence over the existing ones if they share the same path.
			 */
			void AddSource(const std::string &path);
			/**
			 * Update the index to match any changes that may have occurred on
			 * disk.
			 */
			void Refresh();

			// resource access
			/**
			 * Provide stream-level access to the indexed resources.
			 */
			Stream *Open(const std::string &path) const;
			/**
			 * Attempt to load a resource of the specified type at the given
			 * path.
			 */
			std::shared_ptr<const void> Load(const std::type_info &, const std::string &path) const;
			/**
			 * Attempt to load a resource of the specified type at the given
			 * path.
			 */
			template <typename T> std::shared_ptr<const T> Load(const std::string &path) const;
			/**
			 * Return the raw content of a resource as a string.
			 */
			std::string LoadString(const std::string &path) const;

			private:
			typedef std::deque<std::shared_ptr<Source>> Sources;
			Sources sources;
		};

		// global state
		/**
		 * Return the global index.
		 */
		Index &GetIndex();
	}
}

#	include "Index.tpp"
#endif
