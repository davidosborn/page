#ifndef    page_local_vid_Filter_hpp
#   define page_local_vid_Filter_hpp

namespace page
{
	namespace vid
	{
		class DrawContext;

		struct Filter
		{
			// construct/destroy
			explicit Filter(DrawContext &);
			virtual ~Filter();

			// context access
			DrawContext &GetContext();
			const DrawContext &GetContext() const;

			private:
			DrawContext &context;
		};
	}
}

#endif
