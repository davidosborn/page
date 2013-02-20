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

#ifndef    page_local_err_Tag_hpp
#   define page_local_err_Tag_hpp

#	include <algorithm> // copy
#	include <ostream> // basic_ostream
#	include <vector>

#	include <boost/mpl/back_inserter.hpp>
#	include <boost/mpl/copy.hpp>
#	include <boost/mpl/count_if.hpp>
#	include <boost/mpl/empty_sequence.hpp>
#	include <boost/mpl/end.hpp>
#	include <boost/mpl/eval_if.hpp>
#	include <boost/mpl/for_each.hpp>
#	include <boost/mpl/insert_range.hpp>
#	include <boost/mpl/inserter.hpp>
#	include <boost/mpl/not.hpp>
#	include <boost/mpl/single_view.hpp>
#	include <boost/mpl/sort.hpp>
#	include <boost/mpl/transform.hpp>
#	include <boost/mpl/unique.hpp>
#	include <boost/mpl/unpack.hpp>
#	include <boost/mpl/vector.hpp>

#	include "../util/io/separated_ostream_iterator.hpp"
#	include "../util/type_traits/sfinae.hpp" // DEFINE_SFINAE_TYPE_TRAIT_1

namespace page
{
	namespace err
	{
		template <int> struct Tag;
		template <typename...> struct TagSet;

////////////////////////////////////////////////////////////////////////////////

		namespace detail
		{
			/*------------+
			| type traits |
			+------------*/

			/**
			 * @return @c true if @c T is derived from @c Tag.
			 */
			DEFINE_SFINAE_TYPE_TRAIT_1(
				IsTag,
				typename std::enable_if<(
					std::is_base_of<Tag<T::id>, T>::value
					)>::type)

			/**
			 * @return @c true if @c T is derived from @c TagSet.
			 */
			DEFINE_SFINAE_TYPE_TRAIT_1(
				IsTagSet,
				typename std::enable_if<(
					std::is_base_of<
						typename boost::mpl::unpack<
							TagSet
							>::apply<
								typename T::_OrigTags
								>::type,
						T
						>::value
					)>::type)

			/*-------------+
			| tag ordering |
			+-------------*/

			/**
			 * A binary metafunction implementing an ordering relation for tags.
			 *
			 * @note Designed to be use with @c boost::mpl::sort to sort a
			 *       sequence of tags.
			 */
			struct LessTags
			{
				template <typename A, typename B>
					struct apply
				{
					struct type
					{
						static constexpr int value = A::id < B::id;
					};
				};
			};

			/**
			 * A binary metafunction implementing an equivalence relation for
			 * tags.
			 *
			 * @note Designed to be used with @c boost::mpl::unique to remove
			 *       duplicate tags from a sequence of tags.
			 */
			struct EqualTags
			{
				template <typename A, typename B>
					struct apply
				{
					struct type
					{
						static constexpr int value = A::id == B::id;
					};
				};
			};

			/*---------------------+
			| multi-tag processing |
			+---------------------*/

			/**
			 * @return @c TagSet::Tags.
			 */
			template <typename TagSet>
				struct GetTagSetTags
			{
				typedef typename TagSet::Tags type;
			};

			/**
			 * Recursively expands nested sequences of tags to produce a single
			 * flat sequence of tags.
			 *
			 * @note The outer sequence is represented by a Boost/MPL sequence,
			 *       while nested sequences are represented by types derived
			 *       from @c TagSet.
			 */
			template <typename Tags>
				struct FlattenTags
			{
				static_assert(
					boost::mpl::is_sequence<Tags>::value,
					"expected Boost/MPL sequence");

				typedef
					typename boost::mpl::transform<
						Tags,
						boost::mpl::eval_if<
							IsTagSet<boost::mpl::_1>,
							boost::mpl::copy<
								FlattenTags<
									GetTagSetTags<boost::mpl::_1>
									>,
								boost::mpl::back_inserter<
									boost::mpl::eval_if<
										IsTag<boost::mpl::_1>,
										boost::mpl::copy<
											boost::mpl::single_view<boost::mpl::_1>,
											boost::mpl::back_inserter<
												boost::mpl::vector<>
												>
											>,
										boost::mpl::vector<>
										>
									>
								>,
							boost::mpl::single_view<boost::mpl::_1>
							>,
						boost::mpl::inserter<
							boost::mpl::vector<>,
							boost::mpl::insert_range<
								boost::mpl::_1,
								boost::mpl::end<boost::mpl::_1>,
								boost::mpl::_2>>
						>::type
							type;
			};

			/**
			 * Process a variadic sequence of @c Tag and @c TagSet types to
			 * produce a flattened and sorted Boost/MPL sequence of distinct
			 * tags.
			 */
			template <typename... Tags>
				struct CleanTags
			{
				private:
				typedef
					typename FlattenTags<
						boost::mpl::vector<Tags...>
						>::type FlattenedTags;

				static_assert(
					boost::mpl::count_if<
						FlattenedTags,
						boost::mpl::not_<
							IsTag<boost::mpl::_1>
							>
						>::value == 0,
					"at least one element is not a tag");

				public:
				typedef
					typename boost::mpl::unique<
						typename boost::mpl::sort<
							FlattenedTags,
							LessTags
							>::type,
						EqualTags
						>::type type;
			};
		}

////////////////////////////////////////////////////////////////////////////////

		/*------------+
		| tag classes |
		+------------*/

		/**
		 * The base class for tags.
		 */
		template <int _id>
			struct Tag
		{
			/**
			 * A unique ID for ordering.
			 */
			static constexpr int id = _id;

			/**
			 * The name of the tag.
			 */
			static const char *name;
		};

		/**
		 * A container for referencing multiple tags.
		 */
		template <typename... _Tags>
			struct TagSet
		{
			typedef typename detail::CleanTags<_Tags...>::type Tags;

			typedef boost::mpl::vector<_Tags...> _OrigTags;
		};

////////////////////////////////////////////////////////////////////////////////

		/*----------------+
		| runtime support |
		+----------------*/

		/**
		 * A runtime version of @c Tag.
		 */
		struct RuntimeTag
		{
			template <typename Tag>
				explicit RuntimeTag(Tag) :
					id  (Tag::id),
					name(Tag::name)
			{
				static_assert(
					detail::IsTag<Tag>::value,
					"expected Tag");
			}

			int id;
			const char *name;
		};

		/**
		 * A function object which takes a tag, converts it to a @c RuntimeTag,
		 * and writes it to an output iterator.
		 *
		 * @note Designed for use with @c boost::mpl::for_each.
		 */
		template <typename OutputIterator>
			class TagWriter
		{
			public:
			explicit TagWriter(OutputIterator iter) :
				iter(iter) {}

			template <typename Tag>
				void operator ()(Tag tag)
			{
				*iter++ = RuntimeTag(tag);
			}

			private:
			OutputIterator iter;
		};

		/**
		 * A runtime version of @c TagSet.
		 */
		struct RuntimeTagSet
		{
			template <typename TagSet>
				explicit RuntimeTagSet(TagSet)
			{
				auto inserter(std::back_inserter(tags));
				boost::mpl::for_each<typename TagSet::Tags>(
					TagWriter<decltype(inserter)>(inserter));
			}

			std::vector<RuntimeTag> tags;
		};

		/**
		 * A stream inserter for @c RuntimeTag.
		 */
		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &
				operator <<(
					std::basic_ostream<Char, CharTraits> &os,
					const RuntimeTag &tag)
		{
			return os << tag.name << '(' << tag.id << ')';
		}

		/**
		 * A stream inserter for @c RuntimeTagSet.
		 */
		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &
				operator <<(
					std::basic_ostream<Char, CharTraits> &os,
					const RuntimeTagSet &tagSet)
		{
			std::copy(
				tagSet.tags.begin(),
				tagSet.tags.end(),
				util::separated_ostream_iterator<RuntimeTag, Char, CharTraits>(os, ','));
			return os;
		}
	}
}

#endif
