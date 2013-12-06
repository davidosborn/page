#ifndef    page_local_phys_attrib_Pose_hpp
#   define page_local_phys_attrib_Pose_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include "../../math/Matrix.hpp"
#	include "../../res/type/Skeleton.hpp" // Skeleton::Bone
#	include "../../util/class/special_member_functions.hpp" // Unmovable
#	include "../../util/copyable_signal.hpp"
#	include "../../util/Identifiable.hpp"
#	include "PositionOrientationScale.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The skeletal deformation part of a node.
	 */
	class Pose :
		public PositionOrientationScale,
		public util::Identifiable
	{
		public:
		/**
		 * An individual bone, as part of a skeletal deformation.
		 */
		class Bone :
			public PositionOrientationScale,
			public util::Unmovable<Bone>
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			Bone(Pose &, const res::Skeleton::Bone &, Bone *parent);
			Bone(Pose &, const Bone &, Bone *parent);
			Bone(Pose &, Bone &&, Bone *parent);

			/*----------+
			| observers |
			+----------*/

			/**
			 * Returns the skeletal deformation that the bone belongs to.
			 */
			Pose &GetPose();

			/**
			 * Returns the skeletal deformation that the bone belongs to.
			 */
			const Pose &GetPose() const;

			/**
			 * Returns the bone's name.
			 */
			std::string GetName() const;

			/**
			 * Returns a pointer to the bone that is a parent of this bone, or
			 * @c nullptr if it is the root bone.
			 */
			Bone *GetParent();

			/**
			 * Returns a pointer to the bone that is a parent of this bone, or
			 * @c nullptr if it is the root bone.
			 */
			const Bone *GetParent() const;

			/*----------+
			| modifiers |
			+----------*/

			/**
			 * Detaches the bone from its parent and children.  The children are
			 * left as orphans, without a parent.
			 */
			void Detach();

			/*----------+
			| bind pose |
			+----------*/

			/**
			 * Returns the position of the bone in its bind pose.
			 */
			math::Vec3 GetBindPosition() const;

			/**
			 * Returns the orientation of the bone in its bind pose.
			 */
			math::Quat<> GetBindOrientation() const;

			/**
			 * Returns the scale of the bone in its bind pose.
			 */
			math::Vec3 GetBindScale() const;

			/**
			 * Returns the bind-pose matrix.
			 */
			const math::Mat34 &GetBindMatrix() const;

			/**
			 * Returns the normalized bind-pose matrix.
			 */
			const math::Mat3 &GetNormBindMatrix() const;

			/**
			 * Returns the inverse-bind-pose matrix.
			 */
			const math::Mat34 &GetInvBindMatrix() const;

			/**
			 * Returns the normalized inverse-bind-pose matrix.
			 */
			const math::Mat3 &GetNormInvBindMatrix() const;

			/**
			 * Uses the current pose as the bind pose.
			 */
			void SetBindPose();

			/**
			 * Resets the pose to the bind pose.
			 */
			void ResetToBindPose();

			/*---------------+
			| transformation |
			+---------------*/

			/**
			 * Returns the pose matrix.
			 */
			const math::Mat34 &GetPoseMatrix() const;

			/**
			 * Returns the normalized pose matrix.
			 */
			const math::Mat3 &GetNormPoseMatrix() const;

			/**
			 * Returns the inverse-pose matrix.
			 */
			const math::Mat34 &GetInvPoseMatrix() const;

			/**
			 * Returns the normalized inverse-pose matrix.
			 */
			const math::Mat3 &GetNormInvPoseMatrix() const;

			/**
			 * Returns the skinning matrix.
			 */
			const math::Mat34 &GetSkinMatrix() const;

			/**
			 * Returns the normalized skinning matrix.
			 */
			const math::Mat3 &GetNormSkinMatrix() const;

			private:
			/**
			 * If the bone has been marked as dirty, regenerate its
			 * transformation matrices to account for its new transformation.
			 */
			void UpdateDirty() const;

			/**
			 * Mark the bone as dirty, meaning that it has been transformed and
			 * its transformation matrices are no longer valid.
			 */
			void MarkDirty() const;

			/*-------------+
			| data members |
			+-------------*/

			/**
			 * The skeletal deformation that the bone belongs to.
			 */
			Pose *pose;

			/**
			 * The bone's name.
			 */
			std::string name;

			/**
			 * The bone's parent.
			 */
			Bone *parent;

			/**
			 * The bone's children.
			 */
			std::vector<Bone *> children;

			/**
			 * The position of the bone in its bind pose.
			 */
			math::Vec3 bindPosition;

			/**
			 * The orientation of the bone in its bind pose.
			 */
			math::Quat<> bindOrientation;

			/**
			 * The scale of the bone in its bind pose.
			 */
			math::Vec3 bindScale;

			/**
			 * The bind-pose matrix.
			 */
			math::Mat34 bindMatrix;

			/**
			 * The normalized bind-pose matrix.
			 */
			math::Mat3 normBindMatrix;

			/**
			 * The inverse-bind-pose matrix.
			 */
			math::Mat34 invBindMatrix;

			/**
			 * The normalized inverse-bind-pose matrix.
			 */
			math::Mat3 normInvBindMatrix;

			/**
			 * @c true if the bone has been transformed, in which case the
			 * following matrices will need to be regenerated.
			 */
			mutable bool dirty = true;

			/**
			 * The pose matrix.
			 */
			mutable math::Mat34 poseMatrix;

			/**
			 * The normalized pose matrix.
			 */
			mutable math::Mat3 normPoseMatrix;

			/**
			 * The inverse-pose matrix.
			 */
			mutable math::Mat34 invPoseMatrix;

			/**
			 * The normalized inverse-pose matrix.
			 */
			mutable math::Mat3 normInvPoseMatrix;

			/**
			 * The skinning matrix.
			 */
			mutable math::Mat34 skinMatrix;

			/**
			 * The normalized skinning matrix.
			 */
			mutable math::Mat3 normSkinMatrix;
		};

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Pose() = default;
		explicit Pose(const res::Skeleton &);
		Pose(const Pose &);
		Pose(Pose &&) = default;
		Pose &operator =(const Pose &);
		Pose &operator =(Pose &&) = default;

		/*----------+
		| observers |
		+----------*/

		/**
		 * Returns @c true if the pose contains one or more bones.
		 */
		bool IsPosed() const;

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Uses the current pose as the bind pose.
		 */
		void SetBindPose();

		/**
		 * Resets the pose to the bind pose.
		 */
		void ResetToBindPose();

		private:
		/**
		 * Rebuilds the pose from a new skeleton while maintaining the state of
		 * the bones in the new skeleton that already exist in the pose.
		 */
		void Rebuild(const res::Skeleton &);

		/**
		 * Adds a bone from a skeleton resource, including its parent bones if
		 * they haven't already been added.
		 */
		Bone &CopyTreePath(const res::Skeleton::Bone &);

		/*------+
		| bones |
		+------*/

		public:
		/**
		 * Returns an ordered sequence of bones.
		 */
		const std::vector<Bone> &GetBones() const;

		/**
		 * Returns a pointer to the bone with the specified name, or @c nullptr
		 * if no bone has that name.
		 */
		Bone *GetBone(const std::string &);

		/**
		 * Returns a pointer to the bone with the specified name, or @c nullptr
		 * if no bone has that name.
		 */
		const Bone *GetBone(const std::string &) const;

		/*--------+
		| signals |
		+--------*/

		util::copyable_signal<void ()> dirtyPoseSig;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * An ordered sequence of bones, where the parents appear before the
		 * children.
		 */
		std::vector<Bone> bones;

		/**
		 * An associative array mapping the name of a bone to its index in @c
		 * bones.
		 *
		 * @note Using an index instead of a pointer allows the default move
		 * constructor and assignment operator to be used.
		 */
		std::unordered_map<std::string, decltype(bones)::size_type> bonesByName;
	};
}}}

#endif
