#ifndef    page_local_vid_opengl_Filter_hpp
#   define page_local_vid_opengl_Filter_hpp

#	include "../Filter.hpp"
#	include "AttribGuard.hpp"
#	include "RenderTargetPool.hpp" // RenderTargetPool::Pooled
#	include "RenderTargetSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class DrawContext;
			class RenderTarget;
			class Resources;

			struct Filter : vid::Filter
			{
				// construct
				Filter(DrawContext &, Resources &, bool opaque);

				// context access
				DrawContext &GetContext();
				const DrawContext &GetContext() const;

				protected:
				// resource access
				Resources &GetResources();
				const Resources &GetResources() const;

				// attributes
				bool IsOpaque() const;

				// render-target access
				const RenderTarget &GetRenderTarget() const;

				// render-target binding modifiers
				void RestoreRenderTargetBinding();

				// rendering
				void PrepComposite();
				void Fill(const RenderTarget &, float margin = 0);

				// compatibility
				static bool Check(Resources &);

				private:
				Resources &res;
				bool opaque;
				AttribGuard attribGuard;
				RenderTargetSaver renderTargetSaver;
				RenderTargetPool::Pooled renderTarget;
			};
		}
	}
}

#endif
