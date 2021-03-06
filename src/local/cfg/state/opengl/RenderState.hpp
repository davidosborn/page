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
