/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_res_source_Source_hpp
#   define page_local_res_source_Source_hpp

#	include <memory> // {weak,shared}_ptr
#	include <string>
#	include <typeinfo> // type_info
#	include <unordered_map>
#	include <vector>

#	include "../load/function.hpp" // LoadFunction
#	include "../node/Node.hpp"

namespace page { namespace res
{
	class Stream;

	/**
	 * An abstract representation of a collection of resources.
	 */
	class Source
	{
		/*----------------------------------------+
		| constructors & special member functions |
		+----------------------------------------*/

		public:
		explicit Source(const std::string &path);

		virtual ~Source() = default;

		/*----------------------------------+
		| to be overridden by derived class |
		+----------------------------------*/

		/**
		 * @return @c true if the source can handle the specified path.
		 */
		static bool CheckPath(const std::string &path);

		/**
		 * Updates the internal index to match any changes that may have
		 * occurred on disk.
		 */
		virtual void Refresh();

		protected:
		void Index(const Node &);
		void Clear(const std::string &group = "");

		public:
		// resource access
		Stream *Open(const std::string &path) const;
		std::shared_ptr<const void> Load(const std::type_info &, const std::string &path) const;

		private:
		class Group;

		// parsing
		const void *LoadFromDisk(const std::type_info &, const std::string &path) const;
		void ScanToBuildIndex(const Node &, Group &, const std::string &rootPath = "");

		// index
		struct Path
		{
			Node node;
			struct Type
			{
				LoadFunction loader;
				typedef std::weak_ptr<const void> Data;
				Data data;
			};
			typedef std::unordered_map<std::string, Type> Types;
			Types types;
		};
		typedef std::unordered_map<std::string, Path> Paths;
		/**
		 * Map each path string to information about the path.
		 *
		 * @note This variable is mutable to allow the @c Load function to
		 *       update it.
		 */
		mutable Paths paths;
		struct Group
		{
			typedef std::vector<Paths::iterator> PathPointers;
			PathPointers paths;
		};
		typedef std::unordered_map<std::string, Group> Groups;
		Groups groups;
	};
}}

#endif
