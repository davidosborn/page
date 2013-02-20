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

#include <algorithm> // min
#include <climits> // CHAR_BIT
#include <functional> // bind
#include <iostream> // cout
#include <string>

#include <GL/gl.h>

#include "../../err/Exception.hpp"
#include "../../err/platform/opengl.hpp" // CheckError
#include "../../log/Indenter.hpp"
#include "../../math/Color.hpp" // RgbaColor
#include "../../math/Vector.hpp"
#include "../../util/string/operations.hpp" // Trim
#include "../../wnd/Window.hpp" // Window::{GetSize,{focus,size}Sig}
#include "ClientAttribGuard.hpp"
#include "DrawContext.hpp" // DrawContext::DrawContext
#include "Driver.hpp"
#include "ext.hpp" // InitExt
#include "get.hpp" // GetString
#include "Resources.hpp" // Resources::{{,~}Resources,Resize}
#include "tune.hpp" // InitProfile

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			Driver::Driver(wnd::Window &wnd) : vid::Driver(wnd) {}

			// off-screen rendering
			res::Image Driver::RenderImage(const math::Vector<2, unsigned> &size)
			{
				// NOTE: reading from the framebuffer may produce indeterminate
				// results when the OpenGL context is not entirely visible
				math::Vector<2, int> wndSize(GetWindow().GetSize());
				if (!All(wndSize))
					THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("viewport too small")))
				// initialize destination image
				res::Image img;
				img.size = size;
				img.channels.push_back(res::Image::Channel(res::Image::Channel::red,   CHAR_BIT));
				img.channels.push_back(res::Image::Channel(res::Image::Channel::green, CHAR_BIT));
				img.channels.push_back(res::Image::Channel(res::Image::Channel::blue,  CHAR_BIT));
				img.alignment = 1;
				img.data.resize(Content(img.size) * img.channels.size());
				if (!All(img.size)) return img;
				// set pixel storage state
				ClientAttribGuard clientAttribGuard(GL_CLIENT_PIXEL_STORE_BIT);
				glPixelStorei(GL_PACK_ROW_LENGTH, img.size.x);
				if (Any(size != wndSize))
				{
					// render in sections using framebuffer
					math::Aabb<2, int> box(
						math::Aabb<2, int>(0, img.size) +
						math::Vector<2, int>(0, wndSize.y - img.size.y));
					res::Image::Data::iterator iter(img.data.begin());
					for (;;)
					{
						math::Vector<2, unsigned> section(0,
							std::min(wndSize.y - box.min.y, wndSize.y));
						for (;;)
						{
							Render(math::Aabb<2>(box) / wndSize);
							section.x = std::min(box.max.x, wndSize.x);
							glReadPixels(0, 0, section.x, section.y,
								GL_RGB, GL_UNSIGNED_BYTE, &*iter);
							iter += section.x * img.channels.size();
							if (box.max.x <= wndSize.x) break;
							box -= math::Vector<2, int>(wndSize.x, 0);
						}
						iter += (section.y - 1) * img.size.x * img.channels.size();
						if (box.min.y >= 0) break;
						box += math::Vector<2, int>(-box.min.x, wndSize.y);
					}
				}
				else
				{
					// copy from framebuffer
					glReadPixels(0, 0, img.size.x, img.size.y,
						GL_RGB, GL_UNSIGNED_BYTE, &*img.data.begin());
				}
				err::opengl::CheckError();
				return Flip(img, math::Vector<2, bool>(false, true));
			}

			// deferred initialization
			void Driver::Init()
			{
				// print driver information
				const char *versionPtr = GetString(GL_VERSION);
				std::string version(versionPtr ? util::Trim(std::string(versionPtr)) : "1.0");
				std::cout << "using OpenGL driver " << version << std::endl;
				{
					log::Indenter indenter;
					if (const char *rendererPtr = GetString(GL_RENDERER))
					{
						std::string renderer(util::Trim(std::string(rendererPtr)));
						if (!renderer.empty()) std::cout << renderer << std::endl;
					}
					if (const char *vendorPtr = GetString(GL_VENDOR))
					{
						std::string vendor(util::Trim(std::string(vendorPtr)));
						if (!vendor.empty()) std::cout << vendor << std::endl;
					}
				}
				// initialize extensions
				// NOTE: OpenGL state shadowing requires early initialization
				InitExt();
				InitAdapterExt();
				// set data processing state
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				// set rendering state
				glAlphaFunc(GL_GREATER, 0);
				glEnable(GL_CULL_FACE);
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POINT_SMOOTH);
				// set lighting state
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &*math::RgbaColor<GLfloat>(0, 0, 0, 1).begin());
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &*math::RgbaColor<GLfloat>(1, 1, 1, 1).begin());
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &*math::RgbaColor<GLfloat>(1, 1, 1, 1).begin());
				// check for errors
				err::opengl::CheckError();
				// initialize signal handlers
				using std::bind;
				using namespace std::placeholders;
				focusCon.Reset(GetWindow().focusSig.Connect(bind(&Driver::OnFocus, this, _1)));
				sizeCon.Reset(GetWindow().sizeSig.Connect(bind(&Driver::OnSize, this, _1)));
				// initialize profile
				InitProfile();
				// initialize resources
				res.reset(new Resources(GetWindow().GetSize()));
			}

			// preemptive destruction
			void Driver::Deinit()
			{
				res.reset();
				sizeCon.Reset();
				focusCon.Reset();
			}

			// draw context factory function
			vid::DrawContext *Driver::MakeDrawContext(const math::Aabb<2> &logicalBox)
			{
				return new DrawContext(*this, *res, logicalBox);
			}

			// signal handlers
			void Driver::OnFocus(bool focus)
			{
				// FIXME: enable/disable luminance filter
			}
			void Driver::OnSize(const math::Vector<2, unsigned> &size)
			{
				glViewport(0, 0, size.x, size.y);
				res->Resize(size);

				// HACK: the scissor region remains clipped to the initial size
				// of the window unless we update it here
				glScissor(0, 0, size.x, size.y);
			}
		}
	}
}
