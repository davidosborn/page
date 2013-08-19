namespace page { namespace util
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	template <typename D>
		StateSaver<D>::~StateSaver()
	{
		Restore();
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename D>
		void StateSaver<D>::Release()
	{
		enabled = false;
	}

	template <typename D>
		void StateSaver<D>::Restore()
	{
		if (enabled) static_cast<D &>(*this).Load();
	}

	template <typename D>
		void StateSaver<D>::Reset(bool enabled)
	{
		if (enabled) static_cast<D &>(*this).Save();
		else Restore();
		this->enabled = enabled;
	}
}}
