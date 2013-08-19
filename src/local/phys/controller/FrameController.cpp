#include "FrameController.hpp"

namespace page { namespace phys
{
	// construct
	FrameController::FrameController(const Frame &frame, Layer layer) :
		Controller(layer), frame(frame) {}

	// generate frame
	Frame FrameController::DoGetFrame(const Frame &, const Frame &) const
	{
		return frame;
	}
}}
