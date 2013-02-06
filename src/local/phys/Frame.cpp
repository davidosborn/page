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

#include "../math/interp.hpp" // LinearInterp
#include "Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace
		{
			// blending
			// NOTE: nested types do not participate in deduction (bug #31534)
			template <typename T> inline void Blend(util::Optional<T> &dest, const util::Optional<T> &src, float alpha)
			{
				if (src) dest = math::LinearInterp(dest ? *dest : T(), *src, alpha);
			}
			template <typename T> inline void Blend(util::Optional<Frame::Range<T>> &dest, const util::Optional<Frame::Range<T>> &src, float alpha)
			{
				if (src) dest = Frame::Range<T>(
					math::LinearInterp(dest ? dest->min : T(), src->min, alpha),
					math::LinearInterp(dest ? dest->max : T(), src->max, alpha));
			}
			template <typename T> inline void Blend(util::Optional<T> &dest, const util::Optional<T> &src, float alpha, const util::Optional<T> &base)
			{
				if (src) dest = math::LinearInterp(dest ? *dest : base ? *base : T(), *src, alpha);
			}
			template <typename T> inline void Blend(util::Optional<Frame::Range<T>> &dest, const util::Optional<Frame::Range<T>> &src, float alpha, const util::Optional<Frame::Range<T>> &base)
			{
				if (src) dest = Frame::Range<T>(
					math::LinearInterp(dest ? dest->min : base ? base->min : T(), src->min, alpha),
					math::LinearInterp(dest ? dest->max : base ? base->max : T(), src->max, alpha));
			}

			// masking
			// NOTE: nested types do not participate in deduction (bug #31534)
			template <typename T> inline void Mask(util::Optional<T> &dest, const util::Optional<T> &mask)
			{
				if (!mask) dest.reset();
			}
		}

		// concatenation
		Frame operator +(const Frame &a, const Frame &b)
		{
			Frame r;
			r.ambient       = (b.ambient       ? b : a).ambient;
			r.ambientRange  = (b.ambientRange  ? b : a).ambientRange;
			r.aspect        = (b.aspect        ? b : a).aspect;
			r.attenuation   = (b.attenuation   ? b : a).attenuation;
			r.cutoff        = (b.cutoff        ? b : a).cutoff;
			r.depth         = (b.depth         ? b : a).depth;
			r.diffuse       = (b.diffuse       ? b : a).diffuse;
			r.diffuseRange  = (b.diffuseRange  ? b : a).diffuseRange;
			r.emissive      = (b.emissive      ? b : a).emissive;
			r.emissiveRange = (b.emissiveRange ? b : a).emissiveRange;
			r.exposure      = (b.exposure      ? b : a).exposure;
			r.falloff       = (b.falloff       ? b : a).falloff;
			r.fov           = (b.fov           ? b : a).fov;
			r.lifetimeRange = (b.lifetimeRange ? b : a).lifetimeRange;
			r.normal        = (b.normal        ? b : a).normal;
			r.opacity       = (b.opacity       ? b : a).opacity;
			r.opacityRange  = (b.opacityRange  ? b : a).opacityRange;
			r.orientation   = (b.orientation   ? b : a).orientation;
			r.position      = (b.position      ? b : a).position;
			r.range         = (b.range         ? b : a).range;
			r.scale         = (b.scale         ? b : a).scale;
			r.size          = (b.size          ? b : a).size;
			r.sizeRange     = (b.sizeRange     ? b : a).sizeRange;
			r.specular      = (b.specular      ? b : a).specular;
			r.specularRange = (b.specularRange ? b : a).specularRange;
			r.speedRange    = (b.speedRange    ? b : a).speedRange;
			r.texCoord      = (b.texCoord      ? b : a).texCoord;
			r.volume        = (b.volume        ? b : a).volume;
			// bones
			r.bones = a.bones;
			for (Frame::Bones::const_iterator bIter(b.bones.begin()); bIter != b.bones.end(); ++bIter)
			{
				const Frame::Bone &bBone(bIter->second);
				Frame::Bone &rBone(r.bones[bIter->first]);
				if (bBone.position)    rBone.position    = bBone.position;
				if (bBone.orientation) rBone.orientation = bBone.orientation;
				if (bBone.scale)       rBone.scale       = bBone.scale;
			}
			// vertices
			r.vertices = a.vertices;
			for (Frame::Vertices::const_iterator bIter(b.vertices.begin()); bIter != b.vertices.end(); ++bIter)
			{
				const Frame::Vertex &bVertex(bIter->second);
				Frame::Vertex &rVertex(r.vertices[bIter->first]);
				if (bVertex.position) rVertex.position = bVertex.position;
				if (bVertex.normal)   rVertex.normal   = bVertex.normal;
				if (bVertex.texCoord) rVertex.texCoord = bVertex.texCoord;
			}
			return r;
		}
		Frame &operator +=(Frame &a, const Frame &b)
		{
			if (b.ambient)       a.ambient       = b.ambient;
			if (b.ambientRange)  a.ambientRange  = b.ambientRange;
			if (b.aspect)        a.aspect        = b.aspect;
			if (b.attenuation)   a.attenuation   = b.attenuation;
			if (b.cutoff)        a.cutoff        = b.cutoff;
			if (b.depth)         a.depth         = b.depth;
			if (b.diffuse)       a.diffuse       = b.diffuse;
			if (b.diffuseRange)  a.diffuseRange  = b.diffuseRange;
			if (b.emissive)      a.emissive      = b.emissive;
			if (b.emissiveRange) a.emissiveRange = b.emissiveRange;
			if (b.exposure)      a.exposure      = b.exposure;
			if (b.falloff)       a.falloff       = b.falloff;
			if (b.fov)           a.fov           = b.fov;
			if (b.lifetimeRange) a.lifetimeRange = b.lifetimeRange;
			if (b.normal)        a.normal        = b.normal;
			if (b.opacity)       a.opacity       = b.opacity;
			if (b.opacityRange)  a.opacityRange  = b.opacityRange;
			if (b.orientation)   a.orientation   = b.orientation;
			if (b.position)      a.position      = b.position;
			if (b.range)         a.range         = b.range;
			if (b.scale)         a.scale         = b.scale;
			if (b.size)          a.size          = b.size;
			if (b.sizeRange)     a.sizeRange     = b.sizeRange;
			if (b.specular)      a.specular      = b.specular;
			if (b.specularRange) a.specularRange = b.specularRange;
			if (b.speedRange)    a.speedRange    = b.speedRange;
			if (b.texCoord)      a.texCoord      = b.texCoord;
			if (b.volume)        a.volume        = b.volume;
			// bones
			for (Frame::Bones::const_iterator bIter(b.bones.begin()); bIter != b.bones.end(); ++bIter)
			{
				Frame::Bones::iterator aIter(a.bones.find(bIter->first));
				if (aIter != a.bones.end())
				{
					const Frame::Bone &bBone(bIter->second);
					Frame::Bone &aBone(aIter->second);
					if (bBone.position)    aBone.position    = bBone.position;
					if (bBone.orientation) aBone.orientation = bBone.orientation;
					if (bBone.scale)       aBone.scale       = bBone.scale;
				}
				else a.bones.insert(*bIter);
			}
			// vertices
			for (Frame::Vertices::const_iterator bIter(b.vertices.begin()); bIter != b.vertices.end(); ++bIter)
			{
				Frame::Vertices::iterator aIter(a.vertices.find(bIter->first));
				if (aIter != a.vertices.end())
				{
					const Frame::Vertex &bVertex(bIter->second);
					Frame::Vertex &aVertex(aIter->second);
					if (bVertex.position) aVertex.position = bVertex.position;
					if (bVertex.normal)   aVertex.normal   = bVertex.normal;
					if (bVertex.texCoord) aVertex.texCoord = bVertex.texCoord;
				}
				else a.vertices.insert(*bIter);
			}
			return a;
		}

		// blending
		void Blend(Frame &dest, const Frame &src, float alpha)
		{
			Blend(dest.ambient,       src.ambient,       alpha);
			Blend(dest.ambientRange,  src.ambientRange,  alpha);
			Blend(dest.aspect,        src.aspect,        alpha);
			Blend(dest.attenuation,   src.attenuation,   alpha);
			Blend(dest.cutoff,        src.cutoff,        alpha);
			Blend(dest.depth,         src.depth,         alpha);
			Blend(dest.diffuse,       src.diffuse,       alpha);
			Blend(dest.diffuseRange,  src.diffuseRange,  alpha);
			Blend(dest.emissive,      src.emissive,      alpha);
			Blend(dest.emissiveRange, src.emissiveRange, alpha);
			Blend(dest.exposure,      src.exposure,      alpha);
			Blend(dest.falloff,       src.falloff,       alpha);
			Blend(dest.fov,           src.fov,           alpha);
			Blend(dest.lifetimeRange, src.lifetimeRange, alpha);
			Blend(dest.normal,        src.normal,        alpha);
			Blend(dest.opacity,       src.opacity,       alpha);
			Blend(dest.opacityRange,  src.opacityRange,  alpha);
			Blend(dest.orientation,   src.orientation,   alpha);
			Blend(dest.position,      src.position,      alpha);
			Blend(dest.range,         src.range,         alpha);
			Blend(dest.scale,         src.scale,         alpha);
			Blend(dest.size,          src.size,          alpha);
			Blend(dest.sizeRange,     src.sizeRange,     alpha);
			Blend(dest.specular,      src.specular,      alpha);
			Blend(dest.specularRange, src.specularRange, alpha);
			Blend(dest.speedRange,    src.speedRange,    alpha);
			Blend(dest.texCoord,      src.texCoord,      alpha);
			Blend(dest.volume,        src.volume,        alpha);
			// bones
			for (Frame::Bones::const_iterator srcIter(src.bones.begin()); srcIter != src.bones.end(); ++srcIter)
			{
				const Frame::Bone &srcBone(srcIter->second);
				Frame::Bone &destBone(dest.bones[srcIter->first]);
				Blend(destBone.position,    srcBone.position,    alpha);
				Blend(destBone.orientation, srcBone.orientation, alpha);
				Blend(destBone.scale,       srcBone.scale,       alpha);
			}
			// vertices
			for (Frame::Vertices::const_iterator srcIter(src.vertices.begin()); srcIter != src.vertices.end(); ++srcIter)
			{
				const Frame::Vertex &srcVertex(srcIter->second);
				Frame::Vertex &destVertex(dest.vertices[srcIter->first]);
				Blend(destVertex.position, srcVertex.position, alpha);
				Blend(destVertex.normal,   srcVertex.normal,   alpha);
				Blend(destVertex.texCoord, srcVertex.texCoord, alpha);
			}
		}
		void Blend(Frame &dest, const Frame &src, float alpha, const Frame &base)
		{
			Blend(dest.ambient,       src.ambient,       alpha, base.ambient);
			Blend(dest.ambientRange,  src.ambientRange,  alpha, base.ambientRange);
			Blend(dest.aspect,        src.aspect,        alpha, base.aspect);
			Blend(dest.attenuation,   src.attenuation,   alpha, base.attenuation);
			Blend(dest.cutoff,        src.cutoff,        alpha, base.cutoff);
			Blend(dest.depth,         src.depth,         alpha, base.depth);
			Blend(dest.diffuse,       src.diffuse,       alpha, base.diffuse);
			Blend(dest.diffuseRange,  src.diffuseRange,  alpha, base.diffuseRange);
			Blend(dest.emissive,      src.emissive,      alpha, base.emissive);
			Blend(dest.emissiveRange, src.emissiveRange, alpha, base.emissiveRange);
			Blend(dest.exposure,      src.exposure,      alpha, base.exposure);
			Blend(dest.falloff,       src.falloff,       alpha, base.falloff);
			Blend(dest.fov,           src.fov,           alpha, base.fov);
			Blend(dest.lifetimeRange, src.lifetimeRange, alpha, base.lifetimeRange);
			Blend(dest.normal,        src.normal,        alpha, base.normal);
			Blend(dest.opacity,       src.opacity,       alpha, base.opacity);
			Blend(dest.opacityRange,  src.opacityRange,  alpha, base.opacityRange);
			Blend(dest.orientation,   src.orientation,   alpha, base.orientation);
			Blend(dest.position,      src.position,      alpha, base.position);
			Blend(dest.range,         src.range,         alpha, base.range);
			Blend(dest.scale,         src.scale,         alpha, base.scale);
			Blend(dest.size,          src.size,          alpha, base.size);
			Blend(dest.sizeRange,     src.sizeRange,     alpha, base.sizeRange);
			Blend(dest.specular,      src.specular,      alpha, base.specular);
			Blend(dest.specularRange, src.specularRange, alpha, base.specularRange);
			Blend(dest.speedRange,    src.speedRange,    alpha, base.speedRange);
			Blend(dest.texCoord,      src.texCoord,      alpha, base.texCoord);
			Blend(dest.volume,        src.volume,        alpha, base.volume);
			// bones
			for (Frame::Bones::const_iterator srcIter(src.bones.begin()); srcIter != src.bones.end(); ++srcIter)
			{
				const Frame::Bone &srcBone(srcIter->second);
				Frame::Bones::const_iterator baseIter(base.bones.find(srcIter->first));
				const Frame::Bone &baseBone(baseIter != base.bones.end() ? baseIter->second : Frame::Bone());
				Frame::Bone &destBone(dest.bones[srcIter->first]);
				Blend(destBone.position,    srcBone.position,    alpha, baseBone.position);
				Blend(destBone.orientation, srcBone.orientation, alpha, baseBone.orientation);
				Blend(destBone.scale,       srcBone.scale,       alpha, baseBone.scale);
			}
			// vertices
			for (Frame::Vertices::const_iterator srcIter(src.vertices.begin()); srcIter != src.vertices.end(); ++srcIter)
			{
				const Frame::Vertex &srcVertex(srcIter->second);
				Frame::Vertices::const_iterator baseIter(base.vertices.find(srcIter->first));
				const Frame::Vertex &baseVertex(baseIter != base.vertices.end() ? baseIter->second : Frame::Vertex());
				Frame::Vertex &destVertex(dest.vertices[srcIter->first]);
				Blend(destVertex.position, srcVertex.position, alpha, baseVertex.position);
				Blend(destVertex.normal,   srcVertex.normal,   alpha, baseVertex.normal);
				Blend(destVertex.texCoord, srcVertex.texCoord, alpha, baseVertex.texCoord);
			}
		}

		// masking
		void Mask(Frame &dest, const Frame &mask)
		{
			Mask(dest.ambient,       mask.ambient);
			Mask(dest.ambientRange,  mask.ambientRange);
			Mask(dest.aspect,        mask.aspect);
			Mask(dest.attenuation,   mask.attenuation);
			Mask(dest.cutoff,        mask.cutoff);
			Mask(dest.depth,         mask.depth);
			Mask(dest.diffuse,       mask.diffuse);
			Mask(dest.diffuseRange,  mask.diffuseRange);
			Mask(dest.emissive,      mask.emissive);
			Mask(dest.emissiveRange, mask.emissiveRange);
			Mask(dest.exposure,      mask.exposure);
			Mask(dest.falloff,       mask.falloff);
			Mask(dest.fov,           mask.fov);
			Mask(dest.lifetimeRange, mask.lifetimeRange);
			Mask(dest.normal,        mask.normal);
			Mask(dest.opacity,       mask.opacity);
			Mask(dest.opacityRange,  mask.opacityRange);
			Mask(dest.orientation,   mask.orientation);
			Mask(dest.position,      mask.position);
			Mask(dest.range,         mask.range);
			Mask(dest.scale,         mask.scale);
			Mask(dest.size,          mask.size);
			Mask(dest.sizeRange,     mask.sizeRange);
			Mask(dest.specular,      mask.specular);
			Mask(dest.specularRange, mask.specularRange);
			Mask(dest.speedRange,    mask.speedRange);
			Mask(dest.texCoord,      mask.texCoord);
			Mask(dest.volume,        mask.volume);
			// bones
			for (Frame::Bones::iterator destIter(dest.bones.begin()); destIter != dest.bones.end();)
			{
				Frame::Bones::const_iterator maskIter(mask.bones.find(destIter->first));
				if (maskIter == mask.bones.end())
				{
					dest.bones.erase(destIter++);
					continue;
				}
				Frame::Bone &destBone(destIter->second);
				const Frame::Bone &maskBone(maskIter->second);
				Mask(destBone.position,    maskBone.position);
				Mask(destBone.orientation, maskBone.orientation);
				Mask(destBone.scale,       maskBone.scale);
				if (!(destBone.position || destBone.orientation || destBone.scale))
					dest.bones.erase(destIter++);
				else ++destIter;
			}
			// vertices
			for (Frame::Vertices::iterator destIter(dest.vertices.begin()); destIter != dest.vertices.end();)
			{
				Frame::Vertices::const_iterator maskIter(mask.vertices.find(destIter->first));
				if (maskIter == mask.vertices.end())
				{
					dest.vertices.erase(destIter++);
					continue;
				}
				Frame::Vertex &destVertex(destIter->second);
				const Frame::Vertex &maskVertex(maskIter->second);
				Mask(destVertex.position, maskVertex.position);
				Mask(destVertex.normal,   maskVertex.normal);
				Mask(destVertex.texCoord, maskVertex.texCoord);
				if (!(destVertex.position || destVertex.normal || destVertex.texCoord))
					dest.vertices.erase(destIter++);
				else ++destIter;
			}
		}
	}
}
