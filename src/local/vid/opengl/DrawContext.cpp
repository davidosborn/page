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

#include <GL/gl.h>
#include "../../cache/proxy/opengl/Texture.hpp"
#include "../../math/Vector.hpp"
#include "../../wnd/Window.hpp" // Window::GetSize
#include "../Driver.hpp" // Driver::GetWindow
#include "DrawContext.hpp"
#include "Driver.hpp" // Driver->vid::Driver
#include "ext.hpp" // EXT_blend_{minmax,subtract}
#include "filter/AlphaFilter.hpp"
#include "filter/GlowFilter.hpp"
#include "filter/MedianFilter.hpp"
#include "filter/SaturationFilter.hpp"
#include "filter/ScaleBiasFilter.hpp"
#include "Framebuffer.hpp" // Framebuffer::Get{,Pow2}Size
#include "RenderTarget.hpp" // GetTexture, RenderTarget::framebuffer
#include "Resources.hpp"
#include "Texture.hpp" // Bind
#include "TextureFlags.hpp" // filterTextureFlag
#include "TextureFormat.hpp" // defaultTextureFormat
#include "ViewContext.hpp"

// FIXME: for DrawText
#include <algorithm> // find, find_if
#include <cctype> // isspace
#include <cmath> // ceil, sqrt
#include <functional> // bind2nd, not_equal_to
#include <vector>
#include "../../cache/proxy/opengl/FontTexture.hpp"
#include "../../math/Color.hpp" // RgbaColor
#include "../../res/type/Font.hpp" // GetAdvance, GetGlyph, Wrap
#include "../../util/functional/locale.hpp" // isspace_function
#include "FontTexture.hpp" // Bind, FontTexture::GetSection

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				// FIXME: for DrawText
				// glyph coordinates for font rendering
				struct GlyphCoords
				{
					GlyphCoords(
						const math::Aabb<2> &uv,
						const math::Aabb<2> &co) : uv(uv), co(co) {}

					math::Aabb<2> uv, co;
				};
			}

			// construct/destroy
			DrawContext::DrawContext(Driver &driver, Resources &res, const math::Aabb<2> &logicalBox) :
				vid::DrawContext(driver, logicalBox), res(res),
				matrixVolatile(false)
			{
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_SCISSOR_BIT);
				glEnable(GL_SCISSOR_TEST);
				// setup ortho projection
				glMatrixMode(GL_PROJECTION);
				matrixGuard.Push();
				glOrtho(0, 1, 1, 0, -1, 1);
				// setup logical box
				if (logicalBox != math::Aabb<2>(0, 1))
				{
					MultFrame(logicalBox);
					DoSetClip(logicalBox);
				}
				// reset frame stack matrix
				glMatrixMode(GL_MODELVIEW);
				matrixGuard.Push();
				glLoadIdentity();
				// set default state
				// NOTE: attributes pushed earlier with scissor
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			DrawContext::~DrawContext()
			{
				// clear filters before restoring attribGuard
				vid::DrawContext::Deinit();
			}

			// driver access
			Driver &DrawContext::GetDriver()
			{
				return static_cast<Driver &>(vid::DrawContext::GetDriver());
			}
			const Driver &DrawContext::GetDriver() const
			{
				return static_cast<const Driver &>(vid::DrawContext::GetDriver());
			}

			// resource access
			Resources &DrawContext::GetResources()
			{
				return res;
			}
			const Resources &DrawContext::GetResources() const
			{
				return res;
			}

			// filter capabilities
			int DrawContext::GetFilterCaps() const
			{
				int caps = 0;
				if (AlphaFilter::Check(res))      caps |= alphaFilter;
				if (GlowFilter::Check(res))       caps |= glowFilter;
				if (MedianFilter::Check(res))     caps |= medianFilter;
				if (SaturationFilter::Check(res)) caps |= saturationFilter;
				if (ScaleBiasFilter::Check(res))  caps |= scaleBiasFilter;
				return caps;
			}

			// image rendering
			void DrawContext::DrawQuad(const cache::Proxy<res::Image> &img,
				const math::Vector<2> &co1, const math::Vector<2> &uv1,
				const math::Vector<2> &co2, const math::Vector<2> &uv2,
				const math::Vector<2> &co3, const math::Vector<2> &uv3,
				const math::Vector<2> &co4, const math::Vector<2> &uv4,
				const math::Vector<2, bool> &clamp)
			{
				MatrixGuard matrixGuard;
				FixMatrix(matrixGuard, true);
				AttribGuard attribGuard;
				glPushAttrib(GL_ENABLE_BIT);
				Bind(*cache::opengl::Texture(img, defaultTextureFormat, filterTextureFlag, clamp));
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				glTexCoord2fv(&*uv1.begin()); glVertex2fv(&*co1.begin());
				glTexCoord2fv(&*uv2.begin()); glVertex2fv(&*co2.begin());
				glTexCoord2fv(&*uv3.begin()); glVertex2fv(&*co3.begin());
				glTexCoord2fv(&*uv4.begin()); glVertex2fv(&*co4.begin());
				glEnd();
			}

			// text rendering
			void DrawContext::DrawText(const std::string &text,
				const cache::Proxy<res::Font> &fontProxy, float fontSize,
				const math::Aabb<2> &box, bool wrap, res::TextAlign align,
				const math::RgbaColor<> &topColor,
				const math::RgbaColor<> &bottomColor, float borderSize,
				const math::RgbaColor<> &borderColor)
			{
				MatrixGuard matrixGuard;
				FixMatrix(matrixGuard, true);
				// FIXME: most of this can be moved into a platform-independant
				// module; at the same time, the OpenGL part should make use of
				// vertex arrays/buffers
				// calculate metrics
				const res::Font &font(*fontProxy);
				math::Vector<2> scale(fontSize / math::Vector<2>(GetFrameAspect(), 1));
				float width = (box.max.x - box.min.x) / scale.x;
				std::string wrappedText(wrap ? Wrap(font, text, width) : text);
				math::Vector<2> pen(box.min.x, box.min.y + font.maxBearing.y * scale.y);
				// calculate glyph rendering coordinates
				const FontTexture &texture(*cache::opengl::FontTexture(fontProxy,
					std::ceil(fontSize * (Size(GetFrame()) * Size(GetPixelLogicalBox())).y)));
				typedef std::vector<GlyphCoords> Glyphs;
				Glyphs glyphs;
				glyphs.reserve(text.size());
				for (std::string::iterator line(wrappedText.begin()); line != wrappedText.end();)
				{
					std::string::iterator end(std::find(line, wrappedText.end(), '\n'));
					// calculate whitespace coefficient for justified alignment
					float offset = 0, space = 1;
					if (align == res::justifyTextAlign)
					{
						space = GetJustifiedScale(font, line, end, width);
						if (space >= 1)
						{
							if (std::find_if(line, end, util::isspace_function<char>()) == end)
							{
								if (end - line >= 2)
								{
									for (std::string::iterator c(line + 1); c != end; ++c)
									{
										std::string::iterator::difference_type
											lineDistance(line - wrappedText.begin()),
											endDistance(end - wrappedText.begin());
										c = wrappedText.insert(c, ' ') + 1;
										line = wrappedText.begin() + lineDistance;
										end = wrappedText.begin() + endDistance + 1;
									}
									space = GetJustifiedScale(font, line, end, width);
								}
								else
								{
									space = 1;
									goto centerAlign;
								}
							}
						}
						else
						{
							space = 1;
							goto centerAlign;
						}
					}
					// calculate offset for center and right alignment
					switch (align)
					{
						case res::centerTextAlign: centerAlign:
						offset = GetCenteredOffset(font, line, end, width);
						break;
						case res::rightTextAlign:
						offset = GetRightAlignedOffset(font, line, end, width);
						break;
					}
					// generate glyph coordinates
					pen.x += offset * scale.x;
					for (std::string::const_iterator c(line); c != end; ++c)
					{
						if (const res::Font::Glyph *glyph = GetGlyph(font, *c))
						{
							math::Vector<2> co(pen);
							co.x += glyph->bearing.x * scale.x;
							co.y -= glyph->bearing.y * scale.y;
							glyphs.push_back(GlyphCoords(*texture.GetSection(*c),
								math::Aabb<2>(co, co + glyph->size * scale)));
						}
						std::string::const_iterator next(c + 1);
						float advance = (next != end ?
							GetAdvance(font, *c, *next) :
							GetAdvance(font, *c));
						if (std::isspace(*c)) advance *= space;
						pen.x += advance * scale.x;
					}
					if (end == wrappedText.end()) break;
					end = std::find_if((line = end) + 1, wrappedText.end(),
						std::bind2nd(std::not_equal_to<char>(), '\n'));
					pen = math::Vector<2>(box.min.x,
						pen.y + font.lineHeight * scale.y * (end - line));
					line = end;
				}
				// enable clipping
				ClipSaver clipSaver(*this);
				PushClip(box); // FIXME: this used to be PushRootClip
				// initialize render state
				AttribGuard attribGuard;
				glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
				Bind(texture);
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				// render borders
				if (borderSize)
				{
					math::Vector<2>
						jitter(borderSize * scale),
						diagJitter(jitter / std::sqrt(2.f));
					glColor4fv(&*math::RgbaColor<GLfloat>(borderColor).begin());
					for (Glyphs::const_iterator glyph(glyphs.begin()); glyph != glyphs.end(); ++glyph)
					{
						const math::Aabb<2> &uv(glyph->uv), &co(glyph->co);
						// left
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x - jitter.x, co.min.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x - jitter.x, co.min.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x - jitter.x, co.max.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x - jitter.x, co.max.y);
						// right
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x + jitter.x, co.min.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x + jitter.x, co.min.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x + jitter.x, co.max.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x + jitter.x, co.max.y);
						// up
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x, co.min.y - jitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x, co.min.y - jitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x, co.max.y - jitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x, co.max.y - jitter.y);
						// down
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x, co.min.y + jitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x, co.min.y + jitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x, co.max.y + jitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x, co.max.y + jitter.y);
						// up-left
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x - diagJitter.x, co.min.y - diagJitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x - diagJitter.x, co.min.y - diagJitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x - diagJitter.x, co.max.y - diagJitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x - diagJitter.x, co.max.y - diagJitter.y);
						// up-right
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x + diagJitter.x, co.min.y - diagJitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x + diagJitter.x, co.min.y - diagJitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x + diagJitter.x, co.max.y - diagJitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x + diagJitter.x, co.max.y - diagJitter.y);
						// down-left
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x - diagJitter.x, co.min.y + diagJitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x - diagJitter.x, co.min.y + diagJitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x - diagJitter.x, co.max.y + diagJitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x - diagJitter.x, co.max.y + diagJitter.y);
						// down-right
						glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x + diagJitter.x, co.min.y + jitter.y);
						glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x + diagJitter.x, co.min.y + jitter.y);
						glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x + diagJitter.x, co.max.y + jitter.y);
						glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x + diagJitter.x, co.max.y + jitter.y);
					}
				}
				// render glyphs
				for (Glyphs::const_iterator glyph(glyphs.begin()); glyph != glyphs.end(); ++glyph)
				{
					const math::Aabb<2> &uv(glyph->uv), &co(glyph->co);
					glColor4fv(&*math::RgbaColor<GLfloat>(topColor).begin());
					glTexCoord2f(uv.max.x, uv.min.y); glVertex2f(co.max.x, co.min.y);
					glTexCoord2f(uv.min.x, uv.min.y); glVertex2f(co.min.x, co.min.y);
					glColor4fv(&*math::RgbaColor<GLfloat>(bottomColor).begin());
					glTexCoord2f(uv.min.x, uv.max.y); glVertex2f(co.min.x, co.max.y);
					glTexCoord2f(uv.max.x, uv.max.y); glVertex2f(co.max.x, co.max.y);
				}
				glEnd();
			}

			// full-scene rendering
			void DrawContext::Clear()
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}
			void DrawContext::ScaleBias(float scale, float bias)
			{
				// in-place technique described by Mark Kilgard
				// http://opengl.org/resources/code/samples/mjktips/InPlaceScaleAndBias.html
				AttribGuard attribGuard;
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				if (scale > 1)
				{
					glBlendFunc(GL_DST_COLOR, GL_ONE);
					while (scale > 2)
					{
						Fill();
						scale /= 2;
					}
					glColor3f(scale - 1, scale - 1, scale - 1);
					scale = 1;
				}
				if (bias < 0)
				{
					assert(haveExtBlendMinmax && haveExtBlendSubtract);
					glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
					bias = -bias;
				}
				glBlendFunc(GL_ONE, GL_SRC_ALPHA);
				glColor4f(bias, bias, bias, scale);
				Fill();
			}
			void DrawContext::Fill()
			{
				MatrixGuard matrixGuard;
				FixMatrix(matrixGuard, false);
				glBegin(GL_QUADS);
				glVertex2i(0, 0);
				glVertex2i(0, 1);
				glVertex2i(1, 1);
				glVertex2i(1, 0);
				glEnd();
			}
			void DrawContext::Fill(const RenderTarget &rt)
			{
				// FIXME: I don't think we can assume the logical box here,
				// because this function is not always rendering to the screen
				// (ie: it might be doing a shadow buffer)
				math::Aabb<2> uv(LeaveSpace(GetLogicalBox(), math::Aabb<2>(0,
					math::Vector<2>(rt.framebuffer.GetSize()) /
					rt.framebuffer.GetPow2Size())));
				uv.min.y = 1 - uv.min.y;
				uv.max.y = 1 - uv.max.y;
				MatrixGuard matrixGuard;
				FixMatrix(matrixGuard, false);
				AttribGuard attribGuard;
				glPushAttrib(GL_ENABLE_BIT);
				glBindTexture(GL_TEXTURE_2D, GetTexture(rt));
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				glTexCoord2f(uv.min.x, uv.min.y); glVertex2i(0, 0);
				glTexCoord2f(uv.min.x, uv.max.y); glVertex2i(0, 1);
				glTexCoord2f(uv.max.x, uv.max.y); glVertex2i(1, 1);
				glTexCoord2f(uv.max.x, uv.min.y); glVertex2i(1, 0);
				glEnd();
			}

			// matrix volatility
			void DrawContext::BeginVolatileMatrix()
			{
				matrixVolatile = true;
			}
			void DrawContext::EndVolatileMatrix()
			{
				matrixVolatile = false;
			}

			// view context factory function
			vid::ViewContext *DrawContext::MakeViewContext(const math::ViewFrustum<> &frustum)
			{
				return new ViewContext(*this, frustum);
			}

			// frame modifiers
			void DrawContext::DoPushFrame(const math::Aabb<2> &frame)
			{
				if (!matrixVolatile)
				{
					glMatrixMode(GL_MODELVIEW);
					matrixGuard.Push();
					MultFrame(frame);
				}
			}
			void DrawContext::DoPopFrame()
			{
				if (!matrixVolatile) matrixGuard.Pop();
			}

			// clipping modifiers
			void DrawContext::DoSetClip(const math::Aabb<2> &clip)
			{
				math::Vector<2, int> wndSize(GetDriver().GetWindow().GetSize());
				math::Aabb<2, int> scissor(Ceil(clip * wndSize));
				glScissor(
					scissor.min.x,
					wndSize.y - scissor.max.y,
					scissor.max.x - scissor.min.x,
					scissor.max.y - scissor.min.y);
			}

			// filter factory functions
			vid::Filter *DrawContext::MakeAlphaFilter(float opacity, bool opaque)
			{
				return new AlphaFilter(*this, res, opacity, opaque);
			}
			vid::Filter *DrawContext::MakeGlowFilter(float intensity, bool opaque)
			{
				return new GlowFilter(*this, res, intensity, opaque);
			}
			vid::Filter *DrawContext::MakeMedianFilter(unsigned passes, bool opaque)
			{
				return new MedianFilter(*this, res, passes, opaque);
			}
			vid::Filter *DrawContext::MakeSaturationFilter(float saturation, bool opaque)
			{
				return new SaturationFilter(*this, res, saturation, opaque);
			}
			vid::Filter *DrawContext::MakeScaleBiasFilter(float scale, float bias, bool opaque)
			{
				return new ScaleBiasFilter(*this, res, scale, bias, opaque);
			}

			// matrix manipulation
			void DrawContext::FixMatrix(MatrixGuard &matrixGuard, bool frame) const
			{
				if (matrixVolatile)
				{
					glMatrixMode(GL_PROJECTION);
					matrixGuard.Push();
					glLoadIdentity();
					glOrtho(0, 1, 1, 0, -1, 1);
					MultFrame(GetLogicalBox());
					glMatrixMode(GL_MODELVIEW);
					matrixGuard.Push();
					glLoadIdentity();
					if (frame) MultFrame(GetFrame());
				}
				else if (!frame)
				{
					glMatrixMode(GL_MODELVIEW);
					matrixGuard.Push();
					glLoadIdentity();
				}
			}
			void DrawContext::MultFrame(const math::Aabb<2> &frame)
			{
				glTranslatef(frame.min.x, frame.min.y, 0);
				math::Vector<2> frameSize(Size(frame));
				glScalef(frameSize.x, frameSize.y, 1);
			}
		}
	}
}
