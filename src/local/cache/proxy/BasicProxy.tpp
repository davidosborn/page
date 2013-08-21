namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		BasicProxy<T>::BasicProxy(const Signature &signature) :
			signature(signature) {}

	/*------------------------------+
	| ProxyInterface implementation |
	+------------------------------*/

	template <typename T>
		const Signature &BasicProxy<T>::DoGetSignature() const noexcept
	{
		return signature;
	}
}}
