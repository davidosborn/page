namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		BasicProxy<T>::BasicProxy(const std::string &signature) :
			signature(signature) {}

	/*------------------------------+
	| ProxyInterface implementation |
	+------------------------------*/

	template <typename T>
		const std::string &BasicProxy<T>::DoGetSignature() const noexcept
	{
		return signature;
	}
}}
