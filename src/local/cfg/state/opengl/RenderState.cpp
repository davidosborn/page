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
