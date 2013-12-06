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
			TexCoord::GetFrame();
	}

	void Deformation::Vertex::SetFrame(const Frame &frame)
	{
		PositionNormal::SetFrame(frame);
		TexCoord::SetFrame(frame);
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
