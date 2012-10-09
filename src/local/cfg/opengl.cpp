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
#include "../err/exception/throw.hpp" // THROW
#include "opengl.hpp" // ShadowType

namespace page
{
	namespace cfg
	{
		namespace opengl
		{
			namespace
			{
				// configuration variable filters
				ShadowType ConvertInShadowType(const std::string &value)
				{
					if      (value == "exponential") return exponentialShadowType;
					else if (value == "packed")      return packedShadowType;
					else if (value == "variance")    return varianceShadowType;
					THROW err::Exception<err::ParameterTag>("invalid shadow type");
				}
				std::string ConvertOutShadowType(ShadowType value)
				{
					switch (value)
					{
						case exponentialShadowType: return "exponential";
						case packedShadowType:      return "packed";
						case varianceShadowType:    return "variance";
						default: assert(!"invalid shadow type");
					}
				}
			}

			// configuration variables
			Var<bool> vidBump("vid.bump", true);
			Var<bool> vidComposite("vid.composite", true);
			Var<unsigned> vidCompositeDown("vid.composite.down");
			Var<bool> vidGlow("vid.glow", true);
			Var<bool> vidMedian("vid.median", true);
			Var<unsigned> vidMedianLevel("vid.median.level", 1);
			Var<bool> vidMultipass("vid.multipass", true);
			Var<bool> vidOutline("vid.outline", true);
			Var<bool> vidShader("vid.shader", true);
			Var<bool> vidShadow("vid.shadow", true);
			Var<bool> vidShadowBlur("vid.shadow.blur", true);
			Var<unsigned> vidShadowDown("vid.shadow.down");
			Var<bool> vidShadowFilter("vid.shadow.filter", true);
			Var<ShadowType, std::string> vidShadowType("vid.shadow.type", varianceShadowType, 0, ConvertInShadowType, ConvertOutShadowType);
			Var<unsigned> vidTexDown("vid.tex.down");
			Var<bool> vidTexMipmap("vid.tex.mipmap", true);
			Var<bool> vidTuneAuto("vid.tune.auto", true);
			Var<std::string> vidTuneProfile("vid.tune.profile");
			Var<bool> vidVbo("vid.vbo", true);
		}
	}
}
