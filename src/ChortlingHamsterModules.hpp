#include <rack.hpp>

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin *pluginInstance;

// Declare each Model, defined in each module source file
extern Model *modelPop;
extern Model *modelBitwise;

// Custom large knob.
struct CHMRoundLargeKnob : RoundKnob {
	CHMRoundLargeKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CHMRoundLargeKnob.svg")));
	}
};

// Custom large snappable knob.
struct CHMRoundLargeSnapKnob : CHMRoundLargeKnob {
	CHMRoundLargeSnapKnob() {
		snap = true;
	}
};

// Custom small knob.
struct CHMRoundSmallKnob : RoundKnob {
	CHMRoundSmallKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CHMRoundSmallKnob.svg")));
	}
};
