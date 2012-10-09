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

// skeletal deformation

#ifndef    page_phys_attrib_Pose_hpp
#   define page_phys_attrib_Pose_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>
#	include "../../math/Matrix.hpp"
#	include "../../res/type/Skeleton.hpp" // Skeleton::Bone
#	include "../../util/Identifiable.hpp"
#	include "../../util/iterator/indirect_iterator.hpp"
#	include "../../util/Signal.hpp"
#	include "PositionOrientationScale.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Pose : PositionOrientationScale, util::Identifiable
			{
				struct Bone : PositionOrientationScale
				{
					// construct/copy/assign
					// NOTE: copy transfers parentage like auto_ptr
					// FIXME: should require std::move to support copy
					Bone(Pose &, const res::Skeleton::Bone &, Bone *parent);
					Bone(Pose &, const Bone &, Bone *parent);
					Bone(const Bone &);
					Bone &operator =(const Bone &);

					// name/parent
					std::string GetName() const;
					Bone *GetParent();
					const Bone *GetParent() const;

					// matrix access
					const math::Matrix<3, 4> &GetSkinMatrix() const;
					const math::Matrix<3> &GetNormSkinMatrix() const;
					const math::Matrix<3, 4> &GetPoseMatrix() const;
					const math::Matrix<3> &GetNormPoseMatrix() const;
					const math::Matrix<3, 4> &GetInvPoseMatrix() const;
					const math::Matrix<3> &GetNormInvPoseMatrix() const;
					const math::Matrix<3, 4> &GetBindMatrix() const;
					const math::Matrix<3> &GetNormBindMatrix() const;
					const math::Matrix<3, 4> &GetInvBindMatrix() const;
					const math::Matrix<3> &GetNormInvBindMatrix() const;

					// bind-pose
					void SetBind();
					void Reset();

					// bind-pose transformation
					math::Vector<3> GetBindPosition() const;
					math::Quat<> GetBindOrientation() const;
					math::Vector<3> GetBindScale() const;

					// mark dirty on transformation
					void SetPosition(const math::Vector<3> &);
					void SetOrientation(const math::Quat<> &);
					void SetNormal(const math::Vector<3> &);
					void SetScale(const math::Vector<3> &);
					void SetMatrix(const math::Matrix<3, 4> &);
					void SetMatrix(const math::Matrix<3> &);

					// owner access
					Pose &GetPose();
					const Pose &GetPose() const;

					private:
					// dirty
					void UpdateDirty() const;
					void UpdateInvDirty() const;
					void MarkDirty() const;
					void MarkInvDirty() const;

					Pose *pose;
					std::string name;
					Bone *parent;
					typedef std::vector<Bone *> Children;
					Children children;
					mutable bool dirty, invDirty;
					mutable math::Matrix<3, 4> skinMatrix;
					mutable math::Matrix<3> normSkinMatrix;
					mutable math::Matrix<3, 4> poseMatrix;
					mutable math::Matrix<3> normPoseMatrix;
					mutable math::Matrix<3, 4> invPoseMatrix;
					mutable math::Matrix<3> normInvPoseMatrix;
					math::Matrix<3, 4> bindMatrix;
					math::Matrix<3> normBindMatrix;
					math::Matrix<3, 4> invBindMatrix;
					math::Matrix<3> normInvBindMatrix;
					math::Vector<3> bindPosition;
					math::Quat<> bindOrientation;
					math::Vector<3> bindScale;
				};

				// construct/copy/assign
				Pose();
				explicit Pose(const res::Skeleton &);
				Pose(const Pose &);
				Pose &operator =(const Pose &);

				// status
				bool IsPosed() const;

				private:
				// container types required for bone iterator types
				typedef std::unordered_map<std::string, Bone> Bones;
				typedef std::vector<Bone *> OrderedBones;

				public:
				// bone iterator types
				typedef util::indirect_iterator<OrderedBones::const_iterator> BoneIterator;
				typedef util::indirect_iterator<OrderedBones::const_iterator, const Bone> ConstBoneIterator;

				// bone iterators
				BoneIterator BeginBones();
				ConstBoneIterator BeginBones() const;
				BoneIterator EndBones();
				ConstBoneIterator EndBones() const;

				// bone access
				Bone *GetBone(const std::string &);
				const Bone *GetBone(const std::string &) const;

				// bind-pose
				void SetBindPose();
				void ResetPose();

				// signals
				util::Signal<void ()> dirtyPoseSig;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				// rebuild from new skeleton
				// maintains state of existing bones
				void Rebuild(const res::Skeleton &);

				// insert dependency path from skeleton
				Bone &CopyTreePath(const res::Skeleton::Bone &);

				Bones bones;
				OrderedBones orderedBones;
			};
		}
	}
}

#endif
