#include "ChortlingHamsterModules.hpp"

struct Tie : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		VOLTAGE_INPUT,
		RECORD_GATE_INPUT,
		RESTART_PLAYBACK_INPUT,
		CLEAR_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		VOLTAGE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		RECORD_LIGHT,
		LIGHTS_LEN
	};

	// dsp doohickeys
	dsp::SchmittTrigger restartPlaybackTrigger;
	dsp::SchmittTrigger clearRecordingTrigger;
	dsp::SlewLimiter crossfadeSlewLimiter;

	// buffer stuff
	// 4 seconds at 48 KHz during testing.
	static const int bufferSize = 48000 * 4;
	float buffer[bufferSize];
	int bufferPosition = 0;

	// crossfade / slew limiter stuff
	// note: smaller rise and fall speeds means a longer slew
	// approximately eight milliseconds slew
	float crossfadeSlewLimiterRiseAndFallSpeed = 150.f;
	float crossfadeAmount = 0.f;

	// helpers
	float input = 0.f;
	float output = 0.f;
	// false = 0.f = low gate
	// true = 1.f = high gate
	bool recordingGateState = false;

	Tie() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		// configure the tooltip labels on the front panel
		configInput(VOLTAGE_INPUT, "Voltage");
		configInput(RECORD_GATE_INPUT, "Recording gate");
		configInput(RESTART_PLAYBACK_INPUT, "Restart playback trigger");
		configInput(CLEAR_INPUT, "Clear recording trigger");
		configOutput(VOLTAGE_OUTPUT, "Voltage");

		// configure the bypass route (only one)
		configBypass(VOLTAGE_INPUT, VOLTAGE_OUTPUT);

		// fill the recording buffer with zeros, just to be sure
		std::fill(std::begin(buffer), std::end(buffer), 0);

		// set the slew limiter rise and fall speed
		crossfadeSlewLimiter.setRiseFall(crossfadeSlewLimiterRiseAndFallSpeed, crossfadeSlewLimiterRiseAndFallSpeed);
	}

	void process(const ProcessArgs& args) override {
		// clear the recording buffer when a trigger arrives
		if (clearRecordingTrigger.process(inputs[CLEAR_INPUT].getVoltage())) {
			// immediately fill the recording buffer with zeros
			std::fill(std::begin(buffer), std::end(buffer), 0);
		}

		// restart the playback of the recording buffer when a trigger arrives (resets the buffer position to zero)
		if (restartPlaybackTrigger.process(inputs[RESTART_PLAYBACK_INPUT].getVoltage())) {
			bufferPosition = 0;
		}

		// get the input voltage
		input = inputs[VOLTAGE_INPUT].getVoltage();

		// set the recording gate state
		recordingGateState = inputs[RECORD_GATE_INPUT].getVoltage() > 0.f;

		// get the crossfade amount from the slew limiter based on the state of the recording gate
		crossfadeAmount = crossfadeSlewLimiter.process(args.sampleTime, recordingGateState);

		// set the recording light's brightness
		lights[RECORD_LIGHT].setBrightness(recordingGateState);

		// set the value of the current sample
		// note: output is redundant, but keeping in case needed later
		output = buffer[bufferPosition] + (input * crossfadeAmount);

		// set the output voltage
		outputs[VOLTAGE_OUTPUT].setVoltage(output);

		// add the current sample to the recording buffer at the current position
		buffer[bufferPosition] = output;

		// increment the buffer position
		bufferPosition++;
		// reset the buffer position to zero if needed
		if (bufferPosition >= bufferSize) {
			bufferPosition = 0;
		}
	}
};


struct TieWidget : ModuleWidget {
	TieWidget(Tie* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Tie.svg")));

		// screwy screws
		addChild(createWidget<ScrewBlack>(Vec(0, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// inputs
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 22.905)), module, Tie::VOLTAGE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 37.538)), module, Tie::RECORD_GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 70.53)), module, Tie::RESTART_PLAYBACK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 84.192)), module, Tie::CLEAR_INPUT));

		// outputs
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.08, 100.499)), module, Tie::VOLTAGE_OUTPUT));

		// shake it all about puts
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(5.08, 47.063)), module, Tie::RECORD_LIGHT));
	}
};


Model* modelTie = createModel<Tie, TieWidget>("Tie");
// the end