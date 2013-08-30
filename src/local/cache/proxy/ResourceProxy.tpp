#include "../../res/Index.hpp" // GetIndex, Index::Load
#include "../../res/type/TypeRegistry.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		ResourceProxy<T>::ResourceProxy(const std::string &path) :
			BasicProxy<T>(Signature(
				SigType("resource", GLOBAL(res::TypeRegistry).Query<T>().name),
				path)),
			path(path) {}

	/*---------------+
	| implementation |
	+---------------*/

	template <typename T>
		auto ResourceProxy<T>::DoLock() const -> pointer
	{
		return res::GetIndex().Load<T>(path);
	}
}}
