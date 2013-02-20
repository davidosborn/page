/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>

#include "State.hpp" // State::{Get,Set,Unset}Var

namespace page
{
	namespace cfg
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T, typename ExternT>
			Var<T, ExternT>::Var(
				BasicState             &state,
				const std::string      &key,
				const T                &defaultValue,
				const LimitFilter      &limitFilter,
				const ConvertInFilter  &convertInFilter,
				const ConvertOutFilter &convertOutFilter) :
					BasicVar(state, key),
					value(defaultValue)
					defaultValue(defaultValue),
					limitFilter     (limitFilter      ? limitFilter      : DefaultLimitFilter()),
					convertInFilter (convertInFilter  ? convertInFilter  : DefaultConvertInFilter()),
					convertOutFilter(convertOutFilter ? convertOutFilter : DefaultConvertOutFilter()) {}

		/*----------+
		| observers |
		+----------*/

		template <typename T, typename ExternT>
			const T &Var<T, ExternT>::operator *() const noexcept
		{
			return Get();
		}

		template <typename T, typename ExternT>
			const T *Var<T, ExternT>::operator ->() const noexcept
		{
			return &Get();
		}

		template <typename T, typename ExternT>
			const T &Var<T, ExternT>::Get() const noexcept
		{
			return value;
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
			this->value = value;
			modified = true;
		}

		template <typename T, typename ExternT>
			void Var<T, ExternT>::Unset()
		{
			value = defaultValue;
			modified = true;
		}

		/*------------------+
		| (de)serialization |
		+------------------*/

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
			struct Var<T, ExternT>::DefaultLimitFilter
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
	}
}
