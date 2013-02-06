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

#include <algorithm> // find
#include "Pose.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// pose bone
			// construct/copy/assign
			Pose::Bone::Bone(Pose &pose, const res::Skeleton::Bone &skelBone, Bone *parent) :
				PositionOrientationScale(skelBone.position, skelBone.orientation, skelBone.scale),
				pose(&pose), name(skelBone.name), parent(parent),
				dirty(true), invDirty(true)
			{
				SetBind();
				if (parent) parent->children.push_back(this);
			}
			Pose::Bone::Bone(Pose &pose, const Bone &other, Bone *parent) :
				PositionOrientationScale(other), pose(&pose),
				name(other.name), parent(parent),
				dirty(other.dirty), invDirty(other.invDirty),
				skinMatrix(other.skinMatrix), normSkinMatrix(other.normSkinMatrix),
				poseMatrix(other.poseMatrix), normPoseMatrix(other.normPoseMatrix),
				invPoseMatrix(other.invPoseMatrix), normInvPoseMatrix(other.normInvPoseMatrix),
				bindMatrix(other.bindMatrix), normBindMatrix(other.normBindMatrix),
				invBindMatrix(other.invBindMatrix), normInvBindMatrix(other.normInvBindMatrix),
				bindPosition(other.bindPosition), bindOrientation(other.bindOrientation), bindScale(other.bindScale)
			{
				if (parent)
				{
					Children::iterator child(std::find(parent->children.begin(), parent->children.end(), &other));
					if (child != parent->children.end()) *child = this;
					else parent->children.push_back(this);
				}
			}
			Pose::Bone::Bone(const Bone &other) :
				PositionOrientationScale(other), pose(other.pose),
				name(other.name), parent(other.parent), children(other.children),
				dirty(other.dirty), invDirty(other.invDirty),
				skinMatrix(other.skinMatrix), normSkinMatrix(other.normSkinMatrix),
				poseMatrix(other.poseMatrix), normPoseMatrix(other.normPoseMatrix),
				invPoseMatrix(other.invPoseMatrix), normInvPoseMatrix(other.normInvPoseMatrix),
				bindMatrix(other.bindMatrix), normBindMatrix(other.normBindMatrix),
				invBindMatrix(other.invBindMatrix), normInvBindMatrix(other.normInvBindMatrix),
				bindPosition(other.bindPosition), bindOrientation(other.bindOrientation), bindScale(other.bindScale)
			{
				if (parent)
				{
					Children::iterator child(std::find(parent->children.begin(), parent->children.end(), &other));
					if (child != parent->children.end()) *child = this;
					else parent->children.push_back(this);
				}
			}
			Pose::Bone &Pose::Bone::operator =(const Bone &other)
			{
				PositionOrientationScale::operator =(other);
				pose = other.pose;
				name = other.name;
				parent = other.parent;
				children = other.children;
				dirty = other.dirty;
				invDirty = other.invDirty;
				skinMatrix = other.skinMatrix;
				normSkinMatrix = other.normSkinMatrix;
				poseMatrix = other.poseMatrix;
				normPoseMatrix = other.normPoseMatrix;
				invPoseMatrix = other.invPoseMatrix;
				normInvPoseMatrix = other.normInvPoseMatrix;
				bindMatrix = other.bindMatrix;
				normBindMatrix = other.normBindMatrix;
				invBindMatrix = other.invBindMatrix;
				normInvBindMatrix = other.normInvBindMatrix;
				bindPosition = other.bindPosition;
				bindOrientation = other.bindOrientation;
				bindScale = other.bindScale;
				if (parent)
				{
					Children::iterator child(std::find(parent->children.begin(), parent->children.end(), &other));
					if (child != parent->children.end()) *child = this;
					else parent->children.push_back(this);
				}
			}

			// name/parent
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

			// matrix access
			const math::Matrix<3, 4> &Pose::Bone::GetSkinMatrix() const
				{ return UpdateDirty(), skinMatrix; }
			const math::Matrix<3> &Pose::Bone::GetNormSkinMatrix() const
				{ return UpdateDirty(), normSkinMatrix; }
			const math::Matrix<3, 4> &Pose::Bone::GetPoseMatrix() const
				{ return UpdateDirty(), poseMatrix; }
			const math::Matrix<3> &Pose::Bone::GetNormPoseMatrix() const
				{ return UpdateDirty(), normPoseMatrix; }
			const math::Matrix<3, 4> &Pose::Bone::GetInvPoseMatrix() const
				{ return UpdateInvDirty(), invPoseMatrix; }
			const math::Matrix<3> &Pose::Bone::GetNormInvPoseMatrix() const
				{ return UpdateInvDirty(), normInvPoseMatrix; }
			const math::Matrix<3, 4> &Pose::Bone::GetBindMatrix() const
				{ return bindMatrix; }
			const math::Matrix<3> &Pose::Bone::GetNormBindMatrix() const
				{ return normBindMatrix; }
			const math::Matrix<3, 4> &Pose::Bone::GetInvBindMatrix() const
				{ return invBindMatrix; }
			const math::Matrix<3> &Pose::Bone::GetNormInvBindMatrix() const
				{ return normInvBindMatrix; }

			// bind-pose
			void Pose::Bone::SetBind()
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
			void Pose::Bone::Reset()
			{
				SetPosition(bindPosition);
				SetOrientation(bindOrientation);
				SetScale(bindScale);
			}

			// bind-pose transformation
			math::Vector<3> Pose::Bone::GetBindPosition() const
				{ return bindPosition; }
			math::Quat<> Pose::Bone::GetBindOrientation() const
				{ return bindOrientation; }
			math::Vector<3> Pose::Bone::GetBindScale() const
				{ return bindScale; }

			// mark dirty on transformation
			void Pose::Bone::SetPosition(const math::Vector<3> &position)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(position != PositionOrientationScale::GetPosition()))
				{
					PositionOrientationScale::SetPosition(position);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}
			void Pose::Bone::SetOrientation(const math::Quat<> &orientation)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (orientation != PositionOrientationScale::GetOrientation())
				{
					PositionOrientationScale::SetOrientation(orientation);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}
			void Pose::Bone::SetNormal(const math::Vector<3> &normal)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(normal != PositionOrientationScale::GetNormal()))
				{
					PositionOrientationScale::SetNormal(normal);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}
			void Pose::Bone::SetScale(const math::Vector<3> &scale)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(scale != PositionOrientationScale::GetScale()))
				{
					PositionOrientationScale::SetScale(scale);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}
			void Pose::Bone::SetMatrix(const math::Matrix<3, 4> &matrix)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (matrix != PositionOrientationScale::GetMatrix())
				{
					PositionOrientationScale::SetMatrix(matrix);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}
			void Pose::Bone::SetMatrix(const math::Matrix<3> &matrix)
			{
				// NOTE: comparing value to avoid redundant dirty marking
				if (matrix != math::Matrix<3>(PositionOrientationScale::GetMatrix()))
				{
					PositionOrientationScale::SetMatrix(matrix);
					MarkDirty();
					GetPose().dirtyPoseSig();
				}
			}

			// owner access
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

			// dirty
			void Pose::Bone::UpdateDirty() const
			{
				if (dirty)
				{
					poseMatrix = GetMatrix();
					normPoseMatrix = Orientation::GetMatrix();
					if (parent)
					{
						poseMatrix = parent->GetPoseMatrix() * poseMatrix;
						normPoseMatrix = parent->GetNormPoseMatrix() * normPoseMatrix;
					}
					skinMatrix = poseMatrix * invBindMatrix;
					normSkinMatrix = normPoseMatrix * normInvBindMatrix;
					dirty = false;
				}
			}
			void Pose::Bone::UpdateInvDirty() const
			{
				if (invDirty)
				{
					invPoseMatrix = GetInvMatrix();
					normInvPoseMatrix = Orientation::GetInvMatrix();
					if (parent)
					{
						invPoseMatrix *= parent->GetInvPoseMatrix();
						normInvPoseMatrix *= parent->GetNormInvPoseMatrix();
					}
					invDirty = false;
				}
			}
			void Pose::Bone::MarkDirty() const
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
			}
			void Pose::Bone::MarkInvDirty() const
			{
				if (!invDirty)
				{
					for (Children::const_iterator iter(children.begin()); iter != children.end(); ++iter)
					{
						const Bone &child(**iter);
						child.MarkInvDirty();
					}
					invDirty = true;
				}
			}

			// pose
			// construct/copy/assign
			Pose::Pose() {}
			Pose::Pose(const res::Skeleton &skel)
			{
				for (res::Skeleton::Bones::const_iterator bone(skel.bones.begin()); bone != skel.bones.end(); ++bone)
					CopyTreePath(*bone);
			}
			Pose::Pose(const Pose &other)
			{
				orderedBones.reserve(other.orderedBones.size());
				for (ConstBoneIterator otherBone(other.BeginBones()); otherBone != other.EndBones(); ++otherBone)
				{
					const Bone *otherBoneParent = otherBone->GetParent();
					Bone *boneParent = otherBoneParent ? &bones.find(otherBoneParent->GetName())->second : 0;
					orderedBones.push_back(&bones.insert(std::make_pair(otherBone->GetName(), Bone(*this, *otherBone, boneParent))).first->second);
				}
			}
			Pose &Pose::operator =(const Pose &other)
			{
				orderedBones.reserve(other.orderedBones.size());
				for (ConstBoneIterator otherBone(other.BeginBones()); otherBone != other.EndBones(); ++otherBone)
				{
					const Bone *otherBoneParent = otherBone->GetParent();
					Bone *boneParent = otherBoneParent ? &bones.find(otherBoneParent->GetName())->second : 0;
					orderedBones.push_back(&bones.insert(std::make_pair(otherBone->GetName(), Bone(*this, *otherBone, boneParent))).first->second);
				}
				return *this;
			}

			// status
			bool Pose::IsPosed() const
			{
				return !bones.empty();
			}

			// bone iterators
			Pose::BoneIterator Pose::BeginBones()
				{ return BoneIterator(orderedBones.begin()); }
			Pose::ConstBoneIterator Pose::BeginBones() const
				{ return ConstBoneIterator(orderedBones.begin()); }
			Pose::BoneIterator Pose::EndBones()
				{ return BoneIterator(orderedBones.end()); }
			Pose::ConstBoneIterator Pose::EndBones() const
				{ return ConstBoneIterator(orderedBones.end()); }

			// bone access
			Pose::Bone *Pose::GetBone(const std::string &name)
			{
				Pose::Bones::iterator iter(bones.find(name));
				return iter != bones.end() ? &iter->second : 0;
			}
			const Pose::Bone *Pose::GetBone(const std::string &name) const
			{
				Pose::Bones::const_iterator iter(bones.find(name));
				return iter != bones.end() ? &iter->second : 0;
			}

			// bind-pose
			void Pose::SetBindPose()
			{
				for (BoneIterator bone(BeginBones()); bone != EndBones(); ++bone)
					bone->SetBind();
			}
			void Pose::ResetPose()
			{
				for (BoneIterator bone(BeginBones()); bone != EndBones(); ++bone)
					bone->Reset();
			}

			// frame serialization
			Frame Pose::GetFrame() const
			{
				Frame frame(PositionOrientationScale::GetFrame());
				for (Bones::const_iterator iter(bones.begin()); iter != bones.end(); ++iter)
				{
					const Bone &bone(iter->second);
					Frame::Bone frameBone =
					{
						bone.GetPosition(),
						bone.GetOrientation(),
						bone.GetScale()
					};
					frame.bones.insert(std::make_pair(iter->first, frameBone));
				}
				return frame;
			}
			void Pose::Update(const Frame &frame)
			{
				PositionOrientationScale::Update(frame);
				for (Frame::Bones::const_iterator iter(frame.bones.begin()); iter != frame.bones.end(); ++iter)
				{
					if (Bone *bone = GetBone(iter->first))
					{
						const Frame::Bone &frameBone(iter->second);
						if (frameBone.position) bone->SetPosition(*frameBone.position);
						if (frameBone.orientation) bone->SetOrientation(*frameBone.orientation);
						if (frameBone.scale) bone->SetScale(*frameBone.scale);
					}
				}
			}

			// rebuild from new skeleton
			void Pose::Rebuild(const res::Skeleton &skel)
			{
				// FIXME: implement
			}

			// insert dependency path from skeleton
			Pose::Bone &Pose::CopyTreePath(const res::Skeleton::Bone &skelBone)
			{
				Bones::iterator boneIter(bones.find(skelBone.name));
				if (boneIter == bones.end())
				{
					Bone *parentBone = skelBone.parent ? &CopyTreePath(*skelBone.parent) : 0;
					boneIter = bones.insert(std::make_pair(skelBone.name, Bone(*this, skelBone, parentBone))).first;
					orderedBones.push_back(&boneIter->second);
				}
				return boneIter->second;
			}
		}
	}
}

