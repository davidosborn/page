#ifndef    page_local_phys_attrib_Pose_hpp
#   define page_local_phys_attrib_Pose_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include <boost/optional.hpp>

#	include "../../math/Matrix.hpp"
#	include "../../res/type/Skeleton.hpp" // Skeleton::Bone
#	include "../../util/copyable_signal.hpp"
#	include "../../util/Identifiable.hpp"
#	include "PositionOrientationScale.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The skeletal deformation part of a node.
	 *
	 * The bones are designed to be copyable and moveable, and are stored in a
	 * std::vector to support cache locality.  Bones are referenced by their
	 * index in the vector, rather than by their memory address, to simplify
	 * storage issues.
	 *
	 * @note Using std::list to store the bones would destroy cache locality.
	 * Using pointers for bone references would make it hard to copy/move the
	 * bones as required by std::vector and for basic tasks (it has been tried
	 * before).
	 *
	 * @todo res::Skeleton could be modified to also represent parent-bone
	 * references by indices rather than by pointers, for simplicity and to
	 * match up closer to this implementation.
	 */
	class Pose :
		public PositionOrientationScale,
		public util::Identifiable
	{
////////// Pose::Bone //////////////////////////////////////////////////////////

		public:
		/**
		 * An individual bone, as part of a skeletal deformation.
		 */
		class Bone :
			public PositionOrientationScale
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			/**
			 * Creates a default bone.
			 *
			 * @note This constructor does not initialize the bone's bind pose.
			 * The caller should do this with SetBindPose() before using it.
			 */
			explicit Bone(Pose &, const std::string &name);

			/**
			 * Creates a bone from a skeleton-bone resource.  If @a parent
			 * points to a bone, this bone will be attached to it as a child.
			 *
			 * @note This constructor does not initialize the bone's bind pose.
			 * The caller should do this with SetBindPose() before using it.
			 */
			Bone(Pose &, const res::Skeleton::Bone &, Bone *parent = nullptr);

			Bone(const Bone &);
			Bone(Bone &&);

			/**
			 * Provides both copy assignment and move assignment using the "Copy
			 * and Swap" idiom.
			 */
			Bone &operator =(Bone);

			private:
			void Init();

			/*----------+
			| observers |
			+----------*/

			public:
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
			const std::string &GetName() const;

			/**
			 * Returns the bone's index within the pose.
			 */
			unsigned GetIndex() const;

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
			 * Attaches the bone to a parent bone.
			 */
			void SetParent(Bone &);

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
			const math::Vec3 &GetBindPosition() const;

			/**
			 * Returns the orientation of the bone in its bind pose.
			 */
			const math::Quat<> &GetBindOrientation() const;

			/**
			 * Returns the scale of the bone in its bind pose.
			 */
			const math::Vec3 &GetBindScale() const;

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
			 * The index of the bone's parent in the associated pose.
			 */
			boost::optional<unsigned> parentIndex;

			/**
			 * The indices of the bone's children in the associated pose.
			 */
			std::vector<unsigned> childIndices;

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

////////// Pose ////////////////////////////////////////////////////////////////

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Pose() = default;
		explicit Pose(const res::Skeleton &);

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
		 * Adds a bone and its parents from a skeleton resource if they haven't
		 * already been added.  Returns the index of the specified bone in the
		 * pose.
		 */
		Bone &CopyTreePath(const res::Skeleton::Bone &);

		/*------+
		| bones |
		+------*/

		public:
		/**
		 * Returns an ordered sequence of bones.
		 *
		 * @todo Use @c decltype(auto) in C++14.
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
		 * An associative array mapping the name of a bone to its index.
		 */
		std::unordered_map<std::string, unsigned> bonesByName;
	};
}}}

#endif
