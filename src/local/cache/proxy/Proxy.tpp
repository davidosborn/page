#include "../../err/Exception.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		Proxy<T>::Proxy(const ProxyImpl<T> &impl) :
			impl(impl.Clone()) {}

	/*------------------------------+
	| ProxyInterface implementation |
	+------------------------------*/

	template <typename T>
		auto Proxy<T>::DoLock() const -> pointer
	{
		return impl ? impl->DoLock() : nullptr;
	}

	template <typename T>
		const std::string &Proxy<T>::DoGetSignature() const noexcept
	{
		return impl ? impl->GetSignature() : "";
	}
}}
