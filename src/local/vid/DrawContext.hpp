/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_vid_DrawContext_hpp
#   define page_local_vid_DrawContext_hpp

#	include <stack>
#	include <memory> // shared_ptr

#	include "../math/Aabb.hpp"
#	include "../math/fwd.hpp" // ViewFrustum
#	include "../res/type/Font.hpp" // TextAlign
#	include "../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		class Driver;
		class Filter;
		class ViewContext;

		struct DrawContext
		{
			class ClipSaver;
			class FilterSaver;
			class FrameSaver;

			// construct/destroy
			DrawContext(Driver &, const math::Aabb<2> &);
			virtual ~DrawContext();

			// driver access
			Driver &GetDriver();
			const Driver &GetDriver() const;

			// attributes
			const math::Aabb<2> &GetLogicalBox() const;
			math::Aabb<2> GetPixelLogicalBox() const;
			float GetLogicalAspect() const;

			// frame state
			const math::Aabb<2> &GetFrame() const;
			float GetFrameAspect() const;

			// clipping state
			const math::Aabb<2> &GetClip() const;
			float GetClipAspect() const;

			// filter capabilities
			enum
			{
				alphaFilter      = 0x01,
				glowFilter       = 0x02,
				medianFilter     = 0x04,
				saturationFilter = 0x08,
				scaleBiasFilter  = 0x10
			};
			virtual int GetFilterCaps() const;

			// frame stack modifiers
			// NOTE: in current frame coordinates
			void PushFrame(const math::Aabb<2> &);
			void PopFrame();

			// clipping stack modifiers
			// NOTE: in frame coordinates
			void PushClip(const math::Aabb<2> &);
			void SetClip(const math::Aabb<2> &);
			void PopClip();

			// filter stack modifiers
			void PushAlphaFilter(float opacity, bool opaque = false);
			void PushGlowFilter(float intensity, bool opaque = false);
			void PushMedianFilter(unsigned passes, bool opaque = false);
			void PushSaturationFilter(float saturation, bool opaque = false);
			void PushScaleBiasFilter(float scale, float bias, bool opaque = false);
			void PopFilter();

			// image rendering
			virtual void DrawQuad(const cache::Proxy<res::Image> &,
				const math::Vec2 &co1, const math::Vec2 &uv1,
				const math::Vec2 &co2, const math::Vec2 &uv2,
				const math::Vec2 &co3, const math::Vec2 &uv3,
				const math::Vec2 &co4, const math::Vec2 &uv4,
				const math::Vector<2, bool> &clamp) = 0;

			// text rendering
			virtual void DrawText(const std::string &text,
				const cache::Proxy<res::Font> &, float fontSize,
				const math::Aabb<2> &, bool wrap, res::TextAlign,
				const math::RgbaColor<> &topColor,
				const math::RgbaColor<> &bottomColor, float borderSize,
				const math::RgbaColor<> &borderColor) = 0;

			// full-scene rendering
			virtual void Clear() = 0;
			virtual void ScaleBias(float, float) = 0;

			// view context factory function
			virtual ViewContext *MakeViewContext(const math::ViewFrustum<> &) = 0;

			protected:
			// preemptive destruction
			// must be called at beginning of derived destructor
			void Deinit();

			private:
			// frame modifiers
			// NOTE: in current frame coordinates
			virtual void DoPushFrame(const math::Aabb<2> &) = 0;
			virtual void DoPopFrame() = 0;

			// clipping modifiers
			// NOTE: in window coordinates
			virtual void DoSetClip(const math::Aabb<2> &) = 0;

			// filter stack modifiers
			void PushFilter(Filter *);

			// filter factory functions
			virtual Filter *MakeAlphaFilter(float opacity, bool opaque);
			virtual Filter *MakeGlowFilter(float intensity, bool opaque);
			virtual Filter *MakeMedianFilter(unsigned passes, bool opaque);
			virtual Filter *MakeSaturationFilter(float saturation, bool opaque);
			virtual Filter *MakeScaleBiasFilter(float scale, float bias, bool opaque);

			Driver &driver;
			math::Aabb<2> logicalBox;
			typedef std::stack<math::Aabb<2>> Frames;
			Frames frames;
			typedef std::stack<math::Aabb<2>> Clips;
			Clips clips;
			typedef std::stack<std::shared_ptr<Filter>> Filters;
			Filters filters;
		};

		// frame stack saver
		// restores stack depth on destruction
		struct DrawContext::FrameSaver : util::StateSaver<DrawContext::FrameSaver>
		{
			friend class util::StateSaver<DrawContext::FrameSaver>;
			typedef util::StateSaver<DrawContext::FrameSaver> Base;

			// construct
			FrameSaver();
			explicit FrameSaver(DrawContext &);

			// modifiers
			void Reset();
			void Reset(DrawContext &);

			private:
			void Save();
			void Load();

			DrawContext *context;
			unsigned depth;
		};

		// clip stack saver
		// restores stack depth on destruction
		struct DrawContext::ClipSaver : util::StateSaver<DrawContext::ClipSaver>
		{
			friend class util::StateSaver<DrawContext::ClipSaver>;
			typedef util::StateSaver<DrawContext::ClipSaver> Base;

			// construct
			ClipSaver();
			explicit ClipSaver(DrawContext &);

			// modifiers
			void Reset();
			void Reset(DrawContext &);

			private:
			void Save();
			void Load();

			DrawContext *context;
			unsigned depth;
		};

		// filter stack saver
		// restores stack depth on destruction
		struct DrawContext::FilterSaver : util::StateSaver<DrawContext::FilterSaver>
		{
			friend class util::StateSaver<DrawContext::FilterSaver>;
			typedef util::StateSaver<DrawContext::FilterSaver> Base;

			// construct
			FilterSaver();
			explicit FilterSaver(DrawContext &);

			// modifiers
			void Reset();
			void Reset(DrawContext &);

			private:
			void Save();
			void Load();

			DrawContext *context;
			unsigned depth;
		};
	}
}

#endif
