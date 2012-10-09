/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_env_Window_hpp
#   define page_env_Window_hpp

#	include <memory> // unique_ptr
#	include <string>
#	include "../math/Vector.hpp"
#	include "../util/NonCopyable.hpp"
#	include "../util/Signal.hpp"

namespace page
{
	namespace aud { class Driver; }
	namespace inp { class Driver; }
	namespace vid { class Driver; }

	namespace env
	{
		struct Window : util::NonCopyable
		{
			// construct/destroy
			Window();
			virtual ~Window();

			// state
			bool HasFocus() const;
			const math::Vector<2, int> &GetPosition() const;
			const math::Vector<2, unsigned> &GetSize() const;

			// update
			virtual void Update() = 0;

			// driver access
			aud::Driver &GetAudioDriver();
			inp::Driver &GetInputDriver();
			vid::Driver &GetVideoDriver();

			// signals
			util::Signal<void ()> exitSig;
			util::Signal<void (bool)> focusSig;
			util::Signal<void (const math::Vector<2, int> &)> moveSig;
			util::Signal<void (const math::Vector<2, unsigned> &)> sizeSig;

			// environment state
			virtual math::Vector<2, unsigned> GetScreenSize() const = 0;

			protected:
			// deferred state initialization
			// must be called at end of derived constructor
			void InitState(bool focus,
				const math::Vector<2, int> &position,
				const math::Vector<2, unsigned> &size);

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
			void OnMove(const math::Vector<2, int> &);
			void OnSize(const math::Vector<2, unsigned> &);

			// drivers
			std::unique_ptr<aud::Driver> audioDriver;
			std::unique_ptr<inp::Driver> inputDriver;
			std::unique_ptr<vid::Driver> videoDriver;

			bool focus;
			math::Vector<2, int> pos;
			math::Vector<2, unsigned> size;
		};

		// factory function
		Window *MakeWindow(const std::string &title);
	}
}

#endif
