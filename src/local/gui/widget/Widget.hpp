#ifndef    page_local_gui_widget_Widget_hpp
#   define page_local_gui_widget_Widget_hpp

#	include "../../math/Vector.hpp"
#	include "../../util/class/Cloneable.hpp"
#	include "../../util/copyable_signal.hpp"
#	include "../../inp/Key.hpp"

namespace page { namespace res { class Theme; }}

namespace page { namespace gui
{
	class DrawContext;

	/**
	 * The base class for user-interface widgets.
	 */
	class Widget : public util::Cloneable<Widget>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Widget(
			math::Vec2 const& position = 0.5,
			math::Vec2 const& minSize  = 0,
			bool              visible  = true);

		/*------------+
		| positioning |
		+------------*/

		/**
		 * Returns the widget's position.
		 */
		const math::Vec2 &GetPosition() const;

		/**
		 * Returns the widget's minimum size.
		 */
		const math::Vec2 &GetMinSize() const;

		/**
		 * Returns the widget's size constraints.
		 */
		WidgetSizeConstraints GetSizeConstraints(const res::Theme &) const;

		/**
		 * Sets the widget's position.
		 */
		void SetPosition(const math::Vec2 &);

		/**
		 * Sets the widget's minimum size.
		 */
		void SetMinSize(const math::Vec2 &);

		/*-----------+
		| visibility |
		+-----------*/

		/**
		 * @return @c true if the widget is visible.
		 */
		bool IsVisible() const;

		/**
		 * Shows the widget, making it visible.
		 */
		void Show(bool fade = true);

		/**
		 * Hides the widget, making it invisible.
		 */
		void Hide(bool fade = true);

		/**
		 * Sets the widget's visibility.
		 */
		void SetVisible(bool, bool fade = true);

		/**
		 * @return The widget's opacity.
		 */
		bool GetOpacity() const;

		/**
		 * Sets the widget's opacity.
		 */
		void SetOpacity(float);

		/*----------------+
		| special effects |
		+----------------*/

		/**
		 * @return @c true if the widget is glowing.
		 */
		bool IsGlowing() const;

		/**
		 * Sets the widget's glow effect.
		 */
		void SetGlow(bool, bool fade = true);

		/**
		 * @return The widget's glow intensity.
		 */
		float GetGlowIntensity() const;

		/**
		 * Sets the widget's glow intensity.
		 */
		void SetGlowIntensity(float);

		/*---------------+
		| implementation |
		+---------------*/

		/**
		 * Draws the widget.
		 */
		void Draw(DrawContext &) const;

		/**
		 * Updates the widget.
		 *
		 * @param deltaTime The amount of time that has passed since the last
		 *        time this function was called, measured in seconds.
		 */
		void Update(float deltaTime);

		/*--------+
		| signals |
		+--------*/

		/**
		 * A signal that is called when the user enters a character while the
		 * widget has the keyboard focus.
		 *
		 * @param[in] _1 The character that was entered.
		 */
		util::copyable_signal<void (char)> onChar;

		/**
		 * A signal that is called when the user clicks on the widget.
		 *
		 * @param[in] _1 The mouse button that was clicked.
		 */
		util::copyable_signal<void (unsigned)> onClick;

		/**
		 * A signal that is called when the cursor moves into the space of the
		 * widget.
		 */
		util::copyable_signal<void ()> onCursorEnter;

		/**
		 * A signal that is called when the cursor moves out of the space of the
		 * widget.
		 */
		util::copyable_signal<void ()> onCursorLeave;

		/**
		 * A signal that is called when the user presses a key while the widget
		 * has the keyboard focus.
		 *
		 * @param[in] _1 The key that was pressed.
		 */
		util::copyable_signal<void (inp::Key)> onKey;

		/*-----------------------+
		| virtual implementation |
		+-----------------------*/

		private:
		virtual WidgetSizeConstraints GetSizeConstraints(const res::Theme &) const = 0;
		virtual void DoDraw(DrawContext &) const = 0;
		virtual void DoUpdate(float deltaTime);

		/*---------------+
		| event handling |
		+---------------*/

		virtual void OnClick();

		/*----------+
		| constants |
		+----------*/

		protected:
		/**
		 * The duration of the fade-in transition for glow effects.
		 */
		static const float glowFadeInDuration;

		/**
		 * The duration of the fade-out transition for glow effects.
		 */
		static const float glowFadeOutDuration;

		/**
		 * The exponent of the fade transition for glow effects.
		 */
		static const float glowFadeExponent;

		/**
		 * The duration of the fade-in transition for visibility.
		 */
		static const float visibilityFadeInDuration;

		/**
		 * The duration of the fade-out transition for visibility.
		 */
		static const float visibilityFadeOutDuration;

		/**
		 * The exponent of the fade transition for visibility.
		 */
		static const float visibilityFadeExponent;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The coordinates of the top-left corner of the widget.
		 */
		math::Vec2 position;

		/**
		 * The minimum size of the widget.
		 */
		math::Vec2 minSize;

		/**
		 * @defgroup widget-visible
		 * The data members that control the widget's visibility.
		 * @{
		 */
		/**
		 * @c true if the widget is currently visible, not counting its
		 * transition state or opacity.
		 */
		bool visible;

		/**
		 * The widget's visibility, which is the current state of its transition
		 * between being invisible and being fully visible, in the range
		 * @f$[0,1]@f$.
		 */
		float visibility = visible;

		/**
		 * The widget's opacity, when fully visible.
		 */
		float opacity = 1.0;
		///@}

		/**
		 * @defgroup widget-glow
		 * The data members that control the widget's glow effect.
		 * @{
		 */
		/**
		 * @c true if the widget is currently glowing, not counting its
		 * transition state or glow intensity.
		 */
		bool glow = false;

		/**
		 * The widget's transition state between not glowing and fully glowing,
		 * in the range @f$[0,1]@f$.
		 */
		float glowTransitionState = glow;

		/**
		 * The intensity of the widget's glow effect.
		 */
		float glowIntensity = 1.0;
		///@}
	};
}}

#endif
