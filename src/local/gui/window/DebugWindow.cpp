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

#include <boost/lexical_cast.hpp>

#include "../../cache/proxy/ResourceProxy.hpp"
#include "../../log/Stats.hpp"
#include "../../math/RgbaColor.hpp"
#include "../../res/Index.hpp" // Index::Load
#include "../../res/type/Image.hpp"
#include "../widget/Array.hpp"
#include "../widget/Edit.hpp"
#include "../widget/Image.hpp"
#include "../widget/List.hpp"
#include "../widget/Text.hpp"
#include "../widget/Window.hpp"
#include "DebugWindow.hpp"

namespace page { namespace game
{
	namespace
	{
		/**
		 * The color of the key text.
		 */
		const math::RgbaColor<> keyColor(.75, 1, .5);

		/**
		 * The color of the value text.
		 */
		const math::RgbaColor<> valueColor(1, 1, .5);
	}

	/*-------------+
	| constructors |
	+-------------*/

	DebugWindow::DebugWindow()
	{
		Array debugKeyArray(false, false);
		debugKeyArray.Insert(Text("running time",    0, keyColor));
		debugKeyArray.Insert(Text("frames drawn",    0, keyColor));
		debugKeyArray.Insert(Text("frame rate",      0, keyColor));
		// FIXME: waiting on support for video statistics
		/*debugKeyArray.Insert(Text("triangles drawn", 0, keyColor));
		debugKeyArray.Insert(Text("triangle rate",   0, keyColor));
		debugKeyArray.Insert(Text("pixels drawn",    0, keyColor));
		debugKeyArray.Insert(Text("pixel rate",      0, keyColor));*/
		debugKeyArray.Insert(Text("cache tries",     0, keyColor));
		debugKeyArray.Insert(Text("cache misses",    0, keyColor));
		debugKeyArray.Insert(Text("cache coherence", 0, keyColor));

		Array debugValueArray(false, false);
		debugValueArray.Insert(runTimeWidget        = std::make_shared<Text>("0:0:0", 0, valueColor));
		debugValueArray.Insert(frameCountWidget     = std::make_shared<Text>("0",     0, valueColor));
		debugValueArray.Insert(frameRateWidget      = std::make_shared<Text>("0 f/s", 0, valueColor));
		// FIXME: waiting on support for video statistics
		/*debugValueArray.Insert(triangleCountWidget  = std::make_shared<Text>("0",     0, valueColor));
		debugValueArray.Insert(triangleRateWidget   = std::make_shared<Text>("0 t/s", 0, valueColor));
		debugValueArray.Insert(pixelCountWidget     = std::make_shared<Text>("0",     0, valueColor));
		debugValueArray.Insert(pixelRateWidget      = std::make_shared<Text>("0 t/s", 0, valueColor));*/
		debugValueArray.Insert(cacheTriesWidget     = std::make_shared<Text>("0",     0, valueColor));
		debugValueArray.Insert(cacheMissesWidget    = std::make_shared<Text>("0",     0, valueColor));
		debugValueArray.Insert(cacheCoherenceWidget = std::make_shared<Text>("100%",  0, valueColor));

		Array debugArray(true);
		debugArray.Insert(debugKeyArray);
		debugArray.Insert(debugValueArray, 1, math::Vec2(.1, 0));

		window = std::make_shared<Window>("Statistics", debugArray);
	}
}}

#endif
