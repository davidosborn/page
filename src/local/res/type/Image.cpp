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
 *
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

#include <algorithm> // copy, max, min
#include <cassert>
#include <climits> // CHAR_BIT, UCHAR_MAX
#include <cmath> // fmod
#include <cstdlib> // abs
#include <vector>
#include "../../math/Aabb.hpp"
#include "../../util/bit.hpp" // BitCopy
#include "Image.hpp"
#include "register.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		namespace
		{
			// conversion steps
			struct Step
			{
				struct Component
				{
					Component(unsigned offset, unsigned depth) :
						offset(offset), depth(depth) {}

					unsigned offset, depth;
				};
				typedef std::vector<Component> Components;
				Components sources, targets;
				bool alpha;
			};
			typedef std::vector<Step> Steps;

			// conversion step comparison
			inline bool operator ==(const Step::Component &a, const Step::Component &b)
				{ return a.offset == b.offset && a.depth == b.depth; }
			inline bool operator !=(const Step::Component &a, const Step::Component &b)
				{ return a.offset != b.offset || a.depth != b.depth; }

			// alignment calculations
			inline unsigned GetBytePitch(unsigned width, unsigned depth, unsigned alignment)
			{
				assert(alignment);
				width = (width * depth + CHAR_BIT - 1) / CHAR_BIT;
				return width + (alignment - width % alignment) % alignment;
			}
			inline unsigned GetBitPitch(unsigned width, unsigned depth, unsigned alignment)
			{
				return alignment ? GetBytePitch(width, depth, alignment) * CHAR_BIT : width * depth;
			}
			inline unsigned GetPadding(unsigned width, unsigned depth, unsigned alignment)
			{
				if (!alignment) return 0;
				alignment *= CHAR_BIT;
				return ((alignment - width * depth % alignment) % alignment + CHAR_BIT - 1) / CHAR_BIT;
			}
			inline unsigned GetDataSize(const math::Vector<2, unsigned> &size, unsigned depth, unsigned alignment)
			{
				return alignment ?
					GetBytePitch(size.x, depth, alignment) * size.y :
					(Content(size) * depth + CHAR_BIT - 1) / CHAR_BIT;
			}

			// specialized conversions
			Image::Data ConvertBitToByte(const Image &img, unsigned alignment)
			{
				Image::Data data(GetDataSize(img.size, CHAR_BIT, alignment));
				Image::Data::const_iterator src(img.data.begin());
				Image::Data::iterator dest(data.begin());
				unsigned bit = 0,
					srcPadding  = GetPadding(img.size.x, 1, img.alignment),
					destPadding = GetPadding(img.size.x, CHAR_BIT, alignment);
				for (unsigned i = 0; i < img.size.y; ++i)
				{
					for (unsigned i = 0; i < img.size.x; ++i)
					{
						*dest++ = (*src >> (CHAR_BIT - 1) - bit++ & 1) * UCHAR_MAX;
						if (bit == CHAR_BIT)
						{
							++src;
							bit = 0;
						}
					}
					if (img.alignment)
					{
						src += srcPadding;
						bit = 0;
					}
					dest += destPadding;
				}
				return data;
			}
			Image::Data SwizzleBytes(const Image &img, const Image::Channels &channels, const Steps &steps, unsigned alignment)
			{
				unsigned
					srcDepth  = GetDepth(img.channels),
					destDepth = GetDepth(channels);
				Image::Data data(GetDataSize(img.size, destDepth, alignment));
				Image::Data::const_iterator src(img.data.begin());
				Image::Data::iterator dest(data.begin());
				unsigned
					srcPadding  = GetPadding(img.size.x, srcDepth,  img.alignment),
					destPadding = GetPadding(img.size.x, destDepth, alignment);
				srcDepth  /= CHAR_BIT;
				destDepth /= CHAR_BIT;
				for (unsigned i = 0; i < img.size.y; ++i)
				{
					for (unsigned i = 0; i < img.size.x; ++i)
					{
						for (Steps::const_iterator step(steps.begin()); step != steps.end(); ++step)
							dest[step->targets[0].offset / CHAR_BIT] = src[step->sources[0].offset / CHAR_BIT];
						src  += srcDepth;
						dest += destDepth;
					}
					src  += srcPadding;
					dest += destPadding;
				}
				return data;
			}
			Image::Data ConvertBytes(const Image &img, const Image::Channels &channels, const Steps &steps, unsigned alignment)
			{
				unsigned
					srcDepth  = GetDepth(img.channels),
					destDepth = GetDepth(channels);
				Image::Data data(GetDataSize(img.size, destDepth, alignment));
				Image::Data::const_iterator src(img.data.begin());
				Image::Data::iterator dest(data.begin());
				unsigned
					srcPadding  = GetPadding(img.size.x, srcDepth,  img.alignment),
					destPadding = GetPadding(img.size.x, destDepth, alignment);
				srcDepth  /= CHAR_BIT;
				destDepth /= CHAR_BIT;
				for (unsigned i = 0; i < img.size.y; ++i)
				{
					for (unsigned i = 0; i < img.size.x; ++i)
					{
						for (Steps::const_iterator step(steps.begin()); step != steps.end(); ++step)
						{
							unsigned value = step->alpha * UCHAR_MAX;
							if (!step->sources.empty())
							{
								value = 0;
								for (Step::Components::const_iterator source(step->sources.begin()); source != step->sources.end(); ++source)
									value += src[source->offset / CHAR_BIT];
								value /= step->sources.size();
							}
							for (Step::Components::const_iterator target(step->targets.begin()); target != step->targets.end(); ++target)
								dest[target->offset / CHAR_BIT] = value;
						}
						src  += srcDepth;
						dest += destDepth;
					}
					src  += srcPadding;
					dest += destPadding;
				}
				return data;
			}

			// component extraction/insertion
			inline float GetComponent(Image::Data::const_iterator iter, unsigned bitOffset, unsigned bitSize)
			{
				iter += bitOffset / CHAR_BIT;
				bitOffset %= CHAR_BIT;
				unsigned value = 0;
				unsigned char *valuePtr = reinterpret_cast<unsigned char *>(&value);
				unsigned
					valueDepth = std::min(bitSize, sizeof value * CHAR_BIT),
					valueShift = (CHAR_BIT - valueDepth % CHAR_BIT) % CHAR_BIT;
#ifdef WORDS_BIGENDIAN
				valuePtr += sizeof value - (valueDepth + CHAR_BIT - 1) / CHAR_BIT;
#endif
				unsigned n = valueDepth;
				// copy full bytes
				if (n > CHAR_BIT)
				{
					if (bitOffset)
					{
						do
						{
							*valuePtr = *iter++ << bitOffset;
							*valuePtr++ |= *iter >> CHAR_BIT - bitOffset;
						}
						while ((n -= CHAR_BIT) > CHAR_BIT);
					}
					else
					{
						do
						{
							*valuePtr++ = *iter++;
						}
						while ((n -= CHAR_BIT) > CHAR_BIT);
					}
				}
				// copy remaining bits
				if (n > CHAR_BIT - bitOffset)
				{
					*valuePtr = *iter++ << bitOffset;
					*valuePtr |= *iter >> CHAR_BIT - bitOffset & UCHAR_MAX << CHAR_BIT - n;
				}
				else
				{
					*valuePtr = *iter << bitOffset & UCHAR_MAX << CHAR_BIT - n;
				}
				// normalize partial byte
#ifdef WORDS_BIGENDIAN
				*reinterpret_cast<unsigned char *>(&value)
#else
				*valuePtr
#endif
					>>= valueShift;
				return float(value) / ((1u << valueDepth) - 1);
			}
			inline void SetComponent(Image::Data::iterator iter, unsigned bitOffset, unsigned bitSize, float scalar)
			{
				iter += bitOffset / CHAR_BIT;
				bitOffset %= CHAR_BIT;
				unsigned value;
				unsigned char *valuePtr = reinterpret_cast<unsigned char *>(&value);
				unsigned n = std::min(bitSize, sizeof value * CHAR_BIT);
				value = unsigned(std::min(scalar, 1.f) * ((1u << n) - 1));
#ifdef WORDS_BIGENDIAN
				// pack partial word
				value <<= (sizeof destValue - (n + CHAR_BIT - 1) / CHAR_BIT) * CHAR_BIT;
#endif
				// pack partial byte
#ifdef WORDS_BIGENDIAN
				*valuePtr
#else
				reinterpret_cast<unsigned char *>(&value)[(n - 1) / CHAR_BIT]
#endif
					<<= CHAR_BIT - ((n - 1) % CHAR_BIT + 1);
				// copy full bytes
				if (n > CHAR_BIT)
				{
					if (bitOffset)
					{
						do
						{
							*iter++ |= *valuePtr >> bitOffset;
							*iter = *valuePtr++ << CHAR_BIT - bitOffset;
						}
						while ((n -= CHAR_BIT) > CHAR_BIT);
					}
					else
					{
						do
						{
							*iter++ = *valuePtr++;
						}
						while ((n -= CHAR_BIT) > CHAR_BIT);
					}
				}
				// copy remaining bits
				if (n > CHAR_BIT - bitOffset)
				{
					*iter++ |= *valuePtr >> bitOffset;
					*iter = (*valuePtr & UCHAR_MAX << CHAR_BIT - n) << CHAR_BIT - bitOffset;
				}
				else
				{
					*iter |= *valuePtr >> CHAR_BIT - n << CHAR_BIT - n - bitOffset;
				}
			}
		}

		// channel construct
		Image::Channel::Channel(Type type, unsigned depth) :
			type(type), depth(depth) {}

		// image comparison
		bool operator ==(const Image &a, const Image &b)
		{
			return
				All(a.size == b.size) &&
				a.channels == b.channels &&
				a.alignment == b.alignment &&
				a.data == b.data;
		}
		bool operator !=(const Image &a, const Image &b)
		{
			return
				Any(a.size != b.size) ||
				a.channels != b.channels ||
				a.alignment != b.alignment ||
				a.data != b.data;
		}

		// channel comparison
		bool operator ==(const Image::Channel &a, const Image::Channel &b)
		{
			return a.type == b.type && a.depth == b.depth;
		}
		bool operator !=(const Image::Channel &a, const Image::Channel &b)
		{
			return a.type != b.type || a.depth != b.depth;
		}

		// channel type
		bool HasColor(const Image::Channels &channels)
		{
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->type | Image::Channel::gray) return true;
			return false;
		}
		bool HasAlpha(const Image::Channels &channels)
		{
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->type | Image::Channel::alpha) return true;
			return false;
		}

		// channel depth
		unsigned GetDepth(const Image::Channels &channels)
		{
			unsigned depth = 0;
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				depth += channel->depth;
			return depth;
		}
		unsigned GetComponentDepth(const Image::Channels &channels)
		{
			unsigned depth = 0;
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->type) depth = std::max(channel->depth, depth);
			return depth;
		}
		unsigned GetColorDepth(const Image::Channels &channels)
		{
			unsigned depth = 0;
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->type | Image::Channel::gray) depth = std::max(channel->depth, depth);
			return depth;
		}
		unsigned GetAlphaDepth(const Image::Channels &channels)
		{
			unsigned depth = 0;
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->type | Image::Channel::alpha) depth = std::max(channel->depth, depth);
			return depth;
		}

		// transformation
		Image Flip(const Image &img, const math::Vector<2, bool> &flip)
		{
			if (All(!flip)) return img;
			// create destination image
			Image destImg =
			{
				img.size,
				img.channels,
				img.alignment
			};
			destImg.data.resize(img.data.size());
			// initialize iterators
			Image::Data::const_iterator srcIter(img.data.begin());
			Image::Data::iterator destIter(destImg.data.begin());
			unsigned
				depth = GetDepth(img.channels),
				pitch = GetBitPitch(img.size.x, depth, img.alignment),
				srcPadding = GetPadding(img.size.x, depth, img.alignment);
			unsigned srcBit = 0;
			int destBit = 0;
			// account for flipping
			if (flip.x)
			{
				destBit = (img.size.x - 1) * depth;
				destIter += destBit / CHAR_BIT;
				destBit %= CHAR_BIT;
			}
			if (flip.y)
			{
				destBit = (img.size.y - 1) * pitch;
				destIter += destBit / CHAR_BIT;
				destBit %= CHAR_BIT;
			}
			// copy pixels
			for (unsigned i = 0; i < img.size.y; ++i)
			{
				Image::Data::iterator destRowIter(destIter);
				int destRowBit = destBit;
				for (unsigned j = 0; j < img.size.x; ++j)
				{
					Image::Data::iterator destColIter(destIter);
					unsigned destColBit = destBit, n = depth;
					if (n <= CHAR_BIT - destBit)
					{
						if (n <= CHAR_BIT - srcBit)
						{
							*destIter |= *srcIter << srcBit >> CHAR_BIT - n << CHAR_BIT - (destBit + n);
							srcBit += n;
						}
						else
						{
							*destIter |= *srcIter++ << srcBit >> destBit;
							srcBit = n - (CHAR_BIT - srcBit);
							*destIter |= *srcIter >> srcBit << CHAR_BIT - (destBit + n);
						}
					}
					else
					{
						// align destination
						if (destBit)
						{
							if (srcBit < destBit)
							{
								*destIter |= *srcIter << srcBit >> destBit;
								srcBit += CHAR_BIT - destBit;
							}
							else
							{
								srcBit -= destBit;
								*destIter |= *srcIter++ << srcBit;
								*destIter |= *srcIter >> CHAR_BIT - srcBit;
							}
							n -= CHAR_BIT - destBit;
							destBit = 0;
						}
						// copy full bytes
						if (n > CHAR_BIT)
						{
							if (srcBit)
							{
								do
								{
									*destIter = *srcIter++ << srcBit;
									*destIter++ |= *srcIter >> CHAR_BIT - srcBit;
								}
								while ((n -= CHAR_BIT) > CHAR_BIT);
							}
							else
							{
								do
								{
									*destIter++ = *srcIter++;
								}
								while ((n -= CHAR_BIT) > CHAR_BIT);
							}
						}
						// copy remaining bits
						if (n > CHAR_BIT - srcBit)
						{
							*destIter = *srcIter++ << srcBit;
							*destIter |= *srcIter >> CHAR_BIT - srcBit & UCHAR_MAX << CHAR_BIT - n;
							srcBit = n - (CHAR_BIT - srcBit);
						}
						else
						{
							*destIter = *srcIter << srcBit & UCHAR_MAX << CHAR_BIT - n;
							srcBit += n;
							srcIter += srcBit / CHAR_BIT;
							srcBit %= CHAR_BIT;
						}
					}
					// increment destination position
					if (flip.x)
					{
						destBit = destColBit - depth;
						destIter = destColIter + destBit / CHAR_BIT;
						destBit = (destBit % CHAR_BIT + CHAR_BIT) % CHAR_BIT;
					}
					else
					{
						destBit += destColBit + depth;
						destIter = destColIter + destBit / CHAR_BIT;
						destBit %= CHAR_BIT;
					}
				}
				// increment source position
				if (srcPadding)
				{
					srcIter += srcPadding;
					srcBit = 0;
				}
				// increment destination position
				if (flip.y)
				{
					destBit = destRowBit - pitch;
					destIter = destRowIter + destBit / CHAR_BIT;
					destBit = (destBit % CHAR_BIT + CHAR_BIT) % CHAR_BIT;
				}
				else
				{
					destBit = destRowBit + pitch;
					destIter = destRowIter + destBit / CHAR_BIT;
					destBit %= CHAR_BIT;
				}
			}
			return destImg;
		}
		Image Scale(const Image &img, const math::Vector<2, unsigned> &size)
		{
			if (All(size == img.size)) return img;
			Image destImg =
			{
				size,
				img.channels,
				img.alignment
			};
			unsigned
				depth = GetDepth(img.channels),
				pitch = GetBitPitch(img.size.x, depth, img.alignment);
			destImg.data.resize(GetDataSize(size, depth, img.alignment));
			Image::Data::const_iterator srcIter(img.data.begin());
			Image::Data::iterator destIter(destImg.data.begin());
			unsigned srcBit = 0, destBit = 0;
			// initialize intermediate component storage
			typedef std::vector<float> Components;
			Components components(img.channels.size()), accumComponents;
			accumComponents.reserve(img.channels.size());
			// initialize box filter metrics
			math::Vector<2>
				scale(math::Vector<2>(img.size) / size),
				baseBoxOffset(Select(scale < 1, (scale - 1) / 2, math::Vector<2>())),
				boxOffset(baseBoxOffset),
				boxSize(Select(scale < 1, math::ScaleVector<2>(), scale));
			math::Vector<2, unsigned> pixelBoxPos;
			// process pixels
			for (unsigned i = 0; i < size.y; ++i)
			{
				Image::Data::const_iterator srcRowIter(srcIter);
				unsigned srcRowBit = srcBit;
				for (unsigned i = 0; i < size.x; ++i)
				{
					// initialize box filter metrics
					math::Vector<2, unsigned> pixelBoxSize(Ceil(boxOffset + boxSize));
					math::Aabb<2> boxCoverage(1 - boxOffset,
						1 - (pixelBoxSize - (boxOffset + boxSize)));
					if (pixelBoxPos.x + pixelBoxSize.x > img.size.x)
					{
						pixelBoxSize.x = img.size.x - pixelBoxPos.x;
						boxCoverage.max.x = 1;
					}
					if (pixelBoxPos.y + pixelBoxSize.y > img.size.y)
					{
						pixelBoxSize.y = img.size.y - pixelBoxPos.y;
						boxCoverage.max.y = 1;
					}
					// count alpha channels
					unsigned alphaChannels = 0;
					for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
						if (channel->type & Image::Channel::alpha) ++alphaChannels;
					// load and combine pixels
					accumComponents.resize(img.channels.size());
					float totalCoverage = 0, totalColorCoverage = 0;
					unsigned pixelOffset = 0;
					for (unsigned i = 0; i < pixelBoxSize.y; ++i)
					{
						// calculate row coverage
						float coverage = i == 0 ? boxCoverage.min.y :
							i == pixelBoxSize.y - 1 ? boxCoverage.max.y : 1,
							rowCoverage = coverage;
						unsigned pixelRowOffset = pixelOffset;
						for (unsigned i = 0; i < pixelBoxSize.x; ++i)
						{
							// calculate pixel coverage
							float coverage = rowCoverage * (i == 0 ? boxCoverage.min.x :
								i == pixelBoxSize.x - 1 ? boxCoverage.max.x : 1);
							totalCoverage += coverage;
							// load pixel components
							unsigned channelOffset = 0;
							Components::iterator component(components.begin());
							for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
							{
								*component++ = GetComponent(srcIter, srcBit + pixelOffset + channelOffset, channel->depth);
								channelOffset += channel->depth;
							}
							// calculate color coverage from alpha
							float colorCoverage = 1.f;
							if (alphaChannels)
							{
								float alpha = 0.f;
								Components::const_iterator component(components.begin());
								for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel, ++component)
									if (channel->type & Image::Channel::alpha)
										alpha += *component;
								alpha /= alphaChannels;
								colorCoverage = coverage * alpha;
							}
							totalColorCoverage += colorCoverage;
							// accumulate pixel components
							Components::iterator outComponent(accumComponents.begin());
							Components::const_iterator inComponent(components.begin());
							for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
							{
								*outComponent++ += *inComponent++ * (
									channel->type & Image::Channel::alpha ?
									coverage : colorCoverage);
								channelOffset += channel->depth;
							}
							pixelOffset += depth;
						}
						pixelOffset = pixelRowOffset + pitch;
					}
					// store result
					unsigned channelOffset = 0;
					Components::iterator component(accumComponents.begin());
					for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel, ++component)
					{
						if (*component)
							SetComponent(destIter, destBit + channelOffset, channel->depth,
								*component / (channel->type & Image::Channel::alpha ?
								totalCoverage : totalColorCoverage));
						channelOffset += channel->depth;
					}
					accumComponents.clear();
					// increment source position
					boxOffset.x += scale.x;
					unsigned delta = static_cast<unsigned>(boxOffset.x);
					boxOffset.x = std::fmod(boxOffset.x, 1);
					srcBit += depth * delta;
					srcIter += srcBit / CHAR_BIT;
					srcBit %= CHAR_BIT;
					pixelBoxPos.x += delta;
					// increment destination position
					destBit += depth;
					destIter += destBit / CHAR_BIT;
					destBit %= CHAR_BIT;
				}
				// update source position
				srcIter = srcRowIter;
				srcBit = srcRowBit;
				boxOffset = math::Vector<2>(baseBoxOffset.x, boxOffset.y + scale.y);
				unsigned delta = static_cast<unsigned>(boxOffset.y);
				boxOffset.y = std::fmod(boxOffset.y, 1);
				srcBit += pitch * delta;
				srcIter += srcBit / CHAR_BIT;
				srcBit %= CHAR_BIT;
				pixelBoxPos = math::Vector<2, unsigned>(0, pixelBoxPos.y + delta);
			}
			return destImg;
		}

		// format conversion
		Image Convert(const Image &img, const Image::Channels &channels)
		{
			return Convert(img, channels, img.alignment);
		}
		Image Convert(const Image &img, const Image::Channels &channels, unsigned alignment)
		{
			if (channels == img.channels) return img;
			Image destImg =
			{
				img.size,
				channels,
				alignment
			};
			// check for simple depth conversions
			if (channels.size() == img.channels.size() && !channels.empty())
			{
				// check if source and destination channel types match
				Image::Channels::const_iterator  srcChannel(img.channels.begin());
				for (Image::Channels::const_iterator destChannel(channels.begin()); destChannel != channels.end(); ++destChannel, ++srcChannel)
					if (destChannel->type != srcChannel->type) goto NotSimple;
				// check if channel depth is uniform
				if (channels.size() > 1)
				{
					for (Image::Channels::const_iterator channel(img.channels.begin()); ++channel != img.channels.end();)
						if (channel->depth != img.channels[0].depth) goto NotSimple;
					for (Image::Channels::const_iterator channel(channels.begin()); ++channel != channels.end();)
						if (channel->depth != channels[0].depth) goto NotSimple;
				}
				// check for matching specializations
				if (img.channels[0].depth == 1 && channels[0].depth == CHAR_BIT)
				{
					destImg.data = ConvertBitToByte(img, alignment);
					return destImg;
				}
			}
			NotSimple:
			// build conversion steps
			Steps steps;
			unsigned destOffset = 0;
			for (Image::Channels::const_iterator destChannel(channels.begin()); destChannel != channels.end(); ++destChannel)
			{
				if (!destChannel->depth) continue;
				Step step;
				unsigned srcOffset = 0;
				for (Image::Channels::const_iterator srcChannel(img.channels.begin()); srcChannel != img.channels.end(); ++srcChannel)
				{
					if (srcChannel->type & destChannel->type && srcChannel->depth)
						step.sources.push_back(Step::Component(srcOffset, srcChannel->depth));
					srcOffset += srcChannel->depth;
				}
				step.targets.push_back(Step::Component(destOffset, destChannel->depth));
				step.alpha = destChannel->type & Image::Channel::alpha;
				steps.push_back(step);
				destOffset += destChannel->depth;
			}
			// combine similar conversion steps
			for (Steps::iterator step(steps.begin()); step != steps.end(); ++step)
				for (Steps::iterator step2(step + 1); step2 != steps.end();)
					if (step2->sources == step->sources && (!step->sources.empty() || step2->alpha == step->alpha))
					{
						step->targets.insert(step->targets.end(), step2->targets.begin(), step2->targets.end());
						step2 = steps.erase(step2);
					}
					else ++step2;
			// check for pure byte conversions
			for (Image::Channels::const_iterator channel(img.channels.begin()); channel != img.channels.end(); ++channel)
				if (channel->depth != CHAR_BIT) goto NotByte;
			for (Image::Channels::const_iterator channel(channels.begin()); channel != channels.end(); ++channel)
				if (channel->depth != CHAR_BIT) goto NotByte;
			for (Steps::const_iterator step(steps.begin()); step != steps.end(); ++step)
				if (step->sources.size() != 1 || step->targets.size() != 1) goto NotSwizzle;
			destImg.data = SwizzleBytes(img, channels, steps, alignment);
			return destImg;
			NotSwizzle:
			destImg.data = ConvertBytes(img, channels, steps, alignment);
			return destImg;
			NotByte:
			// perform conversion
			unsigned
				srcDepth  = GetDepth(img.channels),
				destDepth = GetDepth(channels);
			destImg.data.resize(GetDataSize(img.size, destDepth, alignment));
			Image::Data::const_iterator srcIter(img.data.begin());
			Image::Data::iterator destIter(destImg.data.begin());
			unsigned srcBit = 0, destBit = 0,
				srcPadding  = GetPadding(img.size.x, srcDepth,  img.alignment),
				destPadding = GetPadding(img.size.x, destDepth, alignment);
			for (unsigned i = 0; i < img.size.y; ++i)
			{
				for (unsigned i = 0; i < img.size.x; ++i)
				{
					for (Steps::const_iterator step(steps.begin()); step != steps.end(); ++step)
					{
						// load average value
						float value = step->alpha;
						if (!step->sources.empty())
						{
							value = 0;
							for (Step::Components::const_iterator source(step->sources.begin()); source != step->sources.end(); ++source)
								value += GetComponent(srcIter, srcBit + source->offset, source->depth);
							value /= step->sources.size();
						}
						// store result
						if (value)
							for (Step::Components::const_iterator target(step->targets.begin()); target != step->targets.end(); ++target)
								SetComponent(destIter, destBit + target->offset, target->depth, value);
					}
					// increment source position
					srcBit += srcDepth;
					srcIter += srcBit / CHAR_BIT;
					srcBit %= CHAR_BIT;
					// increment destination position
					destBit += destDepth;
					destIter += destBit / CHAR_BIT;
					destBit %= CHAR_BIT;
				}
				if (srcPadding)  { srcIter  += srcPadding;  srcBit  = 0; }
				if (destPadding) { destIter += destPadding; destBit = 0; }
			}
			return destImg;
		}

		// alignment
		bool IsAligned(const Image &img, unsigned alignment)
		{
			if (img.alignment == alignment) return true;
			unsigned pitch = img.size.x * GetDepth(img.channels);
			if (!alignment || !img.alignment)
				return
					!(pitch % CHAR_BIT) &&
					!(pitch / CHAR_BIT % std::min(alignment, img.alignment));
			pitch = (pitch + CHAR_BIT - 1) / CHAR_BIT;
			return
				alignment - pitch % alignment ==
				img.alignment - pitch % img.alignment;
		}
		Image Align(const Image &img, unsigned alignment)
		{
			if (IsAligned(img, alignment)) return img;
			Image destImg =
			{
				img.size,
				img.channels,
				alignment
			};
			unsigned depth = GetDepth(img.channels);
			if (alignment) // packed to aligned
			{
				unsigned
					srcPitch = img.size.x * depth / CHAR_BIT,
					srcBit = 0,
					srcBitPitch = img.size.x * depth % CHAR_BIT,
					destPitch = GetBytePitch(img.size.x, depth, alignment);
				destImg.data.resize(destPitch * img.size.y);
				Image::Data::const_iterator srcIter(img.data.begin());
				for (Image::Data::iterator destIter(destImg.data.begin()); destIter != destImg.data.end();)
				{
					util::BitCopy(&*srcIter, srcBit, &*destIter, 0, img.size.x * depth);
					srcIter += srcPitch;
					if ((srcBit += srcBitPitch) >= CHAR_BIT)
					{
						++srcIter;
						srcBit -= CHAR_BIT;
					}
					destIter += destPitch;
				}
			}
			else if (img.alignment) // aligned to packed
			{
				unsigned
					srcPitch = GetBytePitch(img.size.x, depth, img.alignment),
					destPitch = img.size.x * depth / CHAR_BIT,
					destBit = 0,
					destBitPitch = img.size.x * depth % CHAR_BIT;
				destImg.data.resize((Content(img.size) * depth + CHAR_BIT - 1) / CHAR_BIT);
				Image::Data::iterator destIter(destImg.data.begin());
				for (Image::Data::const_iterator srcIter(img.data.begin()); srcIter != img.data.end();)
				{
					util::BitCopy(&*srcIter, 0, &*destIter, destBit, img.size.x * depth);
					srcIter += srcPitch;
					destIter += destPitch;
					if ((destBit += destBitPitch) >= CHAR_BIT)
					{
						++destIter;
						destBit -= CHAR_BIT;
					}
				}
			}
			else // aligned to aligned
			{
				unsigned
					width = (img.size.x * depth + CHAR_BIT - 1) / CHAR_BIT,
					srcPitch  = GetBytePitch(img.size.x, depth, img.alignment),
					destPitch = GetBytePitch(img.size.x, depth, alignment);
				destImg.data.resize(destPitch * img.size.y);
				Image::Data::const_iterator srcIter(img.data.begin());
				for (Image::Data::iterator destIter(destImg.data.begin()); destIter != destImg.data.end();)
				{
					std::copy(srcIter, srcIter + width, destIter);
					srcIter += srcPitch;
					destIter += destPitch;
				}
			}
			return destImg;
		}

		void PostLoadImage(Image &img)
		{
			assert(bool(img.size.x) == bool(img.size.y));
			// HACK: sanity check in case alignment is left uninitialized
			assert(img.alignment <= 128);
		}

		REGISTER_TYPE(Image, "image", PostLoadImage)
	}
}
