#include "ChortlingHamsterModules.hpp"


struct Bitwise : Module {
	enum ParamIds {
		ROW_SELECT_PARAM,
		ROW_SELECT_CV_ATN_PARAM,
		PATTERN_SELECT_PARAM,
		PATTERN_SELECT_CV_ATN_PARAM,
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
		TRIGGER_ALL_INPUT,
		ROW_SELECT_CV_INPUT,
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
		POLYPHONIC_OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		SLOT_ACTIVE_LIGHT1_LIGHT,
		SLOT_ACTIVE_LIGHT2_LIGHT,
		SLOT_ACTIVE_LIGHT3_LIGHT,
		SLOT_ACTIVE_LIGHT4_LIGHT,
		SLOT_PULSE_LIGHT1_LIGHT,
		SLOT_PULSE_LIGHT2_LIGHT,
		SLOT_PULSE_LIGHT3_LIGHT,
		SLOT_PULSE_LIGHT4_LIGHT,
		PATTERN_INDICATOR_LIGHT_01_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_01_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_01_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_01_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_02_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_02_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_02_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_02_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_03_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_03_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_03_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_03_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_04_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_04_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_04_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_04_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_05_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_05_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_05_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_05_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_06_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_06_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_06_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_06_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_07_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_07_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_07_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_07_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_08_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_08_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_08_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_08_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_09_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_09_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_09_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_09_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_10_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_10_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_10_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_10_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_11_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_11_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_11_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_11_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_12_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_12_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_12_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_12_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_13_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_13_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_13_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_13_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_14_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_14_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_14_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_14_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_15_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_15_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_15_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_15_4_LIGHT,
		PATTERN_INDICATOR_LIGHT_16_1_LIGHT,
		PATTERN_INDICATOR_LIGHT_16_2_LIGHT,
		PATTERN_INDICATOR_LIGHT_16_3_LIGHT,
		PATTERN_INDICATOR_LIGHT_16_4_LIGHT,
		NUM_LIGHTS
	};

	// Being pedantic. You never know.
	static const int numberOfPatterns = 8;
	static const int numberOfRows = 16;
	static const int numberOfColumns = 4;

	// Test pattern
	int patterns[numberOfPatterns][numberOfRows][numberOfColumns] = {
		// 1 Hexed
		{
			{0, 0, 0, 0},
			{0, 0, 0, 1},
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 1, 0, 0},
			{0, 1, 0, 1},
			{0, 1, 1, 0},
			{0, 1, 1, 1},
			{1, 0, 0, 0},
			{1, 0, 0, 1},
			{1, 0, 1, 0},
			{1, 0, 1, 1},
			{1, 1, 0, 0},
			{1, 1, 0, 1},
			{1, 1, 1, 0},
			{1, 1, 1, 1}
		},
		// 2 Stairs
		{
			{0, 0, 0, 0},
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{1, 1, 1, 0},
			{1, 1, 1, 1},
			{0, 1, 1, 1},
			{0, 0, 1, 1},
			{0, 0, 0, 1}
		},
		// 3 Snow
		{
			{0, 1, 0, 0},
			{1, 0, 0, 1},
			{1, 0, 1, 0},
			{1, 0, 0, 1},
			{0, 1, 0, 0},
			{1, 0, 0, 1},
			{0, 0, 1, 0},
			{1, 1, 1, 1},
			{0, 1, 0, 1},
			{1, 0, 0, 1},
			{0, 0, 1, 0},
			{1, 0, 0, 1},
			{1, 1, 0, 0},
			{1, 0, 0, 1},
			{0, 0, 1, 1},
			{1, 0, 0, 1}
		},
		// 4 Wriggle
		{
			{0, 0, 0, 1},
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 1}
		},
		// 5 Apartments
		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0},
			{0, 0, 0, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0},
			{1, 0, 1, 0}
		},
		// 6 Papneo
		{
			{1, 0, 0, 0},
			{1, 0, 1, 1},
			{1, 0, 0, 1},
			{1, 1, 0, 1},
			{0, 0, 0, 1},
			{1, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{1, 1, 0, 1},
			{1, 1, 0, 1},
			{0, 0, 0, 1},
			{1, 1, 0, 1},
			{0, 1, 0, 0},
			{0, 1, 0, 1},
			{0, 1, 0, 1},
			{0, 0, 0, 0}
		},
		// 7 Shimmer
		{
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 8 CHM
		{
			{0, 0, 0, 0},
			{1, 0, 0, 1},
			{0, 1, 1, 0},
			{1, 0, 0, 1},
			{1, 0, 0, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 0, 0, 1},
			{0, 1, 1, 0},
			{1, 0, 0, 1},
			{0, 1, 1, 0},
			{1, 0, 0, 1},
			{0, 1, 1, 0},
			{1, 0, 0, 1},
			{0, 1, 1, 0}
		}
	};

	// Value of the row select parameter.
	float rowSelection = 0.f;

	// Value of the row select CV input voltage.
	float rowSelectionCVInputVoltage = 0.f;

	// Value of the row select CV input voltage attenuator.
	float rowSelectionCVInputVoltageAttenuator = 0.f;

	// Value of the pattern select parameter.
	float patternSelection = 0.f;

	// Value of the pattern select CV input voltage.
	float patternSelectionCVInputVoltage = 0.f;

	// Value of the pattern select CV input voltage attenuator.
	float patternSelectionCVInputVoltageAttenuator = 0.f;

	// Values of the main input voltages.
	float inputVoltage[numberOfColumns] = {0.f};

	// As it says on the tin.
	bool isCurrentColumnSelected = false;

	// Placeholder for currentPatternLight
	int currentPatternLight = 0;

	// Maximum value of the row select parameter.
	float maxRowSelectionRangeValue = (float)numberOfRows;

	// Maximum value of the pattern select parameter.
	float maxPatternSelectionRangeValue = (float)numberOfPatterns;

	// Value of trig all input.
	float triggerAllInputValue = 0.f;

	// Schmitt Triggers to process the trigger inputs, 1 to 4 and all.
	dsp::SchmittTrigger inputTrigger[5];

	// Pulse generators.
	dsp::PulseGenerator pulses[8];

	Bitwise() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configParam(PATTERN_SELECT_PARAM, 1.f, maxPatternSelectionRangeValue, 1.f, "Pattern select");
		configParam(PATTERN_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Pattern select CV attenuator");

		configParam(ROW_SELECT_PARAM, 1.f, maxRowSelectionRangeValue, 1.f, "Row select");
		configParam(ROW_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Row select CV attenuator");
	}

	void process(const ProcessArgs &args) override {

		// Check trigger all input.
		triggerAllInputValue = inputTrigger[4].process(inputs[TRIGGER_ALL_INPUT].getVoltage() / 0.7);

		// Get the value of the row select parameter.
		rowSelection = params[ROW_SELECT_PARAM].getValue();

		// Check if the row select CV input is in use.
		if (inputs[ROW_SELECT_CV_INPUT].isConnected()) {

			// Get the value of the row select CV input voltage.
			rowSelectionCVInputVoltage = inputs[ROW_SELECT_CV_INPUT].getVoltage();

			// Get the value of the row select CV input voltage attenuator.
			rowSelectionCVInputVoltageAttenuator = params[ROW_SELECT_CV_ATN_PARAM].getValue();

			// Set the row select value based on the combination of the row select parameter value, row select CV input voltage and row select CV input voltage attenuator. Phew!
			rowSelection = clampSafe(
				rescale(
					rowSelectionCVInputVoltage * rowSelectionCVInputVoltageAttenuator * numberOfRows,
					0.f,
					10.f * numberOfRows,
					0.f,
					numberOfRows
				)
				+ rowSelection,
				1.f,
				maxRowSelectionRangeValue
			);
		}

		// Get the value of the pattern select parameter.
		patternSelection = params[PATTERN_SELECT_PARAM].getValue();

		// Check if the pattern select CV input is in use.
		if (inputs[PATTERN_SELECT_CV_INPUT].isConnected()) {

			// Get the value of the pattern select CV input voltage.
			patternSelectionCVInputVoltage = inputs[PATTERN_SELECT_CV_INPUT].getVoltage();

			// Get the value of the pattern select CV input voltage attenuator.
			patternSelectionCVInputVoltageAttenuator = params[PATTERN_SELECT_CV_ATN_PARAM].getValue();

			// Set the pattern select value based on the combination of the pattern select parameter value, pattern select CV input voltage and pattern select CV input voltage attenuator. Phew!
			patternSelection = clampSafe(
				rescale(
					patternSelectionCVInputVoltage * patternSelectionCVInputVoltageAttenuator * numberOfPatterns,
					0.f,
					10.f * numberOfPatterns,
					0.f,
					numberOfPatterns
				)
				+ patternSelection,
				1.f,
				maxPatternSelectionRangeValue
			);
		}

		// Set pattern indicator lights.
		for (int i = 0; i < numberOfRows; i++) {
			for (int j = 0; j < numberOfColumns; j++) {
				float brightness = 0.f;
				currentPatternLight = j + (i * numberOfColumns) + 8;
				if (patterns[(unsigned int)patternSelection - 1][i][j] == 1) {
					if ((int)rowSelection - 1 == i) {
						brightness = 1.f;
					} else {
						brightness = 0.3f;
					}
				} else {
					brightness = 0.f;
				}
				lights[currentPatternLight].setBrightness(brightness);
			}
		}

		// Loop through the columns and do stuff on them.
		for (int i = 0; i < numberOfColumns; i++) {

			// Is the current column selected in the pattern?
			// @todo Refactor this to use the two-dimensional array.
			isCurrentColumnSelected = patterns[(unsigned int)patternSelection - 1][(unsigned int)rowSelection - 1][i];

			// Set the column light's brightness to indicate if it is selected. Yes means full brightness, no means hit the road, pal.
			lights[i].setBrightness((isCurrentColumnSelected) ? 1.f : 0.f);

			if (inputs[4 + i].isConnected() && inputTrigger[i].process(inputs[4 + i].getVoltage() / 0.7) && isCurrentColumnSelected) {

				// Capture the current column's input voltage.
				inputVoltage[i] = inputs[i].getVoltage();

				// Pulse the output for this column.
				pulses[i].trigger(1e-3f);

				// Pulse light trigger.
				pulses[4 + i].trigger(1e-1f);

			} else if (!inputs[4 + i].isConnected() && inputs[TRIGGER_ALL_INPUT].isConnected() && triggerAllInputValue && isCurrentColumnSelected) {
				// Capture the current column's input voltage.
				inputVoltage[i] = inputs[i].getVoltage();

				// Pulse the output for this column.
				pulses[i].trigger(1e-3f);

				// Pulse light trigger.
				pulses[4 + i].trigger(1e-1f);
			}

			// Set the output voltage for the current column.
			outputs[i].setVoltage(inputVoltage[i]);

			// Set the pulse output for the current column.
			outputs[4 + i].setVoltage((pulses[i].process(args.sampleTime)) ? 10.f : 0.f);

			// Set the pulse light for the current column.
			lights[4 + i].setBrightness((pulses[4 + i].process(args.sampleTime)) ? 1.f : 0.f);

		} // End of do stuff on the columns for loop.

	} // End of process() function.

};


struct BitwiseWidget : ModuleWidget {
	BitwiseWidget(Bitwise *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bitwise.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH * 2, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 3 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH * 2, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<CHMRoundLargeSnapKnob>(mm2px(Vec(19.42, 23.785)), module, Bitwise::ROW_SELECT_PARAM));
		addParam(createParamCentered<CHMRoundLargeSnapKnob>(mm2px(Vec(51.7, 23.785)), module, Bitwise::PATTERN_SELECT_PARAM));
		addParam(createParamCentered<CHMRoundSmallKnob>(mm2px(Vec(6.456, 35.871)), module, Bitwise::ROW_SELECT_CV_ATN_PARAM));
		addParam(createParamCentered<CHMRoundSmallKnob>(mm2px(Vec(64.664, 35.871)), module, Bitwise::PATTERN_SELECT_CV_ATN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 23.785)), module, Bitwise::ROW_SELECT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 23.785)), module, Bitwise::PATTERN_SELECT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 72.647)), module, Bitwise::INPUT1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 72.647)), module, Bitwise::INPUT2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 72.647)), module, Bitwise::INPUT3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 72.647)), module, Bitwise::INPUT4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 85.779)), module, Bitwise::TRIGGER1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 85.779)), module, Bitwise::TRIGGER2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(35.56, 85.779)), module, Bitwise::TRIGGER_ALL_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 85.779)), module, Bitwise::TRIGGER3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 85.779)), module, Bitwise::TRIGGER4_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 102.086)), module, Bitwise::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 102.086)), module, Bitwise::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.56, 102.086)), module, Bitwise::POLYPHONIC_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 102.086)), module, Bitwise::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 102.086)), module, Bitwise::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 115.218)), module, Bitwise::PULSE_OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 115.218)), module, Bitwise::PULSE_OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 115.218)), module, Bitwise::PULSE_OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 115.218)), module, Bitwise::PULSE_OUT4_OUTPUT));

		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT_01_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT_01_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT_01_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT_01_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT_02_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT_02_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT_02_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT_02_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT_03_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT_03_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT_03_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT_03_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT_04_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT_04_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT_04_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT_04_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT_05_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT_05_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT_05_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT_05_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT_06_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT_06_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT_06_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT_06_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT_07_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT_07_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT_07_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT_07_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT_08_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT_08_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT_08_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT_08_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT_09_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT_09_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT_09_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT_09_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT_10_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT_10_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT_10_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT_10_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT_11_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT_11_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT_11_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT_11_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT_12_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT_12_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT_12_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT_12_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT_13_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT_13_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT_13_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT_13_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT_14_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT_14_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT_14_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT_14_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT_15_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT_15_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT_15_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT_15_4_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT_16_1_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT_16_2_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT_16_3_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT_16_4_LIGHT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(6.456, 60.253)), module, Bitwise::SLOT_ACTIVE_LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(21.008, 60.253)), module, Bitwise::SLOT_ACTIVE_LIGHT2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(50.112, 60.253)), module, Bitwise::SLOT_ACTIVE_LIGHT3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(64.664, 60.253)), module, Bitwise::SLOT_ACTIVE_LIGHT4_LIGHT));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(6.456, 123.232)), module, Bitwise::SLOT_PULSE_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(21.008, 123.232)), module, Bitwise::SLOT_PULSE_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(50.112, 123.232)), module, Bitwise::SLOT_PULSE_LIGHT3_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(64.664, 123.232)), module, Bitwise::SLOT_PULSE_LIGHT4_LIGHT));
	}
};


Model *modelBitwise = createModel<Bitwise, BitwiseWidget>("Bitwise");
