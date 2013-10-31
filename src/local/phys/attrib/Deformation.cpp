#include "Deformation.hpp"

namespace page { namespace phys { namespace attrib
{
	// deformation vertex
	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Deformation::Vertex::GetFrame() const
	{
		return
			PositionNormal::GetFrame() +
			TexCoord::GetFrame();
	}

	void Deformation::Vertex::SetFrame(const Frame &frame)
	{
		PositionNormal::SetFrame(frame);
		TexCoord::SetFrame(frame);
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

	/*--------------------+
	| frame serialization |
	+--------------------*/

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

	void Deformation::SetFrame(const Frame &frame)
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
}}}
