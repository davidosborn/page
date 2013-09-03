#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <boost/lexical_cast.hpp>

#include "../../../util/string/operations.hpp" // Partition
#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/extract.hpp" // Extract
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/theme.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Theme.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnCursor(Theme &theme, const std::string &value)
			{
				theme.cursor = cache::ResourceProxy<Cursor>(value);
			}
			void OnListBackground(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnListBackgroundFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.background.flip);
			}
			void OnListBackgroundImage(Theme &theme, const std::string &value)
			{
				theme.list.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnListBackgroundOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.background.offset);
			}
			void OnListBackgroundScale(Theme &theme, const std::string &value)
			{
				theme.list.background.scale = boost::lexical_cast<bool>(value);
			}
			void OnListDecoration(Theme &theme, const std::string &)
			{
				theme.list.decorations.push_back(Theme::Decoration());
			}
			void OnListDecorationAngle(Theme &theme, const std::string &value)
			{
				theme.list.decorations.back().angle = boost::lexical_cast<float>(value);
			}
			void OnListDecorationBackground(Theme &theme, const std::string &value)
			{
				theme.list.decorations.back().background = boost::lexical_cast<bool>(value);
			}
			void OnListDecorationCenter(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.decorations.back().center);
			}
			void OnListDecorationFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.decorations.back().flip);
			}
			void OnListDecorationImage(Theme &theme, const std::string &value)
			{
				theme.list.decorations.back().image = cache::ResourceProxy<Image>(value);
			}
			void OnListDecorationOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.decorations.back().offset);
			}
			void OnListDecorationPosition(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.decorations.back().position);
			}
			void OnListFrame(Theme &theme, const std::string &value)
			{
				if (!value.empty())
					theme.list.frame.left.image =
						theme.list.frame.top.image =
						theme.list.frame.right.image =
						theme.list.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameFlip(Theme &theme, const std::string &value)
			{
				math::Vector<2, bool> flip;
				Extract(value, flip);
				theme.list.frame.left.flip =
					theme.list.frame.top.flip =
					theme.list.frame.right.flip =
					theme.list.frame.bottom.flip = flip;
			}
			void OnListFrameImage(Theme &theme, const std::string &value)
			{
				theme.list.frame.left.image =
					theme.list.frame.top.image =
					theme.list.frame.right.image =
					theme.list.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameMirror(Theme &theme, const std::string &value)
			{
				math::Vector<2, bool> mirror;
				Extract(value, mirror);
				if (mirror.x) theme.list.frame.right.flip.x = !theme.list.frame.right.flip.x;
				if (mirror.y) theme.list.frame.bottom.flip.y = !theme.list.frame.bottom.flip.y;
			}
			void OnListFrameBottom(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameBottomFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.frame.bottom.flip);
			}
			void OnListFrameBottomImage(Theme &theme, const std::string &value)
			{
				theme.list.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameLeft(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.frame.left.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameLeftFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.frame.left.flip);
			}
			void OnListFrameLeftImage(Theme &theme, const std::string &value)
			{
				theme.list.frame.left.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameRight(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.frame.right.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameRightFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.frame.right.flip);
			}
			void OnListFrameRightImage(Theme &theme, const std::string &value)
			{
				theme.list.frame.right.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameTop(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.frame.top.image = cache::ResourceProxy<Image>(value);
			}
			void OnListFrameTopFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.frame.top.flip);
			}
			void OnListFrameTopImage(Theme &theme, const std::string &value)
			{
				theme.list.frame.top.image = cache::ResourceProxy<Image>(value);
			}
			void OnListHighlight(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.highlight.image = cache::ResourceProxy<Image>(value);
			}
			void OnListHighlightFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.highlight.flip);
			}
			void OnListHighlightImage(Theme &theme, const std::string &value)
			{
				theme.list.highlight.image = cache::ResourceProxy<Image>(value);
			}
			void OnListHighlightOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.highlight.offset);
			}
			void OnListHighlightScale(Theme &theme, const std::string &value)
			{
				theme.list.highlight.scale = boost::lexical_cast<bool>(value);
			}
			void OnListMargin(Theme &theme, const std::string &value)
			{
				theme.list.margin = boost::lexical_cast<float>(value);
			}
			void OnListSeparator(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.list.separator.image = cache::ResourceProxy<Image>(value);
			}
			void OnListSeparatorFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.list.separator.flip);
			}
			void OnListSeparatorImage(Theme &theme, const std::string &value)
			{
				theme.list.separator.image = cache::ResourceProxy<Image>(value);
			}
			void OnListText(Theme &theme, const std::string &value)
			{
				if (!value.empty())
				{
					util::Partitioned part(util::Partition(value));
					theme.list.text.font = cache::ResourceProxy<Font>(part.first);
					if (!part.second.empty())
						theme.list.text.size = boost::lexical_cast<float>(part.second);
				}
			}
			void OnListTextFont(Theme &theme, const std::string &value)
			{
				theme.list.text.size = cache::ResourceProxy<Font>(value);
			}
			void OnListTextSize(Theme &theme, const std::string &value)
			{
				theme.list.text.size = boost::lexical_cast<float>(value);
			}
			void OnMargin(Theme &theme, const std::string &value)
			{
				theme.margin = boost::lexical_cast<float>(value);
			}
			void OnScale(Theme &theme, const std::string &value)
			{
				theme.scale = boost::lexical_cast<float>(value);
			}
			void OnText(Theme &theme, const std::string &value)
			{
				if (!value.empty())
				{
					util::Partitioned part(util::Partition(value));
					theme.text.font = cache::ResourceProxy<Font>(part.first);
					if (!part.second.empty())
						theme.text.size = boost::lexical_cast<float>(part.second);
				}
			}
			void OnTextFont(Theme &theme, const std::string &value)
			{
				theme.text.font = cache::ResourceProxy<Font>(value);
			}
			void OnTextSize(Theme &theme, const std::string &value)
			{
				theme.text.size = boost::lexical_cast<float>(value);
			}
			void OnWindowBackground(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowBackgroundFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.background.flip);
			}
			void OnWindowBackgroundImage(Theme &theme, const std::string &value)
			{
				theme.window.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowBackgroundOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.background.offset);
			}
			void OnWindowBackgroundScale(Theme &theme, const std::string &value)
			{
				theme.window.background.scale = boost::lexical_cast<bool>(value);
			}
			void OnWindowDecoration(Theme &theme, const std::string &)
			{
				theme.window.decorations.push_back(Theme::Decoration());
			}
			void OnWindowDecorationAngle(Theme &theme, const std::string &value)
			{
				theme.window.decorations.back().angle = boost::lexical_cast<float>(value);
			}
			void OnWindowDecorationBackground(Theme &theme, const std::string &value)
			{
				theme.window.decorations.back().background = boost::lexical_cast<bool>(value);
			}
			void OnWindowDecorationCenter(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.decorations.back().center);
			}
			void OnWindowDecorationFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.decorations.back().flip);
			}
			void OnWindowDecorationImage(Theme &theme, const std::string &value)
			{
				theme.window.decorations.back().image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowDecorationOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.decorations.back().offset);
			}
			void OnWindowDecorationPosition(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.decorations.back().position);
			}
			void OnWindowFrame(Theme &theme, const std::string &value)
			{
				if (!value.empty())
					theme.window.frame.left.image =
						theme.window.frame.top.image =
						theme.window.frame.right.image =
						theme.window.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameFlip(Theme &theme, const std::string &value)
			{
				math::Vector<2, bool> flip;
				Extract(value, flip);
				theme.window.frame.left.flip =
					theme.window.frame.top.flip =
					theme.window.frame.right.flip =
					theme.window.frame.bottom.flip = flip;
			}
			void OnWindowFrameImage(Theme &theme, const std::string &value)
			{
				theme.window.frame.left.image =
					theme.window.frame.top.image =
					theme.window.frame.right.image =
					theme.window.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameMirror(Theme &theme, const std::string &value)
			{
				math::Vector<2, bool> mirror;
				Extract(value, mirror);
				if (mirror.x) theme.window.frame.right.flip.x = !theme.window.frame.right.flip.x;
				if (mirror.y) theme.window.frame.bottom.flip.y = !theme.window.frame.bottom.flip.y;
			}
			void OnWindowFrameBottom(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameBottomFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.frame.bottom.flip);
			}
			void OnWindowFrameBottomImage(Theme &theme, const std::string &value)
			{
				theme.window.frame.bottom.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameLeft(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.frame.left.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameLeftFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.frame.left.flip);
			}
			void OnWindowFrameLeftImage(Theme &theme, const std::string &value)
			{
				theme.window.frame.left.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameRight(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.frame.right.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameRightFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.frame.right.flip);
			}
			void OnWindowFrameRightcache::ResourceProxy<(Theme &theme, const std::string &value)
			{
				theme.window.frame.right.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameTop(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.frame.top.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowFrameTopFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.frame.top.flip);
			}
			void OnWindowFrameTopImage(Theme &theme, const std::string &value)
			{
				theme.window.frame.top.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowMargin(Theme &theme, const std::string &value)
			{
				theme.window.margin = boost::lexical_cast<float>(value);
			}
			void OnWindowTitle(Theme &theme, const std::string &value)
			{
				if (!value.empty())
				{
					util::Partitioned part(util::Partition(value));
					theme.window.title.text.font = cache::ResourceProxy<Font>(part.first);
					if (!part.second.empty())
						theme.window.title.text.size = boost::lexical_cast<float>(part.second);
				}
			}
			void OnWindowTitleBackground(Theme &theme, const std::string &value)
			{
				if (!value.empty()) theme.window.title.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowTitleBackgroundFlip(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.title.background.flip);
			}
			void OnWindowTitleBackgroundImage(Theme &theme, const std::string &value)
			{
				theme.window.title.background.image = cache::ResourceProxy<Image>(value);
			}
			void OnWindowTitleBackgroundOffset(Theme &theme, const std::string &value)
			{
				Extract(value, theme.window.title.background.offset);
			}
			void OnWindowTitleBackgroundScale(Theme &theme, const std::string &value)
			{
				theme.window.background.scale = boost::lexical_cast<bool>(value);
			}
			void OnWindowTitleText(Theme &theme, const std::string &value)
			{
				if (!value.empty())
				{
					util::Partitioned part(util::Partition(value));
					theme.window.title.text.font = cache::ResourceProxy<Font>(part.first);
					if (!part.second.empty())
						theme.window.title.text.size = boost::lexical_cast<float>(part.second);
				}
			}
			void OnWindowTitleTextFont(Theme &theme, const std::string &value)
			{
				theme.window.title.text.size = cache::ResourceProxy<Font>(value);
			}
			void OnWindowTitleTextSize(Theme &theme, const std::string &value)
			{
				theme.window.title.text.size = boost::lexical_cast<float>(value);
			}

			// parser factory
			inline Parser GetParser(Theme &theme)
			{
				Parser parser;
				parser.Register("cursor",                         Bind(OnCursor,                      theme), Parser::needValue);
				parser.Register("list");
				parser.Register("list.background",                Bind(OnListBackground,              theme));
				parser.Register("list.background.flip",           Bind(OnListBackgroundFlip,          theme), Parser::needValue);
				parser.Register("list.background.image",          Bind(OnListBackgroundImage,         theme), Parser::needValue);
				parser.Register("list.background.offset",         Bind(OnListBackgroundOffset,        theme), Parser::needValue);
				parser.Register("list.background.scale",          Bind(OnListBackgroundScale,         theme), Parser::needValue);
				parser.Register("list.decoration",                Bind(OnListDecoration,              theme), Parser::noValue);
				parser.Register("list.decoration.angle",          Bind(OnListDecorationAngle,         theme), Parser::needValue);
				parser.Register("list.decoration.background",     Bind(OnListDecorationBackground,    theme), Parser::needValue);
				parser.Register("list.decoration.center",         Bind(OnListDecorationCenter,        theme), Parser::needValue);
				parser.Register("list.decoration.flip",           Bind(OnListDecorationFlip,          theme), Parser::needValue);
				parser.Register("list.decoration.image",          Bind(OnListDecorationImage,         theme), Parser::needValue);
				parser.Register("list.decoration.offset",         Bind(OnListDecorationOffset,        theme), Parser::needValue);
				parser.Register("list.decoration.position",       Bind(OnListDecorationPosition,      theme), Parser::needValue);
				parser.Register("list.frame",                     Bind(OnListFrame,                   theme));
				parser.Register("list.frame.flip",                Bind(OnListFrameFlip,               theme), Parser::needValue);
				parser.Register("list.frame.image",               Bind(OnListFrameImage,              theme), Parser::needValue);
				parser.Register("list.frame.mirror",              Bind(OnListFrameMirror,             theme), Parser::needValue);
				parser.Register("list.frame.bottom",              Bind(OnListFrameBottom,             theme));
				parser.Register("list.frame.bottom.flip",         Bind(OnListFrameBottomFlip,         theme), Parser::needValue);
				parser.Register("list.frame.bottom.image",        Bind(OnListFrameBottomImage,        theme), Parser::needValue);
				parser.Register("list.frame.left",                Bind(OnListFrameLeft,               theme));
				parser.Register("list.frame.left.flip",           Bind(OnListFrameLeftFlip,           theme), Parser::needValue);
				parser.Register("list.frame.left.image",          Bind(OnListFrameLeftImage,          theme), Parser::needValue);
				parser.Register("list.frame.right",               Bind(OnListFrameRight,              theme));
				parser.Register("list.frame.right.flip",          Bind(OnListFrameRightFlip,          theme), Parser::needValue);
				parser.Register("list.frame.right.image",         Bind(OnListFrameRightImage,         theme), Parser::needValue);
				parser.Register("list.frame.top",                 Bind(OnListFrameTop,                theme));
				parser.Register("list.frame.top.flip",            Bind(OnListFrameTopFlip,            theme), Parser::needValue);
				parser.Register("list.frame.top.image",           Bind(OnListFrameTopImage,           theme), Parser::needValue);
				parser.Register("list.highlight",                 Bind(OnListHighlight,               theme));
				parser.Register("list.highlight.flip",            Bind(OnListHighlightFlip,           theme), Parser::needValue);
				parser.Register("list.highlight.image",           Bind(OnListHighlightImage,          theme), Parser::needValue);
				parser.Register("list.highlight.offset",          Bind(OnListHighlightOffset,         theme), Parser::needValue);
				parser.Register("list.highlight.scale",           Bind(OnListHighlightScale,          theme), Parser::needValue);
				parser.Register("list.margin",                    Bind(OnListMargin,                  theme), Parser::needValue);
				parser.Register("list.separator",                 Bind(OnListSeparator,               theme));
				parser.Register("list.separator.flip",            Bind(OnListSeparatorFlip,           theme), Parser::needValue);
				parser.Register("list.separator.image",           Bind(OnListSeparatorImage,          theme), Parser::needValue);
				parser.Register("list.text",                      Bind(OnListText,                    theme));
				parser.Register("list.text.font",                 Bind(OnListTextFont,                theme), Parser::needValue);
				parser.Register("list.text.size",                 Bind(OnListTextSize,                theme), Parser::needValue);
				parser.Register("margin",                         Bind(OnMargin,                      theme), Parser::needValue);
				parser.Register("scale",                          Bind(OnScale,                       theme), Parser::needValue);
				parser.Register("text",                           Bind(OnText,                        theme));
				parser.Register("text.font",                      Bind(OnTextFont,                    theme), Parser::needValue);
				parser.Register("text.size",                      Bind(OnTextSize,                    theme), Parser::needValue);
				parser.Register("window");
				parser.Register("window.background",              Bind(OnWindowBackground,            theme));
				parser.Register("window.background.flip",         Bind(OnWindowBackgroundFlip,        theme), Parser::needValue);
				parser.Register("window.background.image",        Bind(OnWindowBackgroundImage,       theme), Parser::needValue);
				parser.Register("window.background.offset",       Bind(OnWindowBackgroundOffset,      theme), Parser::needValue);
				parser.Register("window.background.scale",        Bind(OnWindowBackgroundScale,       theme), Parser::needValue);
				parser.Register("window.decoration",              Bind(OnWindowDecoration,            theme), Parser::noValue);
				parser.Register("window.decoration.angle",        Bind(OnWindowDecorationAngle,       theme), Parser::needValue);
				parser.Register("window.decoration.background",   Bind(OnWindowDecorationBackground,  theme), Parser::needValue);
				parser.Register("window.decoration.center",       Bind(OnWindowDecorationCenter,      theme), Parser::needValue);
				parser.Register("window.decoration.flip",         Bind(OnWindowDecorationFlip,        theme), Parser::needValue);
				parser.Register("window.decoration.image",        Bind(OnWindowDecorationImage,       theme), Parser::needValue);
				parser.Register("window.decoration.offset",       Bind(OnWindowDecorationOffset,      theme), Parser::needValue);
				parser.Register("window.decoration.position",     Bind(OnWindowDecorationPosition,    theme), Parser::needValue);
				parser.Register("window.frame",                   Bind(OnWindowFrame,                 theme));
				parser.Register("window.frame.flip",              Bind(OnWindowFrameFlip,             theme), Parser::needValue);
				parser.Register("window.frame.image",             Bind(OnWindowFrameImage,            theme), Parser::needValue);
				parser.Register("window.frame.mirror",            Bind(OnWindowFrameMirror,           theme), Parser::needValue);
				parser.Register("window.frame.bottom",            Bind(OnWindowFrameBottom,           theme));
				parser.Register("window.frame.bottom.flip",       Bind(OnWindowFrameBottomFlip,       theme), Parser::needValue);
				parser.Register("window.frame.bottom.image",      Bind(OnWindowFrameBottomImage,      theme), Parser::needValue);
				parser.Register("window.frame.left",              Bind(OnWindowFrameLeft,             theme));
				parser.Register("window.frame.left.flip",         Bind(OnWindowFrameLeftFlip,         theme), Parser::needValue);
				parser.Register("window.frame.left.image",        Bind(OnWindowFrameLeftImage,        theme), Parser::needValue);
				parser.Register("window.frame.right",             Bind(OnWindowFrameRight,            theme));
				parser.Register("window.frame.right.flip",        Bind(OnWindowFrameRightFlip,        theme), Parser::needValue);
				parser.Register("window.frame.right.image",       Bind(OnWindowFrameRightImage,       theme), Parser::needValue);
				parser.Register("window.frame.top",               Bind(OnWindowFrameTop,              theme));
				parser.Register("window.frame.top.flip",          Bind(OnWindowFrameTopFlip,          theme), Parser::needValue);
				parser.Register("window.frame.top.image",         Bind(OnWindowFrameTopImage,         theme), Parser::needValue);
				parser.Register("window.margin",                  Bind(OnWindowMargin,                theme), Parser::needValue);
				parser.Register("window.title",                   Bind(OnWindowTitle,                 theme));
				parser.Register("window.title.background",        Bind(OnWindowTitleBackground,       theme));
				parser.Register("window.title.background.flip",   Bind(OnWindowTitleBackgroundFlip,   theme), Parser::needValue);
				parser.Register("window.title.background.image",  Bind(OnWindowTitleBackgroundImage,  theme), Parser::needValue);
				parser.Register("window.title.background.offset", Bind(OnWindowTitleBackgroundOffset, theme), Parser::needValue);
				parser.Register("window.title.background.scale",  Bind(OnWindowTitleBackgroundScale,  theme), Parser::needValue);
				parser.Register("window.title.text",              Bind(OnWindowTitleText,             theme));
				parser.Register("window.title.text.font",         Bind(OnWindowTitleTextFont,         theme), Parser::needValue);
				parser.Register("window.title.text.size",         Bind(OnWindowTitleTextSize,         theme), Parser::needValue);
				return parser;
			}
		}

		Theme *LoadNativeTheme(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Theme> theme(new Theme);
			GetParser(*theme).Parse(*stream);
			return theme.release();
		}

		LoadFunction GetNativeThemeLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeTheme : LoadFunction();
		}

		REGISTER_LOADER(Theme, GetNativeThemeLoader,
			"theme,pagetheme", "", "", true)
	}
}
