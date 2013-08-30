// skeletal deformation

#ifndef    page_local_phys_attrib_Pose_hpp
#   define page_local_phys_attrib_Pose_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include <boost/iterator/indirect_iterator.hpp>
#	include <boost/signal.hpp>

#	include "../../math/Matrix.hpp"
#	include "../../res/type/Skeleton.hpp" // Skeleton::Bone
#	include "../../util/Identifiable.hpp"
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
					const math::Mat3 &GetNormSkinMatrix() const;
					const math::Matrix<3, 4> &GetPoseMatrix() const;
					const math::Mat3 &GetNormPoseMatrix() const;
					const math::Matrix<3, 4> &GetInvPoseMatrix() const;
					const math::Mat3 &GetNormInvPoseMatrix() const;
					const math::Matrix<3, 4> &GetBindMatrix() const;
					const math::Mat3 &GetNormBindMatrix() const;
					const math::Matrix<3, 4> &GetInvBindMatrix() const;
					const math::Mat3 &GetNormInvBindMatrix() const;

					// bind-pose
					void SetBind();
					void Reset();

					// bind-pose transformation
					math::Vec3 GetBindPosition() const;
					math::Quat<> GetBindOrientation() const;
					math::Vec3 GetBindScale() const;

					// mark dirty on transformation
					void SetPosition(const math::Vec3 &);
					void SetOrientation(const math::Quat<> &);
					void SetNormal(const math::Vec3 &);
					void SetScale(const math::Vec3 &);
					void SetMatrix(const math::Matrix<3, 4> &);
					void SetMatrix(const math::Mat3 &);

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
					mutable math::Mat3 normSkinMatrix;
					mutable math::Matrix<3, 4> poseMatrix;
					mutable math::Mat3 normPoseMatrix;
					mutable math::Matrix<3, 4> invPoseMatrix;
					mutable math::Mat3 normInvPoseMatrix;
					math::Matrix<3, 4> bindMatrix;
					math::Mat3 normBindMatrix;
					math::Matrix<3, 4> invBindMatrix;
					math::Mat3 normInvBindMatrix;
					math::Vec3 bindPosition;
					math::Quat<> bindOrientation;
					math::Vec3 bindScale;
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
				typedef boost::indirect_iterator<OrderedBones::const_iterator> BoneIterator;
				typedef boost::indirect_iterator<OrderedBones::const_iterator, const Bone> ConstBoneIterator;

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
				boost::signal<void ()> dirtyPoseSig;

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
