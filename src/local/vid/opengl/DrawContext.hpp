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

#ifndef    page_local_vid_opengl_DrawContext_hpp
#   define page_local_vid_opengl_DrawContext_hpp

#	include "../DrawContext.hpp"
#	include "AttribGuard.hpp"
#	include "MatrixGuard.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Driver;
			class RenderTarget;
			class Resources;
			class Texture;

			struct DrawContext : vid::DrawContext
			{
				// construct/destroy
				DrawContext(Driver &, Resources &, const math::Aabb<2> &);
				~DrawContext();

				// driver access
				Driver &GetDriver();
				const Driver &GetDriver() const;

				// resource access
				Resources &GetResources();
				const Resources &GetResources() const;

				// filter capabilities
				int GetFilterCaps() const;

				// image rendering
				void DrawQuad(const cache::Proxy<res::Image> &,
					const math::Vec2 &co1, const math::Vec2 &uv1,
					const math::Vec2 &co2, const math::Vec2 &uv2,
					const math::Vec2 &co3, const math::Vec2 &uv3,
					const math::Vec2 &co4, const math::Vec2 &uv4,
					const math::Vector<2, bool> &clamp);

				// text rendering
				void DrawText(const std::string &text,
					const cache::Proxy<res::Font> &, float fontSize,
					const math::Aabb<2> &, bool wrap, res::TextAlign,
					const math::RgbaColor<> &topColor,
					const math::RgbaColor<> &bottomColor, float borderSize,
					const math::RgbaColor<> &borderColor);

				// full-scene rendering
				void Clear();
				void ScaleBias(float, float);
				void Fill();
				void Fill(const RenderTarget &);

				// view context factory function
				vid::ViewContext *MakeViewContext(const math::ViewFrustum<> &);

				// matrix volatility modifiers
				// NOTE: mark matrix as volatile during external manipulation
				void BeginVolatileMatrix();
				void EndVolatileMatrix();

				private:
				// frame modifiers
				void DoPushFrame(const math::Aabb<2> &);
				void DoPopFrame();

				// clipping modifiers
				void DoSetClip(const math::Aabb<2> &);

				// filter factory functions
				vid::Filter *MakeAlphaFilter(float opacity, bool opaque);
				vid::Filter *MakeGlowFilter(float intensity, bool opaque);
				vid::Filter *MakeMedianFilter(unsigned passes, bool opaque);
				vid::Filter *MakeSaturationFilter(float saturation, bool opaque);
				vid::Filter *MakeScaleBiasFilter(float scale, float bias, bool opaque);

				// matrix manipulation
				void FixMatrix(MatrixGuard &, bool frame) const;
				static void MultFrame(const math::Aabb<2> &);

				Resources &res;
				AttribGuard attribGuard;
				MatrixGuard matrixGuard;
				bool matrixVolatile;
			};
		}
	}
}

#endif
