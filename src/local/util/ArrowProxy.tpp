namespace page { namespace util
{
	template <typename T>
		ArrowProxy<T>::ArrowProxy(const T &value) :
			value(value) {}

	template <typename T>
		const T *ArrowProxy<T>::operator ->() const
	{
		return &value;
	}
}}
