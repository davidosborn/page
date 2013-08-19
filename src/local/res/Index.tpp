#include "../util/class/typeinfo.hpp" // GetIncompleteTypeInfo

namespace page
{
	namespace res
	{
		// resource access
		template <typename T> std::shared_ptr<const T> Index::Load(const std::string &path) const
		{
			return std::static_pointer_cast<const T>(Load(util::GetIncompleteTypeInfo<T>(), path));
		}
	}
}
