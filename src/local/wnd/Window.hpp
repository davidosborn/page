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

#ifndef    page_local_wnd_Window_hpp
#   define page_local_wnd_Window_hpp

#	include <memory> // unique_ptr

#	include <boost/signal.hpp>

#	include "../math/Vector.hpp"
#	include "../util/class/Cloneable.hpp"

namespace page
{
	namespace aud { class Driver; }
	namespace inp { class Driver; }
	namespace vid { class Driver; }
}

namespace page { namespace wnd
{
	class Window : public util::Cloneable<Window>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Window();

		/*--------------------+
		| copy/move semantics |
		+--------------------*/

		Window(const Window &);
		Window &operator =(const Window &);

		/*---------------+
		| initialization |
		+---------------*/

		protected:
		/**
		 * Initializes the window's state.
		 *
		 * @note This function must be called at the end of the derived class'
		 * constructor.
		 */
		void InitState(
			bool               focus,
			math::Vec2i const& position,
			math::Vec2u const& size);

		/**
		 * Destroys the drivers that have been created.
		 *
		 * @note This function must be called at the beginning of the derived
		 * class' destructor.
		 */
		void ResetDrivers();

		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * Returns @c true if the window has the keyboard focus.
		 */
		bool HasFocus() const;

		/**
		 * Returns the position of the window on the screen, in pixels.
		 */
		const math::Vec2i &GetPosition() const;

		/**
		 * Returns the size of the window, in pixels.
		 */
		const math::Vec2u &GetSize() const;

		/**
		 * Returns the size of the screen, in pixels.
		 */
		virtual math::Vec2u GetScreenSize() const = 0;

		/*-------+
		| update |
		+-------*/

		/**
		 * Synchronizes the window with the operating system, process queued
		 * events and any other required maintenance.
		 *
		 * @note This function is called once per frame in the main loop, for
		 * each existing @c Window instance.
		 */
		virtual void Update() = 0;

		/*--------------+
		| driver access |
		+--------------*/

		/**
		 * Returns the audio driver associated with the window, creating it if
		 * it hasn't already been created.
		 */
		aud::Driver &GetAudioDriver();

		/**
		 * Returns the input driver associated with the window, creating it if
		 * it hasn't already been created.
		 */
		inp::Driver &GetInputDriver();

		/**
		 * Returns the video driver associated with the window, creating it if
		 * it hasn't already been created.
		 */
		vid::Driver &GetVideoDriver();

		protected:
		/**
		 * Returns @c true if the audio driver has been created.
		 */
		bool HasAudioDriver() const;

		/**
		 * Returns @c true if the input driver has been created.
		 */
		bool HasInputDriver() const;

		/**
		 * Returns @c true if the video driver has been created.
		 */
		bool HasVideoDriver() const;

		private:
		/**
		 * Creates the audio driver for the window.
		 *
		 * The default implementation requests the best driver from the global
		 * audio-driver registry.
		 *
		 * @note The implementation of this function can assume that there will
		 * not be any existing audio drivers associated with the window when
		 * this function is called.
		 */
		virtual std::unique_ptr<aud::Driver> MakeAudioDriver();

		/**
		 * Creates the input driver for the window.
		 *
		 * The default implementation requests the best driver from the global
		 * input-driver registry.
		 *
		 * @note The implementation of this function can assume that there will
		 * not be any existing input drivers associated with the window when
		 * this function is called.
		 */
		virtual std::unique_ptr<inp::Driver> MakeInputDriver();

		/**
		 * Creates the video driver for the window.
		 *
		 * The default implementation requests the best driver from the global
		 * video-driver registry.
		 *
		 * @note The implementation of this function can assume that there will
		 * not be any existing video drivers associated with the window when
		 * this function is called.
		 */
		virtual std::unique_ptr<vid::Driver> MakeVideoDriver();

		/*--------+
		| signals |
		+--------*/

		public:
		/**
		 * Called when the users attempts to close the window.
		 */
		boost::signal<void ()> exitSig;

		/**
		 * Called when the window loses or gains the keyboard focus.
		 */
		boost::signal<void (bool)> focusSig;

		/**
		 * Called when the window's position changes.
		 */
		boost::signal<void (const math::Vec2i &)> moveSig;

		/**
		 * Called when the window's size changes.
		 */
		boost::signal<void (const math::Vec2u &)> sizeSig;

		/*----------------+
		| signal handlers |
		+----------------*/

		private:
		void OnExit();
		void OnFocus(bool);
		void OnMove(const math::Vec2i &);
		void OnSize(const math::Vec2u &);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The audio driver associated with this window.
		 */
		std::unique_ptr<aud::Driver> audioDriver;

		/**
		 * The input driver associated with this window.
		 */
		std::unique_ptr<inp::Driver> inputDriver;

		/**
		 * The video driver associated with this window.
		 */
		std::unique_ptr<vid::Driver> videoDriver;

		/**
		 * @c true if the window has the keyboard focus.
		 */
		bool focus;

		/**
		 * The position of the window on the screen, in pixels.
		 */
		math::Vec2i position;

		/**
		 * The size of the window, in pixels.
		 */
		math::Vec2u size;
	};
}}

#endif
