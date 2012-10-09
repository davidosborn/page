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
#include <istream>
#include <iterator> // end, inserter
#include <ostream>
#include <type_traits> // is_convertible
#include <vector>
#include "../err/exception/throw.hpp" // THROW
#include "../util/serialize/deserialize.hpp" // Deserialize
#include "../util/serialize/serialize.hpp" // Serialize
#include "../util/type_traits.hpp" // ENABLE_IF, is_{range,sequence_container}
#include "Source.hpp" // Reader, Writer

namespace page
{
	namespace cfg
	{
		namespace detail
		{
			/**
			 * @defgroup cfg-var-serialize-wrapper
			 * @ingroup  cfg-var-serialize
			 *
			 * These (de)serialization functions are simple wrappers around the
			 * standard (de)serialization functions, found in
			 * "util/serialize/serialize.hpp" and the surrounding source files,
			 * which change the default delimiter for sequences to a
			 * non-collapsing comma.
			 *
			 * @{
			 */
			template <typename Char, typename CharTraits, typename T>
				void Serialize(
					std::basic_ostream<Char, CharTraits> &os,
					const T &value,
					ENABLE_IF(!util::is_serializable_as_sequence<T>::value))
			{
				if (util::Serialize(os, value).fail())
					THROW err::Exception<err::SerializationTag>();
			}
			template <typename Char, typename CharTraits, typename Range>
				void Serialize(
					std::basic_ostream<Char, CharTraits> &os,
					const Range &range,
					ENABLE_IF(util::is_serializable_as_sequence<Range>::value))
			{
				if (util::Serialize(os, range, ',').fail())
					THROW err::Exception<err::SerializationTag>();
			}
			template <typename Char, typename CharTraits, typename T>
				void Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					T &value,
					ENABLE_IF(!util::is_deserializable_as_sequence<T>::value))
			{
				if (util::Deserialize(is, value).fail())
					THROW err::Exception<err::DeserializationTag>();
			}
			template <typename Char, typename CharTraits, typename Range>
				void Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					Range &range,
					ENABLE_IF(util::is_deserializable_as_sequence<Range>::value))
			{
				if (util::Deserialize(is, range, util::SequenceDeserializationFlags::none, ',').fail())
					THROW err::Exception<err::DeserializationTag>();
			}
			///@}

			/**
			 * @defgroup cfg-var-filter
			 * @{
			 */
			template <typename From, typename To,
				bool = std::is_convertible<From, To>::value>
				struct DefaultConversionImpl;
			template <typename From, typename To>
				struct DefaultConversionImpl<From, To, true>
			{
				static To Apply(const From &value)
				{
					return value;
				}
			};
			template <typename From, typename To>
				struct DefaultConversionImpl<From, To, false>
			{
				static To Apply(const From &)
				{
					assert(!"invalid conversion");
				}
			};
			/**
			 * A helper for implementing the default conversion-filters in
			 * @c Var.
			 *
			 * @sa Var::DefaultConvertInFilter
			 * @sa Var::DefaultConvertOutFilter
			 */
			template <typename From, typename To>
				struct DefaultConversion : DefaultConversionImpl<From, To> {};
			///@}

			//////////////////////
			// BasicVarIterator //
			//////////////////////

			// constructors
			template <typename T> BasicVarIterator<T>::BasicVarIterator(T *p) :
				Base(p) {}
			template <typename T> template <typename U> BasicVarIterator<T>::BasicVarIterator(const BasicVarIterator<U> &other) :
				Base(other) {}

			// arithmetic
			template <typename T> void BasicVarIterator<T>::Increment()
			{
				this->iter = this->iter->next;
			}
			template <typename T> void BasicVarIterator<T>::Decrement()
			{
				this->iter = this->iter->prev;
			}
		}
		
		//////////////
		// BasicVar //
		//////////////
		
		// serialization
		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &BasicVar::Serialize(std::basic_ostream<Char, CharTraits> &os, Domain domain) const
		{
			DoSerialize(os, domain);
			return os;
		}
		template <typename Char, typename CharTraits>
			std::basic_istream<Char, CharTraits> &BasicVar::Deserialize(std::basic_istream<Char, CharTraits> &is, Domain domain)
		{
			DoDeserialize(is, domain);
			return is;
		}

		/////////
		// Var //
		/////////
		
		// default filters
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
				return detail::DefaultConversion<ExternT, T>::Apply(value);
			}
		};
		template <typename T, typename ExternT>
			struct Var<T, ExternT>::DefaultConvertOutFilter
		{
			ExternT operator ()(const T &value)
			{
				return detail::DefaultConversion<T, ExternT>::Apply(value);
			}
		};
		
		// constructor
		template <typename T, typename ExternT>
			Var<T, ExternT>::Var(const std::string &name, const T &defaultValue, const LimitFilter &limitFilter, const ConvertInFilter &convertInFilter, const ConvertOutFilter &convertOutFilter) :
			BasicVar(name), defaultValue(defaultValue),
			limitFilter     (limitFilter      ? limitFilter      : DefaultLimitFilter()),
			convertInFilter (convertInFilter  ? convertInFilter  : DefaultConvertInFilter()),
			convertOutFilter(convertOutFilter ? convertOutFilter : DefaultConvertOutFilter()) {}

		// observers
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
			const T &Var<T, ExternT>::Get(Domain domain) const noexcept
		{
			switch (domain == autoDomain ? GetDomain() : domain)
			{
				case autoDomain:
				case userDomain: if (userSet) return userValue;
				case siteDomain: if (siteSet) return siteValue;
				case defaultDomain: return defaultValue;
				default: assert(!"invalid domain");
			}
		}
		
		// modifiers
		template <typename T, typename ExternT>
			Var<T, ExternT> &Var<T, ExternT>::operator =(const T &value)
		{
			Set(value);
			return *this;
		}
		template <typename T, typename ExternT>
			void Var<T, ExternT>::Set(const T &value, Domain domain)
		{
			switch (domain == autoDomain ? GetDomain() : domain)
			{
				case autoDomain:
				if (userSet || !siteSet)
				{
					UserDomain:
					userValue = limitFilter(value);
					userSet = true;
					userMod = true;
					userEverMod = true;
				}
				else
				{
					SiteDomain:
					siteValue = limitFilter(value);
					siteSet = true;
					siteMod = true;
				}
				break;
				case userDomain: goto UserDomain;
				case siteDomain: goto SiteDomain;
				case defaultDomain: assert(!"default domain is read-only");
				default: assert(!"invalid domain");
			}
		}

		// revision control
		template <typename T, typename ExternT>
			bool Var<T, ExternT>::Update(Reader &reader, Domain domain)
		{
			switch (domain == autoDomain ? GetDomain() : domain)
			{
				case autoDomain:
				if (userSet || !siteSet)
				{
					UserDomain:
					ExternT value;
					if (Read(reader, GetName(), value))
					{
						userValue = limitFilter(convertInFilter(value));
						userSet = true;
						userMod = false;
						return true;
					}
				}
				else
				{
					SiteDomain:
					ExternT value;
					if (Read(reader, GetName(), value))
					{
						siteValue = limitFilter(convertInFilter(value));
						siteSet = true;
						siteMod = false;
						return true;
					}
				}
				break;
				case userDomain: goto UserDomain;
				case siteDomain: goto SiteDomain;
				case defaultDomain: return true;
				default: assert(!"invalid domain");
			}
			return false;
		}
		template <typename T, typename ExternT>
			void Var<T, ExternT>::Commit(Writer &writer, Domain domain) const
		{
			switch (domain == autoDomain ? GetDomain() : domain)
			{
				case autoDomain:
				if (userMod)
				{
					UserDomain:
					if (userSet) Write(writer, GetName(), convertOutFilter(userValue));
					else writer.Remove(GetName());
					userMod = false;
				}
				else if (siteMod)
				{
					SiteDomain:
					if (siteSet) Write(writer, GetName(), convertOutFilter(siteValue));
					else writer.Remove(GetName());
					siteMod = false;
				}
				break;
				case userDomain: if (userMod) goto UserDomain; break;
				case siteDomain: if (siteMod) goto SiteDomain; break;
				case defaultDomain: assert(!"default domain is read-only");
				default: assert(!"invalid domain");
			}
		}

		// serialization implementation (level 1)
		template <typename T, typename ExternT>
			void Var<T, ExternT>::DoSerialize(std::ostream &os, Domain domain) const
		{
			DoSerializeImpl(os, domain);
		}
		template <typename T, typename ExternT>
			void Var<T, ExternT>::DoSerialize(std::wostream &os, Domain domain) const
		{
			DoSerializeImpl(os, domain);
		}
		template <typename T, typename ExternT>
			void Var<T, ExternT>::DoDeserialize(std::istream &is, Domain domain)
		{
			DoDeserializeImpl(is, domain);
		}
		template <typename T, typename ExternT>
			void Var<T, ExternT>::DoDeserialize(std::wistream &is, Domain domain)
		{
			DoDeserializeImpl(is, domain);
		}
		
		// serialization implementation (level 2)
		template <typename T, typename ExternT> template <typename Char, typename CharTraits>
			void Var<T, ExternT>::DoSerializeImpl(std::basic_ostream<Char, CharTraits> &os, Domain domain) const
		{
			detail::Serialize(os, convertOutFilter(Get(domain)));
		}
		template <typename T, typename ExternT> template <typename Char, typename CharTraits>
			void Var<T, ExternT>::DoDeserializeImpl(std::basic_istream<Char, CharTraits> &is, Domain domain)
		{
			ExternT value;
			detail::Deserialize(is, value);
			Set(convertInFilter(value), domain);
		}

		// stream insertion/extraction
		template <typename T, typename ExternT, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Var<T, ExternT> &var)
		{
			return var.Serialize(os);
		}
		template <typename T, typename ExternT, typename Char, typename CharTraits>
			std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Var<T, ExternT> &var)
		{
			return var.Deserialize(is);
		}
	}
}
