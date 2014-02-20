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

#ifndef    page_local_phys_node_Sound_hpp
#   define page_local_phys_node_Sound_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "../../util/Identifiable.hpp"
#	include "../attrib/Position.hpp"
#	include "../attrib/Volume.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace phys
{
	class Sound :
		public Node,
		public Controllable,
		public attrib::Position,
		public attrib::Volume,
		public util::Identifiable
	{
		IMPLEMENT_CLONEABLE(Sound, Node)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Sound(const cache::Proxy<res::Sound> &, bool loop = false);

		/*----------+
		| accessors |
		+----------*/

		/**
		 * Returns the sound that this node is playing.
		 */
		const cache::Proxy<res::Sound> &GetSound() const;

		/**
		 * Returns true if the sound is currently playing.
		 */
		bool IsPlaying() const;

		/**
		 * Returns true if the sound is set to loop automatically.
		 */
		bool IsLooping() const;

		/**
		 * Returns the amount of the sound that has been played, measured in
		 * seconds.
		 */
		float GetPlayPosition() const;

		/*-------+
		| update |
		+-------*/

		void Update(float deltaTime);

		/*--------------------+
		| Node implementation |
		+--------------------*/

		NodeType GetType() const override;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The sound that this node is playing.
		 */
		cache::Proxy<res::Sound> sound;

		/**
		 * True if the sound is currently playing.
		 */
		bool playing = true;

		/**
		 * True if the sound is set to loop automatically.
		 */
		bool loop = false;

		/**
		 * The amount of the sound that has been played, measured in seconds.
		 */
		float playPosition = 0;
	};
}}

#endif
