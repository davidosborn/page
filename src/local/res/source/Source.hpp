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
