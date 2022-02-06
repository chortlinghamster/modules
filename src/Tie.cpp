#include "ChortlingHamsterModules.hpp"

struct Tie : Module {
	enum ParamId {
		MONITOR_PUSH_BUTTON_PARAM,
		GATE_OR_LATCH_PARAM,
		LOOP_OR_ONCE_PARAM,
		CROSSFADE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VOLTAGE_IN_INPUT,
		RECORDING_GATE_INPUT,
		RESTART_TRIGGER_INPUT,
		CLEAR_TRIGGER_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		VOLTAGE_OUT_OUTPUT,
		END_OF_CYCLE_TRIGGER_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		MONITOR_LIGHT,
		RECORDING_INDICATOR_LIGHT,
		END_OF_CYCLE_INDICATOR_LIGHT,
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
	// approximately eight milliseconds total slew at 48 KHz
	float crossfadeSlewLimiterRiseAndFallSpeed = 150.f;
	float crossfadeAmount = 0.f;

	// helpers
	float input = 0.f;
	float output = 0.f;
	// for recordingGateState, false = 0.f = low gate, true = 1.f = high gate
	bool recordingGateState = false;
	float gateOrLatch = 0.f;
	float loopOrOnce = 0.f;

	Tie() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		// configure the crossfade length param
		// todo!
		configButton(MONITOR_PUSH_BUTTON_PARAM, "Monitor push");
		configSwitch(GATE_OR_LATCH_PARAM, 0.0, 1.0, 1.0, "Gate or Latch", {"Gate", "Latch"});
		configSwitch(LOOP_OR_ONCE_PARAM, 0.0, 1.0, 1.0, "Loop or Once", {"Loop", "Once"});
		configParam(CROSSFADE_PARAM, 0.f, 1.f, 0.f, "Crossfade anti-click length");


		// configure the tooltip labels on the front panel
		configInput(VOLTAGE_IN_INPUT, "Voltage");
		configInput(RECORDING_GATE_INPUT, "Recording gate");
		configInput(RESTART_TRIGGER_INPUT, "Restart playback trigger");
		configInput(CLEAR_TRIGGER_INPUT, "Clear recording trigger");
		configOutput(VOLTAGE_OUT_OUTPUT, "Voltage");

		// configure the bypass route (only one)
		configBypass(VOLTAGE_IN_INPUT, VOLTAGE_OUT_OUTPUT);

		// fill the recording buffer with zeros, just to be sure
		std::fill(std::begin(buffer), std::end(buffer), 0);

		// set the slew limiter rise and fall speed
		crossfadeSlewLimiter.setRiseFall(crossfadeSlewLimiterRiseAndFallSpeed, crossfadeSlewLimiterRiseAndFallSpeed);
	}

	void process(const ProcessArgs& args) override {
		// clear the recording buffer when a trigger arrives
		if (clearRecordingTrigger.process(inputs[CLEAR_TRIGGER_INPUT].getVoltage())) {
			// immediately fill the recording buffer with zeros
			std::fill(std::begin(buffer), std::end(buffer), 0);
		}

		// restart the playback of the recording buffer when a trigger arrives (resets the buffer position to zero)
		if (restartPlaybackTrigger.process(inputs[RESTART_TRIGGER_INPUT].getVoltage())) {
			bufferPosition = 0;
		}

		// get the input voltage
		input = inputs[VOLTAGE_IN_INPUT].getVoltage();

		// get the state of the gate or latch switch
		gateOrLatch = params[GATE_OR_LATCH_PARAM].getValue();
		// set the recording gate state
		// if gateOrLatch is set to latch, the gate is always open, otherwise, go off the gate input
		recordingGateState = (gateOrLatch) ? inputs[RECORDING_GATE_INPUT].getVoltage() > 0.f : true;

		// get the crossfade amount from the slew limiter based on the state of the recording gate
		crossfadeAmount = crossfadeSlewLimiter.process(args.sampleTime, recordingGateState);

		// set the recording light's brightness
		lights[RECORDING_INDICATOR_LIGHT].setBrightness(recordingGateState);

		// set the value of the current sample
		// note: output is redundant, but keeping in case needed later
		output = buffer[bufferPosition] + (input * crossfadeAmount);

		// set the output voltage
		outputs[VOLTAGE_OUT_OUTPUT].setVoltage(output);

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

		// params
		// monitor toggle button
		// addChild(createWidgetCentered<Widget>(mm2px(Vec(22.86, 20.32))));
		addParam(createLightParamCentered<VCVLightBezel<>>(mm2px(Vec(22.86, 20.32)), module, Tie::MONITOR_PUSH_BUTTON_PARAM, Tie::MONITOR_LIGHT));
		// gate or latch switch
		addParam(createParamCentered<CKSS>(mm2px(Vec(22.86, 40.64)), module, Tie::GATE_OR_LATCH_PARAM));
		// loop or once switch
		addParam(createParamCentered<CKSS>(mm2px(Vec(22.86, 63.504)), module, Tie::LOOP_OR_ONCE_PARAM));
		// crossfade param
		addParam(createParamCentered<CHMRoundMediumKnob>(mm2px(Vec(22.86, 84.192)), module, Tie::CROSSFADE_PARAM));

		// inputs
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 20.32)), module, Tie::VOLTAGE_IN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 40.64)), module, Tie::RECORDING_GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 63.504)), module, Tie::RESTART_TRIGGER_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 84.192)), module, Tie::CLEAR_TRIGGER_INPUT));

		// outputs
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 100.499)), module, Tie::VOLTAGE_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.86, 100.499)), module, Tie::END_OF_CYCLE_TRIGGER_OUTPUT));

		// lights
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(7.62, 47.566)), module, Tie::RECORDING_INDICATOR_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(22.86, 107.425)), module, Tie::END_OF_CYCLE_INDICATOR_LIGHT));

	}
};


Model* modelTie = createModel<Tie, TieWidget>("Tie");
// the end