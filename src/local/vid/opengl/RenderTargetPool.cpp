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

#include <cassert>
#include <functional> // bind
#include "RenderTarget.hpp"
#include "RenderTargetPool.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			RenderTargetPool::RenderTargetPool(const math::Vector<2, unsigned> &size, GLenum format, unsigned colorAttachments, Flags flags) :
				framebuffer(size, format, flags)
			{
				assert(colorAttachments);
				for (unsigned i = 0; i < colorAttachments; ++i)
					freeColorAttachments.push(framebuffer.AttachColorTexture());
			}

			// framebuffer attributes
			const math::Vector<2, unsigned> &RenderTargetPool::GetSize() const
			{
				return framebuffer.GetSize();
			}
			const math::Vector<2, unsigned> &RenderTargetPool::GetPow2Size() const
			{
				return framebuffer.GetPow2Size();
			}

			// factory
			RenderTargetPool::Pooled RenderTargetPool::Get() const
			{
				if (freeColorAttachments.empty())
					freeColorAttachments.push(framebuffer.AttachColorTexture());
				Pooled result(
					new RenderTarget(framebuffer, freeColorAttachments.front()),
					std::bind(
						&RenderTargetPool::DeleteRenderTarget,
						this, std::placeholders::_1));
				freeColorAttachments.pop();
				return result;
			}

			// deleter
			void RenderTargetPool::DeleteRenderTarget(const RenderTarget *rt) const
			{
				freeColorAttachments.push(rt->buffer);
				delete rt;
			}

			// binding
			void Bind(const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				Bind(*rt);
			}
			void Bind(const DrawContext &dc, const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				Bind(dc, *rt);
			}

			// texture access
			GLuint GetTexture(const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				return GetTexture(*rt);
			}
		}
	}
}
