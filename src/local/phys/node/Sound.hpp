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
