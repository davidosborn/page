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

#include <cassert>

#include "../../../err/Exception.hpp"
#include "RenderState.hpp"

namespace page { namespace cfg { namespace opengl
{
	namespace
	{
		/*---------------------------+
		| render.shadow.type filters |
		+---------------------------*/

		/**
		 * Converts @c render.shadow.type from a string to a
		 * @c RenderState::ShadowType.
		 */
		RenderState::ShadowType ConvertInShadowType(const std::string &value)
		{
			if      (value == "exponential") return RenderState::ShadowType::exponential;
			else if (value == "packed")      return RenderState::ShadowType::packed;
			else if (value == "variance")    return RenderState::ShadowType::variance;
			THROW((err::Exception<err::CfgModuleTag, err::ParameterTag>("invalid shadow type")))
		}

		/**
		 * Converts @c render.shadow.type from a @c RenderState::ShadowType to a
		 * string.
		 */
		std::string ConvertOutShadowType(RenderState::ShadowType value)
		{
			switch (value)
			{
				case RenderState::ShadowType::exponential: return "exponential";
				case RenderState::ShadowType::packed:      return "packed";
				case RenderState::ShadowType::variance:    return "variance";
				default: assert(!"invalid shadow type");
			}
		}
	}

	/*-------------+
	| constructors |
	+-------------*/

	RenderState::RenderState() :
		renderBump         (*this, "render.bump",           true),
		renderComposite    (*this, "render.composite",      true),
		renderCompositeDown(*this, "render.composite.down", 0),
		renderGlow         (*this, "render.glow",           true),
		renderMedian       (*this, "render.median",         true),
		renderMedianLevel  (*this, "render.median.level",   1),
		renderMultipass    (*this, "render.multipass",      true),
		renderOutline      (*this, "render.outline",        true),
		renderShader       (*this, "render.shader",         true),
		renderShadow       (*this, "render.shadow",         true),
		renderShadowBlur   (*this, "render.shadow.blur",    true),
		renderShadowDown   (*this, "render.shadow.down",    0),
		renderShadowFilter (*this, "render.shadow.filter",  true),
		renderShadowType   (*this, "render.shadow.type",    ShadowType::variance, nullptr, nullptr, ConvertInShadowType, ConvertOutShadowType),
		renderTextureDown  (*this, "render.texture.down",   0),
		renderTextureMipmap(*this, "render.texture.mipmap", true),
		renderTuneAuto     (*this, "render.tune.auto",      true),
		renderTuneProfile  (*this, "render.tune.profile",   ""),
		renderVbo          (*this, "render.vbo",            true) {}
}}}
