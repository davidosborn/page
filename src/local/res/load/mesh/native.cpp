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

#include <cassert>
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/native/mesh.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/Mesh.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Mesh *LoadNativeMesh(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// check signature
			char sig[sizeof fmt::sig];
			if (stream->ReadSome(sig, sizeof sig) != sizeof sig ||
				std::memcmp(sig, fmt::sig, sizeof sig)) return 0;
			// read header
			fmt::Header header;
			stream->Read(&header, sizeof header);
			util::TransformEndian(&header, fmt::headerFormat, util::littleEndian);
			// read faces
			std::vector<fmt::Face> faces(header.faces);
			stream->Read(&*faces.begin(), sizeof *faces.begin() * faces.size());
			for (std::vector<fmt::Face>::iterator iter(faces.begin()); iter != faces.end(); ++iter)
				util::TransformEndian(&*iter, fmt::faceFormat, util::littleEndian);
			// read vertices
			std::vector<fmt::Vertex> vertices(header.vertices);
			stream->Read(&*vertices.begin(), sizeof *vertices.begin() * vertices.size());
			for (std::vector<fmt::Vertex>::iterator iter(vertices.begin()); iter != vertices.end(); ++iter)
				util::TransformEndian(&*iter, fmt::vertexFormat, util::littleEndian);
			// read influences
			std::vector<fmt::Influence> influences(header.influences);
			stream->Read(&*influences.begin(), sizeof *influences.begin() * influences.size());
			for (std::vector<fmt::Influence>::iterator iter(influences.begin()); iter != influences.end(); ++iter)
				util::TransformEndian(&*iter, fmt::influenceFormat, util::littleEndian);
			// create mesh
			const std::unique_ptr<Mesh> mesh(new Mesh);
			// read and fill bones
			mesh->bones.resize(header.bones);
			for (Mesh::Bones::iterator bone(mesh->bones.begin()); bone != mesh->bones.end(); ++bone)
			{
				fmt::Bone fmtBone;
				stream->Read(&fmtBone, sizeof fmtBone);
				util::TransformEndian(&fmtBone, fmt::boneFormat, util::littleEndian);
				std::vector<char> nameBuffer(fmtBone.nameSize);
				stream->Read(&*nameBuffer.begin(), nameBuffer.size());
				bone->assign(nameBuffer.begin(), nameBuffer.end());
			}
			// done reading
			stream.reset();
			// validate ranges
			for (std::vector<fmt::Face>::const_iterator face(faces.begin()); face != faces.end(); ++face)
				for (const std::uint32_t *vertex = face->vertices; vertex != face->vertices + 3; ++vertex)
					if (*vertex >= vertices.size())
						THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("vertex index out of range")))
			for (std::vector<fmt::Vertex>::const_iterator vertex(vertices.begin()); vertex != vertices.end(); ++vertex)
				if (vertex->influenceBase + vertex->influences > influences.size())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("influence index out of range")))
			for (std::vector<fmt::Influence>::const_iterator influence(influences.begin()); influence != influences.end(); ++influence)
				if (influence->bone >= mesh->bones.size())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("bone index out of range")))
			// fill vertices
			mesh->vertices.reserve(vertices.size());
			for (std::vector<fmt::Vertex>::const_iterator fmtVertex(vertices.begin()); fmtVertex != vertices.end(); ++fmtVertex)
			{
				Mesh::Vertex vertex =
				{
					math::Vec3(fmtVertex->co[0], fmtVertex->co[1], fmtVertex->co[2]),
					math::Vec3(fmtVertex->no[0], fmtVertex->no[1], fmtVertex->no[2]),
					math::Vec2(fmtVertex->uv[0], fmtVertex->uv[1]),
					math::Vec2(), // FIXME: texture coordinate 2 goes here
					math::Vec2()  // FIXME: texture coordinate 3 goes here
				};
				// fill influences
				vertex.influences.reserve(fmtVertex->influences);
				for (std::vector<fmt::Influence>::const_iterator fmtInfluence(influences.begin() + fmtVertex->influenceBase); fmtInfluence != influences.begin() + fmtVertex->influenceBase + fmtVertex->influences; ++fmtInfluence)
				{
					Mesh::Vertex::Influence influence =
					{
						mesh->bones.begin() + fmtInfluence->bone,
						fmtInfluence->weight
					};
					vertex.influences.push_back(influence);
				}
				mesh->vertices.push_back(vertex);
			}
			// fill faces
			mesh->faces.reserve(faces.size());
			for (std::vector<fmt::Face>::const_iterator fmtFace(faces.begin()); fmtFace != faces.end(); ++fmtFace)
			{
				Mesh::Face face =
				{
					mesh->vertices.begin() + fmtFace->vertices[0],
					mesh->vertices.begin() + fmtFace->vertices[1],
					mesh->vertices.begin() + fmtFace->vertices[2]
				};
				mesh->faces.push_back(face);
			}
			return mesh.release();
		}

		LoadFunction GetNativeMeshLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			char sig[sizeof fmt::sig];
			return stream->ReadSome(sig, sizeof sig) == sizeof sig &&
				!std::memcmp(sig, fmt::sig, sizeof sig) ? LoadNativeMesh : LoadFunction();
		}

		REGISTER_LOADER(Mesh, GetNativeMeshLoader,
			"mesh,pagemesh", "", "", true)
	}
}
