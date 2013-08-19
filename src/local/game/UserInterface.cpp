#include <string>

#include <boost/lexical_cast.hpp>

#include "../cache/proxy/Resource.hpp"
#include "../log/Stats.hpp"
#include "../res/Index.hpp" // GetIndex, Index::Load
#include "../res/type/Image.hpp"
#include "../ui/widget/Array.hpp"
#include "../ui/widget/Edit.hpp"
#include "../ui/widget/Image.hpp"
#include "../ui/widget/List.hpp"
#include "../ui/widget/Text.hpp"
#include "../ui/widget/Window.hpp"
#include "UserInterface.hpp"

namespace page { namespace game
{
	// construct
	UserInterface::UserInterface(aud::Driver &audioDriver) :
		ui::UserInterface(res::GetIndex().Load<res::Theme>("ui/theme/default.theme")),
		audioDriver(audioDriver)
	{
		using namespace ui;
		// create debug window
		math::RgbaColor<>
			debugKeyColor(.75, 1, .5),
			debugValueColor(1, 1, .5);
		Array debugKeyArray(false, false);
		debugKeyArray.Insert(Text("running time",    0, debugKeyColor));
		debugKeyArray.Insert(Text("frames drawn",    0, debugKeyColor));
		debugKeyArray.Insert(Text("frame rate",      0, debugKeyColor));
		// FIXME: waiting on support for video statistics
		/*debugKeyArray.Insert(Text("triangles drawn", 0, debugKeyColor));
		debugKeyArray.Insert(Text("triangle rate",   0, debugKeyColor));
		debugKeyArray.Insert(Text("pixels drawn",    0, debugKeyColor));
		debugKeyArray.Insert(Text("pixel rate",      0, debugKeyColor));*/
		debugKeyArray.Insert(Text("cache tries",     0, debugKeyColor));
		debugKeyArray.Insert(Text("cache misses",    0, debugKeyColor));
		debugKeyArray.Insert(Text("cache coherence", 0, debugKeyColor));
		Array debugValueArray(false, false);
		debugValueArray.Insert(statRunTime        = std::shared_ptr<Text>(new Text("0:0:0", 0, debugValueColor)));
		debugValueArray.Insert(statFrameCount     = std::shared_ptr<Text>(new Text("0",     0, debugValueColor)));
		debugValueArray.Insert(statFrameRate      = std::shared_ptr<Text>(new Text("0 f/s", 0, debugValueColor)));
		// FIXME: waiting on support for video statistics
		/*debugValueArray.Insert(statTriCount       = std::shared_ptr<Text>(new Text("0",     0, debugValueColor)));
		debugValueArray.Insert(statTriRate        = std::shared_ptr<Text>(new Text("0 t/s", 0, debugValueColor)));
		debugValueArray.Insert(statPixelCount     = std::shared_ptr<Text>(new Text("0",     0, debugValueColor)));
		debugValueArray.Insert(statPixelRate      = std::shared_ptr<Text>(new Text("0 t/s", 0, debugValueColor)));*/
		debugValueArray.Insert(statCacheTries     = std::shared_ptr<Text>(new Text("0",     0, debugValueColor)));
		debugValueArray.Insert(statCacheMisses    = std::shared_ptr<Text>(new Text("0",     0, debugValueColor)));
		debugValueArray.Insert(statCacheCoherence = std::shared_ptr<Text>(new Text("100%",  0, debugValueColor)));
		Array debugArray(true);
		debugArray.Insert(debugKeyArray);
		debugArray.Insert(debugValueArray, 1, math::Vec2(.1, 0));
		Insert(statWindow = std::shared_ptr<Window>(new Window("Statistics", debugArray)), 0);
		// create main menu
		/*Array menuArray(false);
		menuArray.Insert(Text("Begin"));
		menuArray.Insert(Text("Continue"));
		menuArray.Insert(Text("Settings"));
		menuArray.Insert(Text("Credits"));
		menuArray.Insert(Text("Quit"));
		Insert(Window("Menu", menuArray)).Show(false);
		// create inventory window
		Array invArray[] =
		{
			Array(false), // picture and description
			Array(true)   // list and picture/description
		};
		invArray[0].Insert(Image(cache::Resource<res::Image>("ui/inventory/default.png"), .25));
		invArray[0].Insert(Text("For we can a priori and prior to all given objects have a knowledge of those conditions on which alone experience of them is possible, but never of the laws to which things may in themselves be subject, without reference to possible experience.", .25));
		List invList(math::Vec2(.2, .5));
		invList.Insert("toothpick");
		invList.Insert("hand grenade");
		invList.Insert("baking soda");
		invList.Insert("monkey wrench");
		invArray[1].Insert(invList);
		invArray[1].Insert(invArray[0]);
		Insert(Window("Inventory", invArray[1])).Show();
		// create journal window
		// FIXME: implement
		// create map window
		// FIXME: implement*/
		// create command bar
		cmdBar.reset(new Array(true));
		cmdBar->Insert(Image(cache::Resource<res::Image>("ui/command/menu.png")));
		cmdBar->Insert(Image(cache::Resource<res::Image>("ui/command/inventory.png")));
		cmdBar->Insert(Image(cache::Resource<res::Image>("ui/command/journal.png")));
		cmdBar->Insert(Image(cache::Resource<res::Image>("ui/command/map.png")));
		cmdBar->Insert(Edit());
		Insert(cmdBar, math::Vec2(.5, 1));
	}

	// update
	void UserInterface::Update(float deltaTime)
	{
		if (statWindow->IsVisible())
		{
			const auto &stats(GLOBAL(log::Stats));
			// FIXME: update running time
			statFrameCount    ->SetText(boost::lexical_cast<std::string>(stats.GetFrameCount()));
			statFrameRate     ->SetText(boost::lexical_cast<std::string>(unsigned(std::floor(stats.GetFrameRate() + .5f))) + " f/s");
			statCacheTries    ->SetText(boost::lexical_cast<std::string>(stats.GetCacheTries()));
			statCacheMisses   ->SetText(boost::lexical_cast<std::string>(stats.GetCacheMisses()));
			statCacheCoherence->SetText(boost::lexical_cast<std::string>(unsigned(std::floor(stats.GetCacheCoherence() * 100 + .5f))) + "%");
		}
		ui::UserInterface::Update(deltaTime);
	}

	// modifiers
	void UserInterface::Pause()
	{
		cmdBar->Hide();
	}
	void UserInterface::Resume()
	{
		cmdBar->Show();
	}
}}
