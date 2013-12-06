#include "Position.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Position::Position(const math::Vec3 &value) :
		value(value) {}

	Position::Position(const math::Mat34 &matrix) :
		value(GetTranslation(matrix)) {}

	/*----------+
	| accessors |
	+----------*/

	const math::Vec3 &Position::GetPosition() const
	{
		return value;
	}

	void Position::SetPosition(const math::Vec3 &value)
	{
		// NOTE: comparing value to avoid redundant dirty marking
		if (Any(value != this->value))
		{
			this->value = value;
			dirtyTransformSig();
		}
	}

	math::Mat34 Position::GetMatrix() const
	{
		return TranslationMatrix(value);
	}

	math::Mat34 Position::GetInvMatrix() const
	{
		return TranslationMatrix(-value);
	}

	void Position::SetMatrix(const math::Mat34 &matrix)
	{
		SetPosition(GetTranslation(matrix));
	}

	const math::Vec3 &Position::GetLastPosition() const
	{
		return lastValue;
	}

	const math::Vec3 &Position::GetForce() const
	{
		return force;
	}

	const math::Vec3 &Position::GetVelocity() const
	{
		return velocity;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Position::GetFrame() const
	{
		Frame frame;
		frame.position = value;
		return frame;
	}

	void Position::SetFrame(const Frame &frame)
	{
		if (frame.position)
			SetPosition(*frame.position);
	}

	/*-----------------------------+
	| Transformable implementation |
	+-----------------------------*/

	void Position::BakeTransform()
	{
		lastValue = value;
	}

	void Position::UpdateForce()
	{
		force = value - lastValue;
	}

	void Position::UpdateDelta()
	{
		velocity = value - lastValue;
	}
}}}
