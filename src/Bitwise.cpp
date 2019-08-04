#include "ChortlingHamsterModules.hpp"


struct Bitwise : Module {
	enum ParamIds {
		PATTERN_SELECT_CV_ATN_PARAM,
		PATTERN_SELECT_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT1_INPUT,
		INPUT2_INPUT,
		INPUT3_INPUT,
		INPUT4_INPUT,
		TRIGGER1_INPUT,
		TRIGGER2_INPUT,
		TRIGGER3_INPUT,
		TRIGGER4_INPUT,
		PATTERN_SELECT_CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		PULSE_OUT1_OUTPUT,
		PULSE_OUT2_OUTPUT,
		PULSE_OUT3_OUTPUT,
		PULSE_OUT4_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		LIGHT1_LIGHT,
		LIGHT2_LIGHT,
		LIGHT3_LIGHT,
		LIGHT4_LIGHT,
		NUM_LIGHTS
	};

	// Number of pattern slots
	static const int numberOfPatternSlots = 4;

	// Pattern slot values.
	unsigned int patternSlotTestSelectionValues[numberOfPatternSlots] = {8, 4, 2, 1};

	// Value of the pattern select parameter.
	float patternSelection = 0.f;

	// Value of the pattern select CV input voltage.
	float patternSelectionCVInputVoltage = 0.f;

	// Value of the pattern select CV input voltage attenuator.
	float patternSelectionCVInputVoltageAttenuator = 0.f;

	// Values of the main input voltages.
	float inputVoltage[numberOfPatternSlots] = {0.f};

	// As it says on the tin.
	bool isCurrentPatternSlotSelected = false;

	// Set the maximum value of the pattern select paramter to be just less than actual maximum to avoid wraparound to zero.
	float maxPatternSelectionRangeValue = patternSlotTestSelectionValues[0] * 2 - 0.01;

	// Schmitt Triggers to process the trigger inputs.
	dsp::SchmittTrigger inputTrigger[4];

	// Pulse outputs.
	dsp::PulseGenerator pulseOutput[4];

	Bitwise() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PATTERN_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Pattern select CV attenuator");
		configParam(PATTERN_SELECT_PARAM, 0.f, maxPatternSelectionRangeValue, 0.f, "Pattern select");
	}

	void process(const ProcessArgs &args) override {

		// Get the value of the pattern select parameter.
		patternSelection = params[PATTERN_SELECT_PARAM].getValue();

		// Check if the pattern select CV input is in use.
		if (inputs[PATTERN_SELECT_CV_INPUT].isConnected()) {

			// Get the value of the pattern select CV input voltage.
			patternSelectionCVInputVoltage = inputs[PATTERN_SELECT_CV_INPUT].getVoltage();

			// Get the value of the pattern select CV input voltage attenuator.
			patternSelectionCVInputVoltageAttenuator = params[PATTERN_SELECT_CV_ATN_PARAM].getValue();

			// Set the pattern select value based on the combination of the pattern select parameter value, pattern select CV input voltage and pattern select CV input voltage attenuator. Phew!
			patternSelection = clampSafe(rescale(patternSelectionCVInputVoltage * patternSelectionCVInputVoltageAttenuator * pow(2,numberOfPatternSlots), 0.f, 10.f * pow(2,numberOfPatternSlots), 0.f, pow(2,numberOfPatternSlots)) + patternSelection, 0.f, maxPatternSelectionRangeValue);
		}

		// Loop through the slots and do stuff on them.
		for (int i = 0; i < numberOfPatternSlots; i++) {

			// Is the current slot selected in the pattern?
			isCurrentPatternSlotSelected = ((unsigned int)patternSelection & patternSlotTestSelectionValues[i]) ? true : false;

			// Set the slot light's brightness to indicate if it is selected. Yes means full brightness, no means hit the road, pal.
			lights[i].setBrightness(isCurrentPatternSlotSelected ? 1.f : 0.f);

			// Carry out the sample and hold operation on the slot only if the current pattern slot is selected.
			// Also, send a pulse if this slot is selected and triggered.
			if (inputTrigger[i].process(inputs[4 + i].getVoltage() / 0.7) && isCurrentPatternSlotSelected) {
				inputVoltage[i] = inputs[i].getVoltage();

				pulseOutput[i].trigger(1e-3f);
			}

			// Set the output voltage for the current slot.
			outputs[i].setVoltage(inputVoltage[i]);

			// 	Set the pulse outpout for the current slot.
			outputs[4 + i].setVoltage(pulseOutput[i].process(args.sampleTime));

		} // End of do stuff on the slots for loop.

	} // End of process() function.
};


struct BitwiseWidget : ModuleWidget {
	BitwiseWidget(Bitwise *module) {
		setModule(module);

		// Load the SVG file for the panel.
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bitwise.svg")));

		// PANEL SCREWS

		// Top left.
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		// Top right.
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		// Bottom left.
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		// Bottom right.
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


		// PARAMETERS

		// Pattern selection CV parameter knob.
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(13.758, 26.077)), module, Bitwise::PATTERN_SELECT_CV_ATN_PARAM));
		// Pattern selection parameter knob.
		addParam(createParamCentered<RoundHugeBlackKnob>(mm2px(Vec(50.8, 27.208)), module, Bitwise::PATTERN_SELECT_PARAM));


		// INPUTS

		// Pattern select CV input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(26.106, 26.077)), module, Bitwise::PATTERN_SELECT_CV_INPUT));
		// Slot 1 CV input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.758, 53.667)), module, Bitwise::INPUT1_INPUT));
		// Slot 1 Trigger input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.758, 85.417)), module, Bitwise::TRIGGER1_INPUT));
		// Slot 2 CV input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.453, 53.667)), module, Bitwise::INPUT2_INPUT));
		// Slot 3 Trigger input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.453, 85.417)), module, Bitwise::TRIGGER2_INPUT));
		// Slot 3 CV input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.147, 53.667)), module, Bitwise::INPUT3_INPUT));
		// Slot 3 Trigger input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.147, 85.417)), module, Bitwise::TRIGGER3_INPUT));
		// Slot 4 CV input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(87.842, 53.667)), module, Bitwise::INPUT4_INPUT));
		// Slot 4 Trigger input.
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(87.842, 85.417)), module, Bitwise::TRIGGER4_INPUT));


		// OUTPUTS

		// Slot 1 voltage output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(13.758, 101.292)), module, Bitwise::OUT1_OUTPUT));
		// Slot 2 voltage output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.453, 101.292)), module, Bitwise::OUT2_OUTPUT));
		// Slot 3 voltage output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(63.147, 101.292)), module, Bitwise::OUT3_OUTPUT));
		// Slot 4 voltage output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(87.842, 101.292)), module, Bitwise::OUT4_OUTPUT));
		// Slot 1 pulse output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(13.758, 117.167)), module, Bitwise::PULSE_OUT1_OUTPUT));
		// Slot 2 pulse output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.453, 117.167)), module, Bitwise::PULSE_OUT2_OUTPUT));
		// Slot 3 pulse output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(63.147, 117.167)), module, Bitwise::PULSE_OUT3_OUTPUT));
		// Slot 4 pulse output.
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(87.842, 117.167)), module, Bitwise::PULSE_OUT4_OUTPUT));


		// LIGHTS

		// Slot 1 selected indicator light.
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.758, 69.542)), module, Bitwise::LIGHT1_LIGHT));
		// Slot 2 selected indicator light.
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(38.453, 69.542)), module, Bitwise::LIGHT2_LIGHT));
		// Slot 3 selected indicator light.
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(63.147, 69.542)), module, Bitwise::LIGHT3_LIGHT));
		// Slot 4 selected indicator light.
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(87.842, 69.542)), module, Bitwise::LIGHT4_LIGHT));
	}
};


Model *modelBitwise = createModel<Bitwise, BitwiseWidget>("Bitwise");
