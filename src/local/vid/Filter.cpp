#include "Filter.hpp"

namespace page
{
	namespace vid
	{
		// construct/destroy
		Filter::Filter(DrawContext &context) : context(context) {}
		Filter::~Filter() {}

		// context access
		DrawContext &Filter::GetContext()
		{
			return context;
		}
		const DrawContext &Filter::GetContext() const
		{
			return context;
		}
	}
}
