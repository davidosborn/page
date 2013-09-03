#ifndef    page_local_cfg_state_opengl_RenderState_hpp
#   define page_local_cfg_state_opengl_RenderState_hpp

#	include <string>

#	include "../../var/Var.hpp"
#	include "../BasicState.hpp"

namespace page { namespace cfg { namespace opengl
{
	class RenderState : public virtual BasicState
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		RenderState();

		/*-----------+
		| enum types |
		+-----------*/

		enum class ShadowType
		{
			exponential,
			packed,
			variance
		};

		/*------------------------+
		| configuration variables |
		+------------------------*/

		/**
		 * A configuration variable specifying whether to perform bump mapping.
		 */
		Var<bool>                                    renderBump;

		/**
		 * A configuration variable specifying whether to perform off-screen
		 * compositing.
		 */
		Var<bool>                                    renderComposite;

		/**
		 * A configuration variable specifying how many time to down-sample when
		 * performing off-screen compositing.
		 */
		Var<unsigned>                                renderCompositeDown;

		/**
		 * A configuration variable specifying whether to render a glow effect.
		 */
		Var<bool>                                    renderGlow;

		/**
		 * A configuration variable specifying whether to perform median
		 * filtering.
		 */
		Var<bool>                                    renderMedian;

		/**
		 * A configuration variable specifying the median filtering level.
		 */
		Var<unsigned>                                renderMedianLevel;

		/**
		 * A configuration variable specifying whether to render in multiple
		 * passes when necessary.
		 */
		Var<bool>                                    renderMultipass;

		/**
		 * A configuration variable specifying whether to draw outlines.
		 */
		Var<bool>                                    renderOutline;

		/**
		 * A configuration variable specifying whether to use shaders.
		 */
		Var<bool>                                    renderShader;

		/**
		 * A configuration variable specifying whether to draw shadows.
		 */
		Var<bool>                                    renderShadow;

		/**
		 * A configuration variable specifying whether to perform blurring on
		 * shadows.
		 */
		Var<bool>                                    renderShadowBlur;

		/**
		 * A configuration variable specifying how many times to down-sample
		 * shadows.
		 */
		Var<unsigned>                                renderShadowDown;

		/**
		 * A configuration variable specifying whether to filter shadows.
		 */
		Var<bool>                                    renderShadowFilter;

		/**
		 * A configuration variable specifying the shadow algorithm to use.
		 */
		Var<ShadowType, std::string>                 renderShadowType;

		/**
		 * A configuration variable specifying how many times to down-sample the
		 * textures.
		 */
		Var<unsigned>                                renderTextureDown;

		/**
		 * A configuration variable specifying whether to perform mip-mapping.
		 */
		Var<bool>                                    renderTextureMipmap;

		/**
		 * A configuration variable specifying whether to automatically adjust
		 * the video settings for new hardware.
		 */
		Var<bool>                                    renderTuneAuto;

		/**
		 * A configuration variable specifying the profile to use when
		 * automatically adjusting the video settings.
		 */
		Var<std::string>                             renderTuneProfile;

		/**
		 * A configuration variable specifying whether to use vertex-buffer
		 * objects.
		 */
		Var<bool>                                    renderVbo;
	};
}}}

#endif
