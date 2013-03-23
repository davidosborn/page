/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include <algorithm> // copy
#include <cmath> // fmod
#include <iterator> // back_inserter

#include "AnimationController.hpp"

namespace page { namespace phys
{
	// construct
	AnimationController::AnimationController(const res::Animation &anim, float timeScale) :
		Controller(preCollisionLayer),
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
