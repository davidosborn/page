// key frame interpolator
// inspired by CatMother's anim::Interpolator

#ifndef    page_local_phys_controller_animation_Interpolator_hpp
#   define page_local_phys_controller_animation_Interpolator_hpp

#	include "../../../res/type/Animation.hpp" // Animation::Channel

namespace page
{
	namespace phys
	{
		template <typename T> struct Interpolator
		{
			enum InterpolationType
			{
				stepInterpolation,
				linearInterpolation,
				cubicInterpolation
			};

			// constructor
			// TEST: cubic interpolation should be the default
			Interpolator(const res::Animation::Channel<T> &, InterpolationType = linearInterpolation);

			// interpolated access
			T Get(float time) const;

			// validity
			explicit operator bool() const;

			private:
			// frame
			struct Frame
			{
				// construct
				explicit Frame(float time, const T & = T());
				Frame(const typename res::Animation::Channel<T>::Frame &);

				float time;
				T value;
			};
			typedef std::vector<Frame> Frames;

			// bounded frame search
			typename Frames::const_iterator FindFloorFrame(float time) const;

			// bounded frame iteration
			typename Frames::const_iterator Advance(const typename Frames::const_iterator &, int) const;

			// frame comparison
			static bool CompareFrameTime(const Frame &, const Frame &);

			Frames frames;
			InterpolationType type;
		};
	}
}

#	include "Interpolator.tpp"
#endif
