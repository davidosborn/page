// stream line iterator
// counts lines and tracks indentation
// points to empty string at end of stream

#ifndef    page_local_res_adapt_text_LineIterator_hpp
#   define page_local_res_adapt_text_LineIterator_hpp

#	include <string>

namespace page
{
	namespace res
	{
		class Stream;

		struct LineIterator
		{
			typedef std::string value_type;
			typedef const value_type *pointer;
			typedef const value_type &reference;

			LineIterator(Stream &, char comment = '#');

			reference operator *() const;
			pointer operator ->() const;

			LineIterator &operator ++();
			LineIterator operator ++(int);

			explicit operator bool() const;

			unsigned Indentation() const;

			private:
			void Fetch();
			void Fetch() const;

			Stream &stream;
			char comment;
			std::string line, space;
			unsigned indentation;
			bool warned;
		};
	}
}

#endif
