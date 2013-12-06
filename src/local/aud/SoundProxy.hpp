#ifndef    page_local_aud_SoundProxy_hpp
#   define page_local_aud_SoundProxy_hpp

#	include <memory> // shared_ptr

namespace page { namespace aud
{
	class Sound;

	class SoundProxy
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		SoundProxy();
		explicit SoundProxy(const std::shared_ptr<Sound> &);

		/*----------+
		| observers |
		+----------*/

		bool IsPlaying() const;
		float GetVolume() const;

		explicit operator bool() const;

		bool operator ==(const SoundProxy &) const;
		bool operator !=(const SoundProxy &) const;

		/*----------+
		| modifiers |
		+----------*/

		void Reset();
		void Reset(const std::shared_ptr<Sound> &);
		void Stop();
		void Pause();
		void Resume();
		void SetVolume(float);

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::shared_ptr<Sound> sound;
	};
}}

#endif
