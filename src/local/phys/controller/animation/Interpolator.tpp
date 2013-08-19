#include <algorithm> // copy, lower_bound, max, min
#include <cassert>
#include <iterator> // back_inserter

#include "../../../math/interp.hpp" // {Cubic,Linear}Interp

namespace page
{
	namespace phys
	{
		// constructor
		template <typename T> Interpolator<T>::Interpolator(const res::Animation::Channel<T> &channel, InterpolationType type) :
			type(type)
		{
			// NOTE: animation frames expected to be sorted by increasing time
			frames.reserve(channel.frames.size());
			std::copy(channel.frames.begin(), channel.frames.end(), std::back_inserter(frames));
		}

		// interpolated access
		template <typename T> T Interpolator<T>::Get(float time) const
		{
			typename Frames::const_iterator frame(FindFloorFrame(time));
			assert(frame != frames.end());
			if (frame == frames.end() - 1) return frame->value;
			switch (type)
			{
				case stepInterpolation: return frame->value;
				case linearInterpolation:
				{
					const Frame
						&frame1(*frame),
						&frame2(*Advance(frame, 1));
					return math::LinearInterp(
						frame1.value, frame2.value,
						(time - frame1.time) / (frame2.time - frame1.time));
				}
				case cubicInterpolation:
				{
					const Frame
						&frame1(*Advance(frame, -1)),
						&frame2(*frame),
						&frame3(*Advance(frame, 1)),
						&frame4(*Advance(frame, 2));
					return math::CubicInterp(
						frame1.value, frame2.value, frame3.value, frame4.value,
						(time - frame2.time) / (frame3.time - frame2.time));
				}
				default: assert(!"invalid interpolation type");
			}
		}

		// validity
		template <typename T> Interpolator<T>::operator bool() const
		{
			return !frames.empty();
		}

		// frame
		// construct
		template <typename T> Interpolator<T>::Frame::Frame(float time, const T &value) :
			time(time), value(value) {}
		template <typename T> Interpolator<T>::Frame::Frame(const typename res::Animation::Channel<T>::Frame &frame) :
			time(frame.time), value(frame.value) {}

		// interpolator
		// bounded frame search
		template <typename T> typename Interpolator<T>::Frames::const_iterator Interpolator<T>::FindFloorFrame(float time) const
		{
			assert(!frames.empty());
			typename Frames::const_iterator frame(std::lower_bound(
				frames.begin(), frames.end(), Frame(time), CompareFrameTime));
			if (frame != frames.begin()) --frame;
			return frame;
		}

		// bounded frame iteration
		template <typename T> typename Interpolator<T>::Frames::const_iterator Interpolator<T>::Advance(const typename Interpolator<T>::Frames::const_iterator &iter, int n) const
		{
			assert(!frames.empty());
			// FIXME: for wrapping iterator during looping; must wrap time too
			/*int i = iter - frames.begin(), max = frames.size();
			n = ((i + n) % max + max) % max;*/
			n = std::min(std::max(n, frames.begin() - iter), (frames.end() - 1) - iter);
			return iter + n;
		}

		// frame comparison
		template <typename T> bool Interpolator<T>::CompareFrameTime(const Frame &a, const Frame &b)
		{
			return a.time < b.time;
		}
	}
}
