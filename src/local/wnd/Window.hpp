#ifndef    page_local_wnd_Window_hpp
#   define page_local_wnd_Window_hpp

#	include <memory> // unique_ptr
#	include <string>

#	include "../math/Vector.hpp"
#	include "../util/class/Cloneable.hpp"
#	include "../util/copyable_signal.hpp"

namespace page
{
	namespace aud { class Driver; }
	namespace inp { class Driver; }
	namespace vid { class Driver; }
}

namespace page { namespace wnd
{
	class Window : public virtual util::Cloneable<Window>
	{
		public:
		// construct/destroy
		Window();
		virtual ~Window();

		// state
		bool HasFocus() const;
		const math::Vec2i &GetPosition() const;
		const math::Vec2u &GetSize() const;

		// update
		virtual void Update() = 0;

		// driver access
		aud::Driver &GetAudioDriver();
		inp::Driver &GetInputDriver();
		vid::Driver &GetVideoDriver();

		// signals
		util::copyable_signal<void ()> exitSig;
		util::copyable_signal<void (bool)> focusSig;
		util::copyable_signal<void (const math::Vec2i &)> moveSig;
		util::copyable_signal<void (const math::Vec2u &)> sizeSig;

		// environment state
		virtual math::Vec2u GetScreenSize() const = 0;

		protected:
		// deferred state initialization
		// must be called at end of derived constructor
		void InitState(bool focus,
			const math::Vec2i &position,
			const math::Vec2u &size);

		// preemptive destruction
		// must be called at beginning of derived destructor
		void Deinit();

		// driver state
		bool HasAudioDriver() const;
		bool HasInputDriver() const;
		bool HasVideoDriver() const;

		private:
		// driver factory functions
		virtual aud::Driver *MakeAudioDriver() = 0;
		virtual inp::Driver *MakeInputDriver() = 0;
		virtual vid::Driver *MakeVideoDriver() = 0;

		// signal handlers
		void OnExit();
		void OnFocus(bool);
		void OnMove(const math::Vec2i &);
		void OnSize(const math::Vec2u &);

		// drivers
		std::unique_ptr<aud::Driver> audioDriver;
		std::unique_ptr<inp::Driver> inputDriver;
		std::unique_ptr<vid::Driver> videoDriver;

		bool focus;
		math::Vec2i pos;
		math::Vec2u size;
	};
}}

#endif
