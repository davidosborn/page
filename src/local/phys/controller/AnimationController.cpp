#include <algorithm> // copy
#include <cmath> // fmod
#include <iterator> // back_inserter

#include "AnimationController.hpp"

namespace page { namespace phys
{
	// construct
	AnimationController::AnimationController(const res::Animation &anim, float timeScale) :
		Controller(AnimationLayer::preCollision),
		time(0), timeScale(timeScale), duration(anim.duration),
		position(anim.position),
		orientation(anim.orientation),
		normal(anim.normal),
		scale(anim.scale),
		texCoord(anim.texCoord),
		ambient(anim.ambient),
		diffuse(anim.diffuse),
		specular(anim.specular),
		attenuation(anim.attenuation),
		cutoff(anim.cutoff),
		depth(anim.depth),
		exposure(anim.exposure),
		falloff(anim.falloff),
		fov(anim.fov),
		opacity(anim.opacity),
		range(anim.range),
		size(anim.size),
		volume(anim.volume)
	{
		bones.reserve(anim.bones.size());
		std::copy(anim.bones.begin(), anim.bones.end(), std::back_inserter(bones));
		vertices.reserve(anim.vertices.size());
		std::copy(anim.vertices.begin(), anim.vertices.end(), std::back_inserter(vertices));
	}

	// modifiers
	void AnimationController::SetPlayPosition(float playPosition)
	{
		time = std::fmod(std::fmod(playPosition, duration) + duration, duration);
	}

	// update/generate frame
	void AnimationController::DoUpdate(float deltaTime)
	{
		// FIXME: don't assume looping
		SetPlayPosition(time + deltaTime * timeScale);
	}
	Frame AnimationController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		if (position)    frame.position    = position.Get(time);
		if (orientation) frame.orientation = orientation.Get(time);
		if (normal)      frame.normal      = normal.Get(time);
		if (scale)       frame.scale       = scale.Get(time);
		if (texCoord)    frame.texCoord    = texCoord.Get(time);
		if (ambient)     frame.ambient     = ambient.Get(time);
		if (diffuse)     frame.diffuse     = diffuse.Get(time);
		if (specular)    frame.specular    = specular.Get(time);
		if (attenuation) frame.attenuation = attenuation.Get(time);
		if (cutoff)      frame.cutoff      = cutoff.Get(time);
		if (depth)       frame.depth       = depth.Get(time);
		if (exposure)    frame.exposure    = exposure.Get(time);
		if (falloff)     frame.falloff     = falloff.Get(time);
		if (fov)         frame.fov         = fov.Get(time);
		if (opacity)     frame.opacity     = opacity.Get(time);
		if (range)       frame.range       = range.Get(time);
		if (size)        frame.size        = size.Get(time);
		if (volume)      frame.volume      = volume.Get(time);
		for (Bones::const_iterator bone(bones.begin()); bone != bones.end(); ++bone)
		{
			Frame::Bone &frameBone(frame.bones[bone->name]);
			if (bone->position)    frameBone.position    = bone->position.Get(time);
			if (bone->orientation) frameBone.orientation = bone->orientation.Get(time);
			if (bone->scale)       frameBone.scale       = bone->scale.Get(time);
		}
		for (Vertices::const_iterator vertex(vertices.begin()); vertex != vertices.end(); ++vertex)
		{
			Frame::Vertex &frameVertex(frame.vertices[vertex->index]);
			if (vertex->position) frameVertex.position = vertex->position.Get(time);
			if (vertex->normal)   frameVertex.normal   = vertex->normal.Get(time);
			if (vertex->texCoord) frameVertex.texCoord = vertex->texCoord.Get(time);
		}
		return frame;
	}

	// bone
	// construct
	AnimationController::Bone::Bone(res::Animation::Bones::const_iterator::reference pair) :
		name(pair.first),
		position(pair.second.position),
		orientation(pair.second.orientation),
		scale(pair.second.scale) {}

	// vertex
	// construct
	AnimationController::Vertex::Vertex(res::Animation::Vertices::const_iterator::reference pair) :
		index(pair.first),
		position(pair.second.position),
		normal(pair.second.normal),
		texCoord(pair.second.texCoord) {}
}}
