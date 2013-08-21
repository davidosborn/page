#include "../../err/Exception.hpp"
#include "../Signature.hpp"
#include "BasicProxy.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		Proxy<T>::Proxy(const BasicProxy<T> &impl) :
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
		const Signature &Proxy<T>::DoGetSignature() const noexcept
	{
		return impl ? impl->GetSignature() : Signature();
	}
}}
