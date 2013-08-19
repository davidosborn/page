#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo
#include "../../util/functional/cast.hpp" // reinterpret_cast_function
#include "../../util/memory/VoidDeleterAdaptor.hpp"

namespace page { namespace res
{
////////// PostLoader //////////////////////////////////////////////////////////

	template <typename T>
		PostLoader::PostLoader(const std::function<void (T &)> &f) :
			f(std::bind(f, std::bind(
				util::reinterpret_cast_function<T &, Referenceable &>(),
				std::placeholders::_1))) {}

	template <typename T>
		void PostLoader::operator ()(T &t) const
	{
		(*this)(&t);
	}

////////// TypeRegistry ////////////////////////////////////////////////////////

	template <typename T>
		void TypeRegistry::Register(
			std::string                     const& name,
			std::function<void (T &)>       const& postLoader,
			std::function<void (const T *)> const& deleter)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(name, postLoader, util::VoidDeleterAdaptor<T>(deleter)));
	}

	template <typename T>
		const typename TypeRegistry::Record &TypeRegistry::Query() const
	{
		return Query(util::GetIncompleteTypeInfo<T>());
	}
}}
