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

#include "Deformation.hpp"

namespace page { namespace phys { namespace attrib
{
////////// Deformation::Vertex /////////////////////////////////////////////////

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Deformation::Vertex::GetFrame() const
	{
		return
			PositionNormal::GetFrame() +
			TexCoord      ::GetFrame();
	}

	void Deformation::Vertex::SetFrame(const Frame &frame)
	{
		PositionNormal::SetFrame(frame);
		TexCoord      ::SetFrame(frame);
	}

////////// Deformation /////////////////////////////////////////////////////////

	/*----------+
	| observers |
	+----------*/

	bool Deformation::IsDeformed() const
	{
		return !vertices.empty();
	}

	/*----------+
	| modifiers |
	+----------*/

	void Deformation::UpdateDeformation()
	{
		// FIXME: if any vertices have returned to their original
		// states, remove them from the list
	}

	void Deformation::ResetDeformation()
	{
		vertices.clear();
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Deformation::GetFrame() const
	{
		Frame frame;
		for (const auto &kv : vertices)
		{
			const Vertex &vertex(kv.second);
			Frame::Vertex frameVertex =
			{
				vertex.GetPosition(),
				vertex.GetNormal(),
				vertex.GetTexCoord()
			};
			frame.vertices.insert(std::make_pair(kv.first, frameVertex));
		}
		return frame;
	}

	void Deformation::SetFrame(const Frame &frame)
	{
		for (const auto &kv : frame.vertices)
		{
			const Frame::Vertex &frameVertex(kv.second);
			Vertex &vertex(vertices[kv.first]);
			if (frameVertex.position) vertex.SetPosition(*frameVertex.position);
			if (frameVertex.normal)   vertex.SetNormal  (*frameVertex.normal);
			if (frameVertex.texCoord) vertex.SetTexCoord(*frameVertex.texCoord);
		}
	}
}}}
