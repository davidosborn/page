#include <utility> // move

#include <boost/range/algorithm/find.hpp> // find

#include "Pose.hpp"

namespace page { namespace phys { namespace attrib
{
////////// Pose::Bone //////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Pose::Bone::Bone()
	{
		dirtyTransformSig.Connect([]
		{
			if (!dirty)
			{
				for (Children::const_iterator iter(children.begin()); iter != children.end(); ++iter)
				{
					const Bone &child(**iter);
					child.MarkDirty();
				}
				dirty = true;
			}
			/**
			 * @todo This signal gets called every time a bone is transformed,
			 * which could result in a lot of unnecessary function calls.  A
			 * polling approach would make more sense.
			 */
			GetPose().dirtyPoseSig();
		});
	}

	Pose::Bone::Bone(Pose &pose, const res::Skeleton::Bone &skelBone, Bone *parent) :
		PositionOrientationScale(bone.position, bone.orientation, bone.scale),
		pose(&pose), name(bone.name), parent(parent)
	{
		SetBindPose();

		if (parent)
			parent->children.push_back(this);
	}

	Pose::Bone::Bone(Pose &pose, const Bone &other, Bone *parent) :
		PositionOrientationScale(other),

		pose             (&pose),
		name             (other.name),
		parent           (parent),
		bindPosition     (other.bindPosition),
		bindOrientation  (other.bindOrientation),
		bindScale        (other.bindScale),
		bindMatrix       (other.bindMatrix),
		normBindMatrix   (other.normBindMatrix),
		invBindMatrix    (other.invBindMatrix),
		normInvBindMatrix(other.normInvBindMatrix),
		dirty            (other.dirty),
		skinMatrix       (other.skinMatrix),
		normSkinMatrix   (other.normSkinMatrix),
		poseMatrix       (other.poseMatrix),
		normPoseMatrix   (other.normPoseMatrix),
		invPoseMatrix    (other.invPoseMatrix),
		normInvPoseMatrix(other.normInvPoseMatrix),
	{
		if (parent)
			parent->children.push_back(this);
	}

	Pose::Bone::Bone(Pose &pose, Bone &&other, Bone *parent) :
		PositionOrientationScale(std::move(other)),

		pose             (&pose),
		name             (std::move(other.name)),
		parent           (parent),
		bindPosition     (std::move(other.bindPosition)),
		bindOrientation  (std::move(other.bindOrientation)),
		bindScale        (std::move(other.bindScale)),
		bindMatrix       (std::move(other.bindMatrix)),
		normBindMatrix   (std::move(other.normBindMatrix)),
		invBindMatrix    (std::move(other.invBindMatrix)),
		normInvBindMatrix(std::move(other.normInvBindMatrix)),
		dirty            (std::move(other.dirty)),
		skinMatrix       (std::move(other.skinMatrix)),
		normSkinMatrix   (std::move(other.normSkinMatrix)),
		poseMatrix       (std::move(other.poseMatrix)),
		normPoseMatrix   (std::move(other.normPoseMatrix)),
		invPoseMatrix    (std::move(other.invPoseMatrix)),
		normInvPoseMatrix(std::move(other.normInvPoseMatrix))
	{
		Detach();

		if (parent)
			parent->children.push_back(this);
	}

	Pose::Bone::~Bone()
	{
		Detach();
	}

	/*----------+
	| observers |
	+----------*/

	Pose &Pose::Bone::GetPose()
	{
		assert(pose);
		return *pose;
	}

	const Pose &Pose::Bone::GetPose() const
	{
		assert(pose);
		return *pose;
	}

	std::string Pose::Bone::GetName() const
	{
		return name;
	}

	Pose::Bone *Pose::Bone::GetParent()
	{
		return parent;
	}

	const Pose::Bone *Pose::Bone::GetParent() const
	{
		return parent;
	}

	/*----------+
	| modifiers |
	+----------*/

	void Pose::Bone::Detach()
	{
		// detach parent
		if (parent)
		{
			auto iter(boost::find(parent->children, this));
			assert(iter != parent->children.end());
			parent->children.erase(iter);
		}

		// detach children
		for (auto &child : children)
		{
			assert(child.parent == this);
			child.parent = nullptr;
		}
	}

	/*----------+
	| bind pose |
	+----------*/

	math::Vec3 Pose::Bone::GetBindPosition() const
	{
		return bindPosition;
	}

	math::Quat<> Pose::Bone::GetBindOrientation() const
	{
		return bindOrientation;
	}

	math::Vec3 Pose::Bone::GetBindScale() const
	{
		return bindScale;
	}

	const math::Mat34 &Pose::Bone::GetBindMatrix() const
	{
		return bindMatrix;
	}

	const math::Mat3 &Pose::Bone::GetNormBindMatrix() const
	{
		return normBindMatrix;
	}

	const math::Mat34 &Pose::Bone::GetInvBindMatrix() const
	{
		return invBindMatrix;
	}

	const math::Mat3 &Pose::Bone::GetNormInvBindMatrix() const
	{
		return normInvBindMatrix;
	}

	void Pose::Bone::SetBindPose()
	{
		// bind matrix
		bindMatrix = GetMatrix();
		normBindMatrix = Orientation::GetMatrix();
		if (parent)
		{
			bindMatrix = parent->bindMatrix * bindMatrix;
			normBindMatrix = parent->normBindMatrix * normBindMatrix;
		}

		// inverse bind matrix
		invBindMatrix = GetInvMatrix();
		normInvBindMatrix = Orientation::GetInvMatrix();
		if (parent)
		{
			invBindMatrix *= parent->invBindMatrix;
			normInvBindMatrix *= parent->normInvBindMatrix;
		}

		// bind transformation
		bindPosition = GetPosition();
		bindOrientation = GetOrientation();
		bindScale = GetScale();
	}

	void Pose::Bone::ResetToBindPose()
	{
		SetPosition(bindPosition);
		SetOrientation(bindOrientation);
		SetScale(bindScale);
	}

	/*---------------+
	| transformation |
	+---------------*/

	const math::Mat34 &Pose::Bone::GetPoseMatrix() const
	{
		UpdateDirty();
		return poseMatrix;
	}

	const math::Mat3 &Pose::Bone::GetNormPoseMatrix() const
	{
		UpdateDirty();
		return normPoseMatrix;
	}

	const math::Mat34 &Pose::Bone::GetInvPoseMatrix() const
	{
		UpdateDirty();
		return invPoseMatrix;
	}

	const math::Mat3 &Pose::Bone::GetNormInvPoseMatrix() const
	{
		UpdateDirty();
		return normInvPoseMatrix;
	}

	const math::Mat34 &Pose::Bone::GetSkinMatrix() const
	{
		UpdateDirty();
		return skinMatrix;
	}

	const math::Mat3 &Pose::Bone::GetNormSkinMatrix() const
	{
		UpdateDirty();
		return normSkinMatrix;
	}

	void Pose::Bone::UpdateDirty() const
	{
		if (dirty)
		{
			poseMatrix     = GetMatrix();
			normPoseMatrix = Orientation::GetMatrix();

			if (parent)
			{
				poseMatrix     = parent->GetPoseMatrix()     * poseMatrix;
				normPoseMatrix = parent->GetNormPoseMatrix() * normPoseMatrix;
			}

			skinMatrix     = poseMatrix     * invBindMatrix;
			normSkinMatrix = normPoseMatrix * normInvBindMatrix;

			dirty = false;
		}
	}

	void Pose::Bone::MarkDirty() const
	{
		if (!dirty)
		{
			for (const auto &child : children)
				child->MarkDirty();

			dirty = true;
		}
	}

////////// Pose ////////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Pose::Pose(const res::Skeleton &skeleton)
	{
		for (const auto &skelBone : skeleton.bones)
			CopyTreePath(skelBone);
	}

	Pose::Pose(const Pose &other)
	{
		bones.reserve(other.bones.size());
		for (const auto &otherBone : other.bones)
		{
			auto otherBoneParent(otherBone.GetParent());
			auto boneParent(otherBoneParent ? bonesByName.find(otherBoneParent->GetName())->second : nullptr);
			bones.emplace_back(*this, otherBone, boneParent);
			bonesByName.insert(std::make_pair(otherBone.GetName(), &bones.back()));
		}
	}

	Pose &Pose::operator =(Pose other)
	{
		std::swap(*this, other);
		return *this;
	}

	/*----------+
	| observers |
	+----------*/

	bool Pose::IsPosed() const
	{
		return !bones.empty();
	}

	/*----------+
	| modifiers |
	+----------*/

	void Pose::SetBindPose()
	{
		for (auto &bone : bones)
			bone.SetBindPose();
	}

	void Pose::ResetToBindPose()
	{
		for (auto &bone : bones)
			bone.ResetToBindPose();
	}

	void Pose::Rebuild(const res::Skeleton &skeleton)
	{
		// FIXME: implement
	}

	Pose::Bone &Pose::CopyTreePath(const res::Skeleton::Bone &skelBone)
	{
		auto boneIter(bonesByName.find(skelBone.name));
		if (boneIter == bonesByName.end())
		{
			auto parentBone(skelBone.parent ? &CopyTreePath(*skelBone.parent) : nullptr);
			bones.emplace_back(*this, skelBone, parentBone);
			boneIter = bonesByName.insert(std::make_pair(skelBone.name, &bones.back()));
		}
		return *boneIter;
	}

	/*------+
	| bones |
	+------*/

	auto Pose::GetBones()
	{
		return bones;
	}

	Pose::Bone *Pose::GetBone(const std::string &name)
	{
		auto iter(bonesByName.find(name));
		return iter != bonesByName.end() ? &bones[iter->second] : nullptr;
	}

	const Pose::Bone *Pose::GetBone(const std::string &name) const
	{
		auto iter(bonesByName.find(name));
		return iter != bonesByName.end() ? &bones[iter->second] : nullptr;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Pose::GetFrame() const
	{
		Frame frame(PositionOrientationScale::GetFrame());

		// bones
		for (const auto &bone : bones)
		{
			Frame::Bone boneFrame
			{
				bone.GetPosition(),
				bone.GetOrientation(),
				bone.GetScale()
			};
			frame.bones.insert(std::make_pair(bone.GetName(), boneFrame));
		}

		return frame;
	}

	void Pose::SetFrame(const Frame &frame)
	{
		PositionOrientationScale::SetFrame(frame);

		// bones
		for (const auto &kv : frame.bones)
			if (Bone *bone = GetBone(kv.first))
			{
				const auto &boneFrame(kv.second);
				if (boneFrame.position)    bone->SetPosition   (*boneFrame.position);
				if (boneFrame.orientation) bone->SetOrientation(*boneFrame.orientation);
				if (boneFrame.scale)       bone->SetScale      (*boneFrame.scale);
			}
	}
}}}
