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

#include "Deformation.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// deformation vertex
			// frame serialization
			Frame Deformation::Vertex::GetFrame() const
			{
				return
					PositionNormal::GetFrame() +
					TexCoord::GetFrame();
			}
			void Deformation::Vertex::Update(const Frame &frame)
			{
				PositionNormal::Update(frame);
				TexCoord::Update(frame);
			}

			// deformation
			// status
			bool Deformation::IsDeformed() const
			{
				return !vertices.empty();
			}

			// modifiers
			void Deformation::UpdateDeformation()
			{
				// FIXME: if any vertices have returned to their original
				// states, remove them from the list
			}
			void Deformation::ResetDeformation()
			{
				vertices.clear();
			}

			// frame serialization
			Frame Deformation::GetFrame() const
			{
				Frame frame;
				for (Vertices::const_iterator iter(vertices.begin()); iter != vertices.end(); ++iter)
				{
					const Vertex &vertex(iter->second);
					Frame::Vertex frameVertex =
					{
						vertex.GetPosition(),
						vertex.GetNormal(),
						vertex.GetTexCoord()
					};
					frame.vertices.insert(std::make_pair(iter->first, frameVertex));
				}
				return frame;
			}
			void Deformation::Update(const Frame &frame)
			{
				for (Frame::Vertices::const_iterator iter(frame.vertices.begin()); iter != frame.vertices.end(); ++iter)
				{
					const Frame::Vertex &frameVertex(iter->second);
					Vertex &vertex(vertices[iter->first]);
					if (frameVertex.position) vertex.SetPosition(*frameVertex.position);
					if (frameVertex.normal) vertex.SetNormal(*frameVertex.normal);
					if (frameVertex.texCoord) vertex.SetTexCoord(*frameVertex.texCoord);
				}
			}
		}
	}
}
