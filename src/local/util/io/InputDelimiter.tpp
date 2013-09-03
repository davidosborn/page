#include <algorithm> // find
#include <cstring> // str{chr,len}
#include <iterator> // end, begin
#include <locale> // isspace

namespace page
{
	namespace util
	{
		/*-------------+
		| constructors |
		+-------------*/

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter() {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(Char c) :
				InputDelimiter([c](Char x) { return CharTraits::eq(x, c); }) {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(const Char *s) :
				InputDelimiter(
					std::strlen(s) ?
					[s](Char c) { return std::strchr(s, c); } :
					nullptr) {}

		template <typename Char, typename CharTraits> template <typename InputRange>
			InputDelimiter<Char, CharTraits>::InputDelimiter(InputRange range, ENABLE_IF_IMPL(is_range<InputRange>::value)) :
				InputDelimiter(
					std::begin(range) != std::end(range) ?
					[&range](Char c) { return std::find(std::begin(range), std::end(range), c) != std::end(range); } :
					nullptr) {}

		template <typename Char, typename CharTraits> template <typename InputIterator>
			InputDelimiter<Char, CharTraits>::InputDelimiter(InputIterator first, InputIterator last, ENABLE_IF_IMPL(is_iterator<InputIterator>::value)) :
				InputDelimiter(
					first != last ?
					[&first, &last](Char c) { return std::find(first, last, c) != last; } :
					nullptr) {}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::InputDelimiter(const Predicate &predicate) :
				predicate(predicate) {}

		/*----------+
		| observers |
		+----------*/

		template <typename Char, typename CharTraits>
			const typename InputDelimiter<Char, CharTraits>::Predicate &
			InputDelimiter<Char, CharTraits>::GetPredicate() const
		{
			return predicate;
		}

		template <typename Char, typename CharTraits>
			bool InputDelimiter<Char, CharTraits>::operator()(Char c) const
		{
			return predicate ? predicate(c) : false;
		}

		template <typename Char, typename CharTraits>
			InputDelimiter<Char, CharTraits>::operator bool() const
		{
			return predicate != nullptr;
		}

		/*----------------------+
		| predefined delimiters |
		+----------------------*/

		template <typename Char, typename CharTraits>
			const InputDelimiter<Char, CharTraits> &
			InputDelimiter<Char, CharTraits>::GetEmptyDelimiter()
		{
			static InputDelimiter r;
			return r;
		}

		template <typename Char, typename CharTraits>
			const InputDelimiter<Char, CharTraits> &
			InputDelimiter<Char, CharTraits>::GetSpaceDelimiter()
		{
			static InputDelimiter r([](Char c) { return std::isspace(c, std::locale()); });
			return r;
		}
	}
}