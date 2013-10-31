#include "../../res/Index.hpp" // Index::Load
#include "../../res/type/TypeRegistry.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		ResourceProxy<T>::ResourceProxy(std::nullptr_t) {}

	template <typename T>
		ResourceProxy<T>::ResourceProxy(const std::string &path) :
			BasicProxy<T>(!path.empty() ?
				Signature("resource", GLOBAL(res::TypeRegistry).Query<T>().name, path) :
				Signature()),
			path(path) {}

	/*---------------+
	| implementation |
	+---------------*/

	template <typename T>
		auto ResourceProxy<T>::DoLock() const -> pointer
	{
		return GLOBAL(res::Index).Load<T>(path);
	}
}}
