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

#ifndef    page_local_vid_opengl_ViewContext_hpp
#   define page_local_vid_opengl_ViewContext_hpp

#	include <functional> // function
#	include "../../math/fwd.hpp" // OrthoFrustum
#	include "../../math/Matrix.hpp"
#	include "../../phys/Form.hpp" // Form::Part
#	include "../../phys/Scene.hpp" // Scene::View
#	include "../../res/type/Material.hpp" // Material::Pass
#	include "../../util/Optional.hpp"
#	include "../ViewContext.hpp"
#	include "AttribGuard.hpp"
#	include "MatrixGuard.hpp"
#	include "RenderTargetPool.hpp" // RenderTargetPool::Pooled
#	include "Vertex.hpp" // VertexFormat

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class DrawContext;

			struct ViewContext : vid::ViewContext
			{
				typedef DrawContext Base;

				// construct/destroy
				ViewContext(Base &, const math::ViewFrustum<> &);
				~ViewContext();

				// base context access
				Base &GetBase();
				const Base &GetBase() const;

				// scene rendering
				void Draw(const phys::Scene &);

				private:
				enum ShaderType
				{
					basicShaderType,
					emissiveSpecularShaderType,
					normalShaderType,
					shadowShaderType
				};
				enum FixedType
				{
					basicFixedType,
					zcullFixedType
				};
				struct ShadowAttachment
				{
					RenderTargetPool::Pooled renderTarget;
					math::Matrix<3, 4> matrix;
				};

				// mesh rendering
				void Draw(const phys::Form &, ShaderType, const util::Optional<ShadowAttachment> & = nullptr);
				void Draw(const phys::Form &, FixedType);
				void Draw(const phys::Scene::View<phys::Form>::Type &, ShaderType, const util::Optional<ShadowAttachment> & = nullptr);
				void Draw(const phys::Scene::View<phys::Form>::Type &, FixedType);

				// mesh rendering implementation
				typedef std::function<VertexFormat (const res::Material::Pass &, MatrixGuard &)> PrepMaterialCallback;
				void Draw(const phys::Form &, const PrepMaterialCallback &, bool multipass);
				void Draw(const phys::Form::Part &, const PrepMaterialCallback &, bool multipass);
				void Draw(const phys::Form::Part &, const VertexFormat &);

				// shader material setup
				VertexFormat PrepShaderMaterial(const res::Material::Pass &, MatrixGuard &, ShaderType, const util::Optional<ShadowAttachment> & = nullptr);
				VertexFormat PrepBasicShaderMaterial(const res::Material::Pass &, MatrixGuard &, const util::Optional<ShadowAttachment> & = nullptr);
				VertexFormat PrepEmissiveSpecularShaderMaterial(const res::Material::Pass &, MatrixGuard &, const util::Optional<ShadowAttachment> & = nullptr);
				VertexFormat PrepNormalShaderMaterial(const res::Material::Pass &, MatrixGuard &);
				VertexFormat PrepShadowShaderMaterial(const res::Material::Pass &, MatrixGuard &);

				// fixed-function material setup
				VertexFormat PrepFixedMaterial(const res::Material::Pass &, MatrixGuard &, FixedType);
				VertexFormat PrepBasicFixedMaterial(const res::Material::Pass &, MatrixGuard &);
				VertexFormat PrepZcullFixedMaterial(const res::Material::Pass &, MatrixGuard &);

				// shadow rendering
				ShadowAttachment DrawShadow(const math::OrthoFrustum<> &,
					const phys::Scene::View<phys::Form>::Type &);

				// sprite rendering
				// FIXME: implement

				// debug rendering
				void Draw(const res::Track &);
				void Draw(const phys::Scene::View<phys::Collidable>::Type &);
				void DrawBounds(const phys::Scene::View<phys::Form>::Type &);
				void DrawBounds(const phys::Form &);
				void DrawSkeleton(const phys::Scene::View<phys::Form>::Type &);
				void DrawSkeleton(const phys::attrib::Pose &);

				// decisions
				bool UseEmissiveSpecularGlow() const;

				AttribGuard attribGuard;
				MatrixGuard matrixGuard;
			};
		}
	}
}

#endif
