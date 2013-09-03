#include <cassert>

#include "State.hpp" // State::{Get,Set,Unset}Var

namespace page { namespace cfg
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T, typename ExternT>
		Var<T, ExternT>::Var(
			BasicState             &state,
			const std::string      &key,
			const T                &defaultValue,
			const GetFilter        &getFilter,
			const SetFilter        &setFilter,
			const ConvertInFilter  &convertInFilter,
			const ConvertOutFilter &convertOutFilter) :
				BasicVar(state, key),
				value(defaultValue)
				defaultValue(defaultValue),
				getFilter       (getFilter        ? getFilter        : DefaultGetFilter()),
				setFilter       (setFilter        ? setFilter        : DefaultSetFilter()),
				convertInFilter (convertInFilter  ? convertInFilter  : DefaultConvertInFilter()),
				convertOutFilter(convertOutFilter ? convertOutFilter : DefaultConvertOutFilter()) {}

	/*----------+
	| observers |
	+----------*/

	template <typename T, typename ExternT>
		T Var<T, ExternT>::operator *() const
	{
		return Get();
	}

	template <typename T, typename ExternT>
		util::ArrowProxy<T> Var<T, ExternT>::operator ->() const
	{
		return Get();
	}

	template <typename T, typename ExternT>
		T Var<T, ExternT>::Get() const
	{
		return getFilter(value);
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename T, typename ExternT>
		Var<T, ExternT> &Var<T, ExternT>::operator =(const T &value)
	{
		Set(value);
		return *this;
	}

	template <typename T, typename ExternT>
		void Var<T, ExternT>::Set(const T &value)
	{
		this->value = setFilter(value);
		modified = true;
	}

	template <typename T, typename ExternT>
		void Var<T, ExternT>::Unset()
	{
		Set(defaultValue);
	}

	/*--------------+
	| serialization |
	+--------------*/

	template <typename T, typename ExternT>
		void Var<T, ExternT>::Serialize(std::ostream &os) const
	{
		os << convertOutFilter(Get());
	}
	template <typename T, typename ExternT>
		void Var<T, ExternT>::Serialize(std::wostream &os) const
	{
		os << convertOutFilter(Get());
	}

	template <typename T, typename ExternT>
		void Var<T, ExternT>::Deserialize(std::istream &is)
	{
		ExternT value;
		is >> value;
		Set(convertInFilter(value));
	}
	template <typename T, typename ExternT>
		void Var<T, ExternT>::Deserialize(std::wistream &is)
	{
		ExternT value;
		is >> value;
		Set(convertInFilter(value));
	}

	/*----------------+
	| default filters |
	+----------------*/

	template <typename T, typename ExternT>
		struct Var<T, ExternT>::DefaultGetFilter
	{
		T operator ()(const T &value)
		{
			return value;
		}
	};

	template <typename T, typename ExternT>
		struct Var<T, ExternT>::DefaultSetFilter
	{
		T operator ()(const T &value)
		{
			return value;
		}
	};

	template <typename T, typename ExternT>
		struct Var<T, ExternT>::DefaultConvertInFilter
	{
		T operator ()(const ExternT &value)
		{
			return value;
		}
	};

	template <typename T, typename ExternT>
		struct Var<T, ExternT>::DefaultConvertOutFilter
	{
		ExternT operator ()(const T &value)
		{
			return value;
		}
	};
}}
