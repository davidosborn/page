#include "../../phys/attrib/Pose.hpp" // Pose->util::Identifiable
#include "../../phys/Skin.hpp"
#include "SkinProxy.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	SkinProxy::SkinProxy(const Proxy<res::Mesh> &mesh, const phys::attrib::Pose &pose) :
		SkinProxy(mesh, pose.GetId()) {}

	SkinProxy::SkinProxy(const Proxy<res::Mesh> &mesh, util::Identifiable::Id poseId) :
		BasicProxy<phys::Skin>(Signature("skin", mesh, poseId)),
		mesh(mesh), poseId(poseId) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto SkinProxy::DoLock() const override -> pointer
	{
		return std::make_shared<phys::Skin>(*mesh,
			util::GetReferenceById<phys::attrib::Pose>(poseId));
	}
}}
