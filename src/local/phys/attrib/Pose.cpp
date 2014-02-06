#include <utility> // move, swap

#include <boost/range/algorithm/find.hpp> // find

#include "Pose.hpp"
#include "../../util/iterator/zip.hpp"

namespace page { namespace phys { namespace attrib
{
////////// Pose::Bone //////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Pose::Bone::Bone(Pose &pose, const std::string &name) :
		pose(&pose), name(name)
	{
		Init();
	}

	Pose::Bone::Bone(Pose &pose, const res::Skeleton::Bone &skelBone, Bone *parent) :
		PositionOrientationScale(skelBone.position, skelBone.orientation, skelBone.scale),
		pose(&pose), name(skelBone.name)
	{
		if (parent != nullptr)
			SetParent(*parent);

		Init();
	}

	Pose::Bone::Bone(const Bone &other) :
		PositionOrientationScale(other),
		pose                    (other.pose),
		name                    (other.name),
		parentIndex             (other.parentIndex),
		childIndices            (other.childIndices),
		bindPosition            (other.bindPosition),
		bindOrientation         (other.bindOrientation),
		bindScale               (other.bindScale),
		bindMatrix              (other.bindMatrix),
		normBindMatrix          (other.normBindMatrix),
		invBindMatrix           (other.invBindMatrix),
		normInvBindMatrix       (other.normInvBindMatrix),
		dirty                   (other.dirty),
		poseMatrix              (other.poseMatrix),
		normPoseMatrix          (other.normPoseMatrix),
		invPoseMatrix           (other.invPoseMatrix),
		normInvPoseMatrix       (other.normInvPoseMatrix),
		skinMatrix              (other.skinMatrix),
		normSkinMatrix          (other.normSkinMatrix)
	{
		Init();
	}

	Pose::Bone::Bone(Bone &&other) :
		PositionOrientationScale(std::move(other)),
		pose                    (std::move(other.pose)),
		name                    (std::move(other.name)),
		parentIndex             (std::move(other.parentIndex)),
		childIndices            (std::move(other.childIndices)),
		bindPosition            (std::move(other.bindPosition)),
		bindOrientation         (std::move(other.bindOrientation)),
		bindScale               (std::move(other.bindScale)),
		bindMatrix              (std::move(other.bindMatrix)),
		normBindMatrix          (std::move(other.normBindMatrix)),
		invBindMatrix           (std::move(other.invBindMatrix)),
		normInvBindMatrix       (std::move(other.normInvBindMatrix)),
		dirty                   (std::move(other.dirty)),
		poseMatrix              (std::move(other.poseMatrix)),
		normPoseMatrix          (std::move(other.normPoseMatrix)),
		invPoseMatrix           (std::move(other.invPoseMatrix)),
		normInvPoseMatrix       (std::move(other.normInvPoseMatrix)),
		skinMatrix              (std::move(other.skinMatrix)),
		normSkinMatrix          (std::move(other.normSkinMatrix))
	{
		Init();
	}

	Pose::Bone &Pose::Bone::operator =(Bone other)
	{
		std::swap(*this, other);
		return *this;
	}

	void Pose::Bone::Init()
	{
		dirtyTransformSig.connect([this]
		{
			MarkDirty();

			/**
			 * @todo This signal gets called every time a bone is transformed,
			 * which could result in a lot of unnecessary function calls.  A
			 * polling approach would make more sense.
			 */
			pose->dirtyPoseSig();
		});
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

	const std::string &Pose::Bone::GetName() const
	{
		return name;
	}

	unsigned Pose::Bone::GetIndex() const
	{
		return this - pose->bones.data();
	}

	Pose::Bone *Pose::Bone::GetParent()
	{
		return parentIndex ? &pose->bones[*parentIndex] : nullptr;
	}

	const Pose::Bone *Pose::Bone::GetParent() const
	{
		return parentIndex ? &pose->bones[*parentIndex] : nullptr;
	}

	/*----------+
	| modifiers |
	+----------*/

	void Pose::Bone::SetParent(Bone &parent)
	{
		assert(parent.pose == pose);
		parentIndex = parent.GetIndex();
		parent.childIndices.push_back(GetIndex());
	}

	void Pose::Bone::Detach()
	{
		// detach parent
		if (parentIndex)
		{
			auto &parentChildIndices(pose->bones[*parentIndex].childIndices);
			auto iter(boost::find(parentChildIndices, GetIndex()));
			assert(iter != parentChildIndices.end());
			parentChildIndices.erase(iter);
		}

		// detach children
		for (const auto &childIndex : childIndices)
		{
			auto &childParentIndex(pose->bones[childIndex].parentIndex);
			assert(childParentIndex == GetIndex());
			childParentIndex = boost::none;
		}
	}

	/*----------+
	| bind pose |
	+----------*/

	const math::Vec3 &Pose::Bone::GetBindPosition() const
	{
		return bindPosition;
	}

	const math::Quat<> &Pose::Bone::GetBindOrientation() const
	{
		return bindOrientation;
	}

	const math::Vec3 &Pose::Bone::GetBindScale() const
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
		auto parent(GetParent());

		// bind matrix
		bindMatrix     = GetMatrix();
		normBindMatrix = Orientation::GetMatrix();
		if (parent)
		{
			bindMatrix     = parent->bindMatrix * bindMatrix;
			normBindMatrix = parent->normBindMatrix * normBindMatrix;
		}

		// inverse bind matrix
		invBindMatrix     = GetInvMatrix();
		normInvBindMatrix = Orientation::GetInvMatrix();
		if (parent)
		{
			invBindMatrix     *= parent->invBindMatrix;
			normInvBindMatrix *= parent->normInvBindMatrix;
		}

		// bind transformation
		bindPosition    = GetPosition();
		bindOrientation = GetOrientation();
		bindScale       = GetScale();
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

			auto parent(GetParent());
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
			for (const auto &childIndex : childIndices)
				pose->bones[childIndex].MarkDirty();

			dirty = true;
		}
	}

////////// Pose ////////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Pose::Pose(const res::Skeleton &skeleton)
	{
		// create the bones as orphans
		for (const auto &skelBone : skeleton.bones)
			bones.emplace_back(*this, skelBone);

		// attach child bones to parents
		for (auto bone : util::zip(bones, skeleton.bones))
			if (bone.get<1>().parent != nullptr)
				bone.get<0>().SetParent(bones[bone.get<1>().parent - skeleton.bones.data()]);

		SetBindPose();
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
			bones.emplace_back(*this, skelBone, skelBone.parent ? &CopyTreePath(*skelBone.parent) : nullptr);
			boneIter = bonesByName.insert(std::make_pair(skelBone.name, bones.size() - 1)).first;
		}
		return bones[boneIter->second];
	}

	/*------+
	| bones |
	+------*/

	const std::vector<Pose::Bone> &Pose::GetBones() const
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
