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
