#ifndef    page_local_cache_Signature_hpp
#   define page_local_cache_Signature_hpp

#	include <string>
#	include <utility> // forward

#	include "../util/string/operations.hpp" // Join

namespace page { namespace cache
{
	/**
	 * A string wrapper that uniquely identifies a resource based on its type
	 * and source.
	 *
	 * The signature is formatted like a function call, in the following manner:
	 * <tt>type(sources...)</tt>.
	 *
	 * The sources are comma-separated values, where each value represents a
	 * parameter involved in acquiring the resource, which together define its
	 * source.  There should be no extraneous whitespace between values.  If a
	 * parameter is a resource, it should be represented by its signature.
	 */
	class Signature : public std::string
	{
		public:
		template <typename... Args>
			Signature(const std::string &type, Args &&... args) :
				std::string(util::Join(type, std::forward<Args>(args)...)) {}
	};
}}

#endif
