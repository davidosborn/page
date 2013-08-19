#ifndef    page_local_aud_Channel_hpp
#   define page_local_aud_Channel_hpp

namespace page { namespace aud
{
	struct Channel
	{
		// construct/destroy
		// NOTE: use fade when emerging from occlusion
		explicit Channel(bool fade);
		virtual ~Channel();

		// update
		void Update(float deltaTime);

		protected:
		// state
		float GetVolume() const;
		float GetLinearVolume() const;

		private:
		// state
		virtual float DoGetLinearVolume() const = 0;

		// update
		virtual void DoUpdate(float deltaTime) = 0;
		virtual void DoUpdate2() = 0;

		bool playing;
		float level;

		protected:
		static const float fadeExponent;
	};
}}

#endif
