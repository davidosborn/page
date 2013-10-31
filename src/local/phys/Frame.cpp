#include "../math/interp.hpp" // LinearInterp
#include "Frame.hpp"

namespace page { namespace phys
{
	namespace
	{
		/*---------------------+
		| Blend implementation |
		+---------------------*/

		template <typename T>
			inline void Blend(Frame::Channel<T> &dest, const Frame::Channel<T> &src, float alpha)
		{
			if (src)
				dest = dest ?
					math::LinearInterp(*dest, *src, alpha) :
					*src;
		}

		template <typename T>
			inline void Blend(Frame::Channel<Frame::Range<T>> &dest, const Frame::Channel<Frame::Range<T>> &src, float alpha)
		{
			if (src)
				dest = dest ?
					Frame::Range<T>(
						math::LinearInterp(dest->min, src->min, alpha),
						math::LinearInterp(dest->max, src->max, alpha)) :
					*src;
		}

		/*--------------------+
		| Mask implementation |
		+--------------------*/

		template <typename T>
			inline void Mask(Frame::Channel<T> &dest, const Frame::Channel<T> &mask)
		{
			if (!mask) dest.reset();
		}
	}

	/*-----------+
	| operations |
	+-----------*/

	Frame operator +(Frame a, const Frame &b)
	{
		a += b;
		return a;
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
		for (const auto &kv : b.bones)
		{
			auto iter(a.bones.find(kv.first));
			if (iter != a.bones.end())
			{
				auto &a(iter->second);
				const auto &b(kv.second);

				if (b.position)    a.position    = b.position;
				if (b.orientation) a.orientation = b.orientation;
				if (b.scale)       a.scale       = b.scale;
			}
			else a.bones.insert(kv);
		}

		// parts
		for (const auto &kv : b.parts)
		{
			auto iter(a.parts.find(kv.first));
			if (iter != a.parts.end())
			{
				auto &a(iter->second);
				const auto &b(kv.second);

				if (b.position)    a.position    = b.position;
				if (b.orientation) a.orientation = b.orientation;
				if (b.scale)       a.scale       = b.scale;
			}
			else a.parts.insert(kv);
		}

		// vertices
		for (const auto &kv : b.vertices)
		{
			auto iter(a.vertices.find(kv.first));
			iter (iter != a.vertices.end())
			{
				auto &a(iter->second);
				const auto &b(kv.second);

				if (b.position) a.position = b.position;
				if (b.normal)   a.normal   = b.normal;
				if (b.texCoord) a.texCoord = b.texCoord;
			}
			else a.vertices.insert(kv);
		}

		return a;
	}

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
		for (const auto &kv : src.bones())
		{
			auto iter(dest.bones.find(kv.first));
			if (iter != dest.bones.end())
			{
				auto &destBone(iter->second);
				const auto &srcBone(kv.second);

				Blend(destBone.position,    srcBone.position,    alpha);
				Blend(destBone.orientation, srcBone.orientation, alpha);
				Blend(destBone.scale,       srcBone.scale,       alpha);
			}
			else dest.bones.insert(kv);
		}

		// parts
		for (const auto &kv : src.parts())
		{
			auto iter(dest.parts.find(kv.first));
			if (iter != dest.parts.end())
			{
				auto &destPart(iter->second);
				const auto &srcPart(kv.second);

				Blend(destPart.position,    srcPart.position,    alpha);
				Blend(destPart.orientation, srcPart.orientation, alpha);
				Blend(destPart.scale,       srcPart.scale,       alpha);
			}
			else dest.parts.insert(kv);
		}

		// vertices
		for (const auto &kv : src.vertices())
		{
			auto iter(dest.vertices.find(kv.first));
			if (iter != dest.vertices.end())
			{
				auto &destVertex(iter->second);
				const auto &srcVertex(kv.second);

				Blend(destVertex.position, srcVertex.position, alpha);
				Blend(destVertex.normal,   srcVertex.normal,   alpha);
				Blend(destVertex.texCoord, srcVertex.texCoord, alpha);
			}
			else dest.vertices.insert(kv);
		}
	}

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
		for (auto destIter(dest.bones.begin()); destIter != dest.bones.end();)
		{
			auto maskIter(mask.bones.find(destIter->first));
			if (maskIter == mask.bones.end())
			{
				destIter = dest.bones.erase(destIter);
				continue;
			}

			auto &destBone(destIter->second);
			const auto &maskBone(maskIter->second);
			Mask(destBone.position,    maskBone.position);
			Mask(destBone.orientation, maskBone.orientation);
			Mask(destBone.scale,       maskBone.scale);

			++destIter;
		}

		// parts
		for (auto destIter(dest.parts.begin()); destIter != dest.parts.end();)
		{
			auto maskIter(mask.parts.find(destIter->first));
			if (maskIter == mask.parts.end())
			{
				destIter = dest.parts.erase(destIter);
				continue;
			}

			auto &destPart(destIter->second);
			const auto &maskPart(maskIter->second);
			Mask(destPart.position,    maskPart.position);
			Mask(destPart.orientation, maskPart.orientation);
			Mask(destPart.scale,       maskPart.scale);

			++destIter;
		}

		// vertices
		for (auto destIter(dest.vertices.begin()); destIter != dest.vertices.end();)
		{
			auto maskIter(mask.vertices.find(destIter->first));
			if (maskIter == mask.vertices.end())
			{
				destIter = dest.vertices.erase(destIter);
				continue;
			}

			auto &destVertex(destIter->second);
			const auto &maskVertex(maskIter->second);
			Mask(destVertex.position, maskVertex.position);
			Mask(destVertex.normal,   maskVertex.normal);
			Mask(destVertex.texCoord, maskVertex.texCoord);

			++destIter;
		}
	}
}}
