#include <algorithm> // max
#include <cassert>
#include <unordered_map>

#include "../../util/lang.hpp" // GetPhonemes
#include "../attrib/Pose.hpp"
#include "LipsyncController.hpp"

namespace page { namespace phys
{
	namespace
	{
		// phoneme keyframes
		typedef std::vector<Frame> Keyframes;
		typedef std::unordered_map<std::string, Keyframes> PhonemeKeyframes;
		PhonemeKeyframes MakePhonemeKeyframes()
		{
			PhonemeKeyframes phonemeKeyframes;
			// NOTE: see util/lang.hpp for phoneme descriptions
			// AA
			{
				Frame frame;
				// FIXME: implement
				phonemeKeyframes.insert(std::make_pair("AA", Keyframes(1, frame)));
			}
			// AE
			{
				// FIXME: implement
			}
			// AH
			{
				// FIXME: implement
			}
			// AO
			{
				// FIXME: implement
			}
			// AW
			{
				// FIXME: implement
			}
			// AY
			{
				// FIXME: implement
			}
			// B
			{
				// FIXME: implement
			}
			// CH
			{
				// FIXME: implement
			}
			// D
			{
				// FIXME: implement
			}
			// DH
			{
				// FIXME: implement
			}
			// EH
			{
				// FIXME: implement
			}
			// ER
			{
				// FIXME: implement
			}
			// EY
			{
				// FIXME: implement
			}
			// F
			{
				// FIXME: implement
			}
			// G
			{
				// FIXME: implement
			}
			// HH
			{
				// FIXME: implement
			}
			// IH
			{
				// FIXME: implement
			}
			// IY
			{
				// FIXME: implement
			}
			// JH
			{
				// FIXME: implement
			}
			// K
			{
				// FIXME: implement
			}
			// L
			{
				// FIXME: implement
			}
			// M
			{
				// FIXME: implement
			}
			// N
			{
				// FIXME: implement
			}
			// NG
			{
				// FIXME: implement
			}
			// OW
			{
				// FIXME: implement
			}
			// OY
			{
				// FIXME: implement
			}
			// P
			{
				// FIXME: implement
			}
			// R
			{
				// FIXME: implement
			}
			// S
			{
				// FIXME: implement
			}
			// SH
			{
				// FIXME: implement
			}
			// T
			{
				// FIXME: implement
			}
			// TH
			{
				// FIXME: implement
			}
			// UH
			{
				// FIXME: implement
			}
			// UW
			{
				// FIXME: implement
			}
			// V
			{
				// FIXME: implement
			}
			// W
			{
				// FIXME: implement
			}
			// Y
			{
				// FIXME: implement
			}
			// Z
			{
				// FIXME: implement
			}
			// ZH
			{
				// FIXME: implement
			}
			return phonemeKeyframes;
		}
		const PhonemeKeyframes &GetPhonemeKeyframes()
		{
			static PhonemeKeyframes phonemeKeyframes(MakePhonemeKeyframes());
			return phonemeKeyframes;
		}
	}

	// construct
	LipsyncController::LipsyncController(const attrib::Pose &pose) :
		TargetController(AnimationLayer::preCollision, .25)
	{
		assert(Check(pose));
	}

	// modifiers
	void LipsyncController::PushText(const std::string &text)
	{
		phonemes += util::GetPhonemes(text);
	}
	void LipsyncController::PushPhonemes(const std::string &phonemes)
	{
		this->phonemes += phonemes;
	}

	// check compatibility
	bool LipsyncController::Check(const attrib::Pose &pose)
	{
		return pose.GetBone("jaw");
	}

	// update targets
	void LipsyncController::UpdateTargets(float deltaTime)
	{
		if (phonemeDuration)
		{
			phonemeDuration = std::max(phonemeDuration - deltaTime, 0.f);
			if (!phonemeDuration && !phonemes.empty())
			{
				// FIXME: set targets and duration for new phoneme
				// FIXME: remove phoneme from list
			}
		}
	}
}}
