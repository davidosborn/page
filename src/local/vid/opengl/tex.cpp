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

#include <algorithm> // transform
#include <cassert>
#include <climits> // CHAR_BIT

#ifdef HAVE_GLU
#	include <GL/glu.h> // gluBuild2DMipmaps, gluScaleImage
#endif

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../math/pow2.hpp" // Pow2Ceil
#include "ext.hpp" // ARB_texture_non_power_of_two, EXT_{abgr,bgra}, {EXT,SGIS}_texture_edge_clamp, SGIS_generate_mipmap
#include "tex.hpp" // Compatibility

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				GLuint MakeTextureFromData(const res::Image::Data &data, const math::Vec2u &size, GLint internalFormat, GLenum format, GLenum type, bool mipmap, const math::Vector<2, bool> clamp)
				{
					// determine compatible size
					math::Vector<2, GLsizei> texSize(Max(size >> *CVAR(opengl)::renderTextureDown, 1));
					if (!haveArbTextureNonPowerOfTwo)
						std::transform(texSize.begin(), texSize.end(), texSize.begin(), math::Pow2Ceil);
					res::Image::Data newData;
					// scale to fit
					if (Any(texSize != size))
					{
#ifdef HAVE_GLU
						unsigned depth;
						switch (format)
						{
							case GL_RED:
							case GL_GREEN:
							case GL_BLUE:
							case GL_ALPHA:
							case GL_LUMINANCE: depth = 1; break;
							case GL_LUMINANCE_ALPHA: depth = 2; break;
							case GL_RGB:
							case GL_BGR_EXT: depth = 3; break;
							case GL_RGBA:
							case GL_BGRA_EXT: depth = 4; break;
							default: assert(!"invalid texture format");
						}
						switch (type)
						{
							case GL_UNSIGNED_BYTE:  depth *= 1; break;
							case GL_UNSIGNED_SHORT: depth *= 2; break;
							case GL_UNSIGNED_INT:   depth *= 4; break;
							default: assert(!"invalid texture type");
						}
						res::Image::Data scaledData(Content(texSize) * depth);
						if (gluScaleImage(format, size.x, size.y, type, &*(newData.empty() ? data : newData).begin(), texSize.x, texSize.y, type, &*scaledData.begin()))
							THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to scale image")))
						newData.swap(scaledData);
#else
						// FIXME: implement; use Scale(res::Image)
#endif
					}
					// generate texture
					GLuint tex;
					if (glGenTextures(1, &tex), glGetError())
						THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to generate texture")))
					glBindTexture(GL_TEXTURE_2D, tex);
					if (mipmap && *CVAR(opengl)::renderTextureMipmap)
					{
						if (haveSgisGenerateMipmap)
						{
							glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
							glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texSize.x, texSize.y, 0, format, type, &*(newData.empty() ? data : newData).begin());
						}
						else
#ifdef HAVE_GLU
							gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, texSize.x, texSize.y, format, type, &*(newData.empty() ? data : newData).begin());
#else
							glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texSize.x, texSize.y, 0, format, type, &*(newData.empty() ? data : newData).begin());
#endif
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					}
					else
					{
						glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texSize.x, texSize.y, 0, format, type, &*(newData.empty() ? data : newData).begin());
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
					// initialize clamp state
					if (haveExtTextureEdgeClamp || haveSgisTextureEdgeClamp)
					{
						if (clamp.x) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_SGIS);
						if (clamp.y) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_SGIS);
					}
					else
					{
						if (clamp.x) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
						if (clamp.y) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
						// HACK: to prevent interpolation at edges, disable
						// linear filtering
						if (mipmap && *CVAR(opengl)::renderTextureMipmap)
						{
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						}
						else
						{
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						}
					}
					if (glGetError())
						THROW((err::Exception<err::VidModuleTag, err::OpenglPlatformTag>("failed to initialize texture")))
					return tex;
				}
			}

			Compatibility GetCompatibility(const res::Image &img)
			{
				Compatibility compat = {};
				// check for exactly matching format
				switch (img.channels.size())
				{
					case 1:
					switch (img.channels[0].type)
					{
						case res::Image::Channel::red:   compat.format = GL_RED;       break;
						case res::Image::Channel::green: compat.format = GL_GREEN;     break;
						case res::Image::Channel::blue:  compat.format = GL_BLUE;      break;
						case res::Image::Channel::alpha: compat.format = GL_ALPHA;     break;
						case res::Image::Channel::gray:  compat.format = GL_LUMINANCE; break;
						case res::Image::Channel::mono:  compat.format = GL_INTENSITY; break;
					}
					break;
					case 2:
					if (img.channels[0].type == res::Image::Channel::gray &&
						img.channels[1].type == res::Image::Channel::alpha)
						compat.format = GL_LUMINANCE_ALPHA;
					break;
					case 3:
					if (img.channels[0].type == res::Image::Channel::red &&
						img.channels[1].type == res::Image::Channel::green &&
						img.channels[2].type == res::Image::Channel::blue)
						compat.format = GL_RGB;
					else if (haveExtBgra &&
						img.channels[0].type == res::Image::Channel::blue &&
						img.channels[1].type == res::Image::Channel::green &&
						img.channels[2].type == res::Image::Channel::red)
						compat.format = GL_BGR_EXT;
					break;
					case 4:
					if (img.channels[0].type == res::Image::Channel::red &&
						img.channels[1].type == res::Image::Channel::green &&
						img.channels[2].type == res::Image::Channel::blue &&
						img.channels[3].type == res::Image::Channel::alpha)
						compat.format = GL_RGBA;
					else if (haveExtAbgr &&
						img.channels[0].type == res::Image::Channel::alpha &&
						img.channels[1].type == res::Image::Channel::blue &&
						img.channels[2].type == res::Image::Channel::green &&
						img.channels[3].type == res::Image::Channel::red)
						compat.format = GL_ABGR_EXT;
					else if (haveExtBgra &&
						img.channels[1].type == res::Image::Channel::blue &&
						img.channels[2].type == res::Image::Channel::green &&
						img.channels[3].type == res::Image::Channel::red &&
						img.channels[0].type == res::Image::Channel::alpha)
						compat.format = GL_BGRA_EXT;
					break;
				}
				if (compat.format && !img.channels.empty())
				{
					// check for exactly matching depth
					for (res::Image::Channels::const_iterator channel(img.channels.begin() + 1);; ++channel)
					{
						if (channel == img.channels.end())
						{
							switch (img.channels[0].depth)
							{
								case CHAR_BIT * sizeof(GLubyte):  compat.type = GL_UNSIGNED_BYTE;  break;
								case CHAR_BIT * sizeof(GLushort): compat.type = GL_UNSIGNED_SHORT; break;
								case CHAR_BIT * sizeof(GLuint):   compat.type = GL_UNSIGNED_INT;   break;
							}
							break;
						}
						if (channel->depth != img.channels[0].depth) break;
					}
				}
				compat.internalFormat = img.channels.size();
				if (compat.format && compat.type)
				{
					// select matching internal format
					switch (compat.format)
					{
						case GL_RED:
						case GL_GREEN:
						case GL_BLUE:
						case GL_RGB:
						case GL_BGR_EXT:
						switch (compat.type)
						{
							case GL_UNSIGNED_BYTE:  compat.internalFormat = GL_RGB8;  break;
							case GL_UNSIGNED_SHORT: compat.internalFormat = GL_RGB16; break;
							default:                compat.internalFormat = GL_RGB;
						}
						break;
						case GL_RGBA:
						case GL_ABGR_EXT:
						case GL_BGRA_EXT:
						switch (compat.type)
						{
							case GL_UNSIGNED_BYTE:  compat.internalFormat = GL_RGBA8;  break;
							case GL_UNSIGNED_SHORT: compat.internalFormat = GL_RGBA16; break;
							default:                compat.internalFormat = GL_RGBA;
						}
						break;
						case GL_ALPHA:
						switch (compat.type)
						{
							case GL_UNSIGNED_BYTE:  compat.internalFormat = GL_ALPHA8;  break;
							case GL_UNSIGNED_SHORT: compat.internalFormat = GL_ALPHA16; break;
							default:                compat.internalFormat = GL_ALPHA;
						}
						break;
						case GL_LUMINANCE:
						switch (compat.type)
						{
							case GL_UNSIGNED_BYTE:  compat.internalFormat = GL_LUMINANCE8;  break;
							case GL_UNSIGNED_SHORT: compat.internalFormat = GL_LUMINANCE16; break;
							default:                compat.internalFormat = GL_LUMINANCE;
						}
						break;
						case GL_INTENSITY:
						switch (compat.type)
						{
							case GL_UNSIGNED_BYTE:  compat.internalFormat = GL_INTENSITY8;  break;
							case GL_UNSIGNED_SHORT: compat.internalFormat = GL_INTENSITY16; break;
							default:                compat.internalFormat = GL_INTENSITY;
						}
						break;
						default: assert(!"invalid texture format");
					}
				}
				else
				{
					if (!compat.format)
					{
						// find closest compatible format
						if (HasColor(img.channels))
						{
							if (HasAlpha(img.channels))
							{
								compat.format = GL_INTENSITY;
								for (res::Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
									if (channel->type == res::Image::Channel::gray ||
										channel->type == res::Image::Channel::alpha)
									{
										compat.format = GL_LUMINANCE_ALPHA;
										break;
									}
								for (res::Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
									if (channel->type == res::Image::Channel::red ||
										channel->type == res::Image::Channel::green ||
										channel->type == res::Image::Channel::blue)
									{
										compat.format = GL_RGBA;
										break;
									}
							}
							else
							{
								compat.format = GL_LUMINANCE;
								for (res::Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
									if (channel->type == res::Image::Channel::red ||
										channel->type == res::Image::Channel::green ||
										channel->type == res::Image::Channel::blue)
									{
										compat.format = GL_RGB;
										break;
									}
							}
						}
						else compat.format = HasAlpha(img.channels) ? GL_ALPHA : GL_LUMINANCE;
					}
					// find closest compatible internal format
					// FIXME: use the original channel depths to find the best
					// fitting internal format
					switch (compat.format)
					{
						case GL_ALPHA:           compat.internalFormat = GL_ALPHA;           break;
						case GL_INTENSITY:       compat.internalFormat = GL_INTENSITY;       break;
						case GL_LUMINANCE:       compat.internalFormat = GL_LUMINANCE;       break;
						case GL_LUMINANCE_ALPHA: compat.internalFormat = GL_LUMINANCE_ALPHA; break;
						case GL_RGB:             compat.internalFormat = GL_RGB;             break;
						case GL_RGBA:            compat.internalFormat = GL_RGBA;            break;
						default: assert(!"invalid texture format");
					}
					// find minimum compatible depth
					unsigned depth = GetComponentDepth(img.channels);
					if (depth <= CHAR_BIT * sizeof(GLubyte))
					{
						compat.type = GL_UNSIGNED_BYTE;
						depth = CHAR_BIT * sizeof(GLubyte);
					}
					else if (depth <= CHAR_BIT * sizeof(GLushort))
					{
						compat.type = GL_UNSIGNED_SHORT;
						depth = CHAR_BIT * sizeof(GLushort);
					}
					else
					{
						compat.type = GL_UNSIGNED_INT;
						depth = CHAR_BIT * sizeof(GLuint);
					}
					// build compatible channels
					switch (compat.format)
					{
						case GL_ALPHA:
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::alpha, depth));
						break;
						case GL_INTENSITY:
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::mono, depth));
						break;
						case GL_LUMINANCE:
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::gray, depth));
						break;
						case GL_LUMINANCE_ALPHA:
						compat.channels.reserve(2);
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::gray, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::alpha, depth));
						break;
						case GL_RGB:
						compat.channels.reserve(3);
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::red, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::green, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::blue, depth));
						break;
						case GL_RGBA:
						compat.channels.reserve(4);
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::red, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::green, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::blue, depth));
						compat.channels.push_back(res::Image::Channel(res::Image::Channel::alpha, depth));
						break;
						default: assert(!"invalid texture format");
					}
				}
				if (compat.format == GL_INTENSITY) compat.format = GL_LUMINANCE;
				return compat;
			}

			GLuint MakeTexture(const res::Image &img, bool mipmap, const math::Vector<2, bool> &clamp)
			{
				// convert image to compatible format
				Compatibility compat(GetCompatibility(img));
				res::Image scratchImg;
				const res::Image *compatImg = &img;
				if (!compat.channels.empty())
				{
					scratchImg = Convert(*compatImg, compat.channels, 1);
					compatImg = &scratchImg;
				}
				else if (!IsAligned(*compatImg, 1))
				{
					scratchImg = Align(*compatImg, 1);
					compatImg = &scratchImg;
				}
				// create texture
				return MakeTextureFromData(compatImg->data, compatImg->size,
					compat.internalFormat, compat.format, compat.type, mipmap, clamp);
			}
			GLuint MakeAlphaTexture(const res::Image &img, bool mipmap, const math::Vector<2, bool> &clamp)
			{
				// determine best texture format
				res::Image::Channel::Type imgType;
				unsigned imgDepth;
				if (HasAlpha(img.channels))
				{
					imgType = res::Image::Channel::alpha;
					imgDepth = GetAlphaDepth(img.channels);
				}
				else
				{
					imgType = res::Image::Channel::gray;
					imgDepth = GetColorDepth(img.channels);
				}
				GLint internalFormat;
				GLenum type;
				unsigned depth;
				if (imgDepth <= sizeof(GLubyte))
				{
					internalFormat = imgDepth <= 4 ? GL_ALPHA4 : GL_ALPHA8;
					type = GL_UNSIGNED_BYTE;
					depth = CHAR_BIT * sizeof(GLubyte);
				}
				else
				{
					internalFormat = imgDepth <= 12 ? GL_ALPHA12 : GL_ALPHA16;
					type = GL_UNSIGNED_SHORT;
					depth = CHAR_BIT * sizeof(GLushort);
				}
				// convert image to compatible format
				res::Image scratchImg;
				const res::Image *compatImg = &img;
				if (!compatImg->channels.size() != 1 || depth != imgDepth)
				{
					scratchImg = Convert(*compatImg, res::Image::Channels(1, res::Image::Channel(imgType, depth)), 1);
					compatImg = &scratchImg;
				}
				else if (!IsAligned(*compatImg, 1))
				{
					scratchImg = Align(*compatImg, 1);
					compatImg = &scratchImg;
				}
				// create texture
				return MakeTextureFromData(compatImg->data, compatImg->size,
					internalFormat, GL_ALPHA, type, mipmap, clamp);
			}
			GLuint MakeLuminanceTexture(const res::Image &img, bool mipmap, const math::Vector<2, bool> &clamp)
			{
				// determine best texture format
				res::Image::Channel::Type imgType;
				unsigned imgDepth;
				if (HasColor(img.channels))
				{
					imgType = res::Image::Channel::gray;
					imgDepth = GetColorDepth(img.channels);
				}
				else
				{
					imgType = res::Image::Channel::alpha;
					imgDepth = GetAlphaDepth(img.channels);
				}
				GLint internalFormat;
				GLenum type;
				unsigned depth;
				if (imgDepth <= sizeof(GLubyte))
				{
					internalFormat = imgDepth <= 4 ? GL_LUMINANCE4 : GL_LUMINANCE8;
					type = GL_UNSIGNED_BYTE;
					depth = CHAR_BIT * sizeof(GLubyte);
				}
				else
				{
					internalFormat = imgDepth <= 12 ? GL_LUMINANCE12 : GL_LUMINANCE16;
					type = GL_UNSIGNED_SHORT;
					depth = CHAR_BIT * sizeof(GLushort);
				}
				// convert image to compatible format
				res::Image scratchImg;
				const res::Image *compatImg = &img;
				if (!compatImg->channels.size() != 1 || depth != imgDepth)
				{
					scratchImg = Convert(*compatImg, res::Image::Channels(1, res::Image::Channel(imgType, depth)), 1);
					compatImg = &scratchImg;
				}
				else if (!IsAligned(*compatImg, 1))
				{
					scratchImg = Align(*compatImg, 1);
					compatImg = &scratchImg;
				}
				// create texture
				return MakeTextureFromData(compatImg->data, compatImg->size,
					internalFormat, GL_LUMINANCE, type, mipmap, clamp);
			}
		}
	}
}
