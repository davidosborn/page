#ifndef    page_local_phys_mixin_Trackable_hpp
#   define page_local_phys_mixin_Trackable_hpp

#	include "../../math/Vector.hpp"
#	include "../../res/type/Track.hpp" // Track::Face
#	include "Positionable.hpp"
#	include "Transformable.hpp"

namespace page { namespace phys
{
	/**
	 * A mixin which makes the derived node trackable.  Trackable nodes follow
	 * the defined track.
	 */
	class Trackable :
		public virtual Positionable,
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Trackable();
		explicit Trackable(const res::Track &);

		/*--------------------+
		| track binding state |
		+--------------------*/

		bool HasTrack() const;
		bool HasTrackFace() const;
		const res::Track &GetTrack() const;
		const res::Track::Face &GetTrackFace() const;
		unsigned GetTrackFaceIndex() const;

		/*------------------------+
		| track binding modifiers |
		+------------------------*/

		void SetTrack();
		void SetTrack(const res::Track &);
		void SetTrack(const res::Track *);
		void SetTrackFace();
		void SetTrackFace(const res::Track::Face &);
		void SetTrackFace(const res::Track::Face *);
		bool BindTrackFace();

		/*-------------------+
		| position modifiers |
		+-------------------*/

		using Positionable::SetPosition;
		void SetPosition(const math::Vec2 &);
		void SetTracklessPosition(const math::Vec2 &);

		/*------------------------+
		| deferred initialization |
		+------------------------*/

		protected:
		/**
		 * @note This function must be called at the end of the derived
		 * constructor.
		 */
		void Init();

		/*-------------+
		| data members |
		+-------------*/

		private:
		const res::Track *track;
		const res::Track::Face *trackFace;
	};
}}

#endif
