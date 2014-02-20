/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
