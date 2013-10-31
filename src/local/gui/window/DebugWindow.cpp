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
