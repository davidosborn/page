#ifndef    page_local_vid_opengl_ViewContext_hpp
#   define page_local_vid_opengl_ViewContext_hpp

#	include <functional> // function

#	include <boost/optional.hpp>

#	include "../../math/fwd.hpp" // OrthoFrustum
#	include "../../math/Matrix.hpp"
#	include "../../phys/Form.hpp" // Form::Part
#	include "../../phys/Scene.hpp" // Scene::View
#	include "../../res/type/Material.hpp" // Material::Pass
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
				void Draw(const phys::Form &, ShaderType, const boost::optional<ShadowAttachment> & = nullptr);
				void Draw(const phys::Form &, FixedType);
				void Draw(const phys::Scene::View<phys::Form>::Type &, ShaderType, const boost::optional<ShadowAttachment> & = nullptr);
				void Draw(const phys::Scene::View<phys::Form>::Type &, FixedType);

				// mesh rendering implementation
				typedef std::function<VertexFormat (const res::Material::Pass &, MatrixGuard &)> PrepMaterialCallback;
				void Draw(const phys::Form &, const PrepMaterialCallback &, bool multipass);
				void Draw(const phys::Form::Part &, const PrepMaterialCallback &, bool multipass);
				void Draw(const phys::Form::Part &, const VertexFormat &);

				// shader material setup
				VertexFormat PrepShaderMaterial(const res::Material::Pass &, MatrixGuard &, ShaderType, const boost::optional<ShadowAttachment> & = nullptr);
				VertexFormat PrepBasicShaderMaterial(const res::Material::Pass &, MatrixGuard &, const boost::optional<ShadowAttachment> & = nullptr);
				VertexFormat PrepEmissiveSpecularShaderMaterial(const res::Material::Pass &, MatrixGuard &, const boost::optional<ShadowAttachment> & = nullptr);
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
