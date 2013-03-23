/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include "../wnd/Window.hpp" // Window::GetSize
#include "DrawContext.hpp"
#include "Driver.hpp" // Driver::GetWindow
#include "Filter.hpp"

namespace page
{
	namespace vid
	{
		// construct/destroy
		DrawContext::DrawContext(Driver &driver, const math::Aabb<2> &logicalBox) :
			driver(driver), logicalBox(logicalBox)
		{
			assert(All(Size(logicalBox)));
			frames.push(math::Aabb<2>(0, 1));
			clips.push(math::Aabb<2>(0, 1));
		}
		DrawContext::~DrawContext() {}

		// driver access
		Driver &DrawContext::GetDriver()
		{
			return driver;
		}
		const Driver &DrawContext::GetDriver() const
		{
			return driver;
		}

		// attributes
		const math::Aabb<2> &DrawContext::GetLogicalBox() const
		{
			return logicalBox;
		}
		math::Aabb<2> DrawContext::GetPixelLogicalBox() const
		{
			return logicalBox * driver.GetWindow().GetSize();
		}
		float DrawContext::GetLogicalAspect() const
		{
			return Aspect(GetPixelLogicalBox());
		}

		// frame state
		const math::Aabb<2> &DrawContext::GetFrame() const
		{
			return frames.top();
		}
		float DrawContext::GetFrameAspect() const
		{
			return GetLogicalAspect() * Aspect(frames.top());
		}

		// clipping state
		const math::Aabb<2> &DrawContext::GetClip() const
		{
			return clips.top();
		}
		float DrawContext::GetClipAspect() const
		{
			return GetLogicalAspect() * Aspect(clips.top());
		}

		// filter capabilities
		int DrawContext::GetFilterCaps() const
		{
			return 0;
		}

		// frame stack modifiers
		void DrawContext::PushFrame(const math::Aabb<2> &frame)
		{
			frames.push(LeaveSpace(frames.top(), frame));
			DoPushFrame(frame);
		}
		void DrawContext::PopFrame()
		{
			assert(frames.size() > 1);
			frames.pop();
			DoPopFrame();
		}

		// clipping stack modifiers
		void DrawContext::PushClip(const math::Aabb<2> &clip)
		{
			clips.push(Min(LeaveSpace(frames.top(), clip), clips.top()));
			DoSetClip(LeaveSpace(logicalBox, clips.top()));
		}
		void DrawContext::SetClip(const math::Aabb<2> &clip)
		{
			clips.pop();
			PushClip(clip);
		}
		void DrawContext::PopClip()
		{
			assert(clips.size() > 1);
			clips.pop();
			DoSetClip(LeaveSpace(logicalBox, clips.top()));
		}

		// filter stack modifiers
		void DrawContext::PushAlphaFilter(float opacity, bool opaque)
		{
			PushFilter(MakeAlphaFilter(opacity, opaque));
		}
		void DrawContext::PushGlowFilter(float intensity, bool opaque)
		{
			PushFilter(MakeGlowFilter(intensity, opaque));
		}
		void DrawContext::PushMedianFilter(unsigned passes, bool opaque)
		{
			PushFilter(MakeMedianFilter(passes, opaque));
		}
		void DrawContext::PushSaturationFilter(float saturation, bool opaque)
		{
			PushFilter(MakeSaturationFilter(saturation, opaque));
		}
		void DrawContext::PushScaleBiasFilter(float scale, float bias, bool opaque)
		{
			PushFilter(MakeScaleBiasFilter(scale, bias, opaque));
		}
		void DrawContext::PopFilter()
		{
			assert(!filters.empty());
			filters.pop();
		}

		// preemptive destruction
		void DrawContext::Deinit()
		{
			while (!filters.empty()) filters.pop();
			// NOTE: frame/clip stack should be restored in derived destructor
		}

		// filter stack modifiers
		void DrawContext::PushFilter(Filter *filter)
		{
			filters.push(std::shared_ptr<Filter>(filter));
		}

		// filter factory functions
		Filter *DrawContext::MakeAlphaFilter(float, bool)            { assert(!"not supported"); }
		Filter *DrawContext::MakeGlowFilter(float, bool)             { assert(!"not supported"); }
		Filter *DrawContext::MakeMedianFilter(unsigned, bool)        { assert(!"not supported"); }
		Filter *DrawContext::MakeSaturationFilter(float, bool)       { assert(!"not supported"); }
		Filter *DrawContext::MakeScaleBiasFilter(float, float, bool) { assert(!"not supported"); }

		// frame stack saver
		// construct
		DrawContext::FrameSaver::FrameSaver() {}
		DrawContext::FrameSaver::FrameSaver(DrawContext &context)
		{
			Reset(context);
		}

		// modifiers
		void DrawContext::FrameSaver::Reset()
		{
			Base::Reset();
		}
		void DrawContext::FrameSaver::Reset(DrawContext &context)
		{
			this->context = &context;
			Base::Reset(true);
		}
		void DrawContext::FrameSaver::Save()
		{
			depth = context->frames.size();
		}
		void DrawContext::FrameSaver::Load()
		{
			while (context->frames.size() > depth)
				context->PopFrame();
		}

		// clip stack saver
		DrawContext::ClipSaver::ClipSaver() {}
		DrawContext::ClipSaver::ClipSaver(DrawContext &context)
		{
			Reset(context);
		}

		// modifiers
		void DrawContext::ClipSaver::Reset()
		{
			Base::Reset();
		}
		void DrawContext::ClipSaver::Reset(DrawContext &context)
		{
			this->context = &context;
			Base::Reset(true);
		}
		void DrawContext::ClipSaver::Save()
		{
			depth = context->clips.size();
		}
		void DrawContext::ClipSaver::Load()
		{
			while (context->clips.size() > depth) context->PopClip();
		}

		// filter stack saver
		DrawContext::FilterSaver::FilterSaver() {}
		DrawContext::FilterSaver::FilterSaver(DrawContext &context)
		{
			Reset(context);
		}

		// modifiers
		void DrawContext::FilterSaver::Reset()
		{
			Base::Reset();
		}
		void DrawContext::FilterSaver::Reset(DrawContext &context)
		{
			this->context = &context;
			Base::Reset(true);
		}
		void DrawContext::FilterSaver::Save()
		{
			depth = context->filters.size();
		}
		void DrawContext::FilterSaver::Load()
		{
			while (context->filters.size() > depth) context->filters.pop();
		}
	}
}
