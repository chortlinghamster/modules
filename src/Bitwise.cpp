#include "ChortlingHamsterModules.hpp"

static float setSelection(float &val, float &cv, float &attn, float &max)
{
	return clampSafe(
		rescale(
			cv * attn * max,
			0.f,
			10.f * max,
			0.f,
			max
		)
		+ val,
		1.f,
		max
	);
}


struct Bitwise : Module {
	enum ParamIds {
		ROW_SELECT_PARAM,
		ROW_SELECT_CV_ATN_PARAM,
		PATTERN_SELECT_PARAM,
		PATTERN_SELECT_CV_ATN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_VOLTAGE, 4),
		ENUMS(IN_TRIGGER, 4),
		TRIGGER_ALL_INPUT,
		ROW_SELECT_CV_INPUT,
		PATTERN_SELECT_CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_VOLTAGE, 4),
		ENUMS(OUT_PULSE, 4),
		POLYPHONIC_OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(ACTIVE_LIGHT, 4),
		ENUMS(PULSE_LIGHT, 4),
		ENUMS(PATTERN_INDICATOR_LIGHT, 64),
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
	float row = 0.f;

	// Value of the row select CV input voltage.
	float rowCV = 0.f;

	// Value of the row select CV input voltage attenuator.
	float rowCVAttn = 0.f;

	// Value of the pattern select parameter.
	float pattern = 0.f;

	// Value of the pattern select CV input voltage.
	float patternCV = 0.f;

	// Value of the pattern select CV input voltage attenuator.
	float patternCVAttn = 0.f;

	// Values of the main input voltages.
	float inputVoltage[numberOfColumns] = {0.f};

	// As it says on the tin.
	bool isCurrentColumnSelected = false;

	// Placeholder for currentPatternLight
	int currentPatternLight = 0;

	// Maximum value of the row select parameter.
	float maxRows = (float)numberOfRows;

	// Maximum value of the pattern select parameter.
	float maxPatterns = (float)numberOfPatterns;

	// Value of trig all input.
	float triggerAll = 0.f;

	// Schmitt Triggers to process the trigger inputs, 1 to 4 and all.
	dsp::SchmittTrigger triggers[5];

	// Pulse generators.
	dsp::PulseGenerator pulses[8];

	Bitwise() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configParam(PATTERN_SELECT_PARAM, 1.f, maxPatterns, 1.f, "Pattern select");
		configParam(PATTERN_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Pattern select CV attenuator");

		configParam(ROW_SELECT_PARAM, 1.f, maxRows, 1.f, "Row select");
		configParam(ROW_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Row select CV attenuator");
	}

	void process(const ProcessArgs &args) override {

		// Check trigger all input.
		triggerAll = triggers[4].process(inputs[TRIGGER_ALL_INPUT].getVoltage() / 0.7);

		// Get the value of the row select parameter.
		row = params[ROW_SELECT_PARAM].getValue();

		// Check if the row select CV input is in use.
		if (inputs[ROW_SELECT_CV_INPUT].isConnected()) {

			// Get the value of the row select CV input voltage.
			rowCV = inputs[ROW_SELECT_CV_INPUT].getVoltage();

			// Get the value of the row select CV input voltage attenuator.
			rowCVAttn = params[ROW_SELECT_CV_ATN_PARAM].getValue();

			// Set the row select value based on the combination of the row select parameter value, row select CV input voltage and row select CV input voltage attenuator. Phew!
			row = setSelection(row, rowCV, rowCVAttn, maxRows);
			// row = clampSafe(
			// 	rescale(
			// 		rowCV * rowCVAttn * maxRows,
			// 		0.f,
			// 		10.f * maxRows,
			// 		0.f,
			// 		maxRows
			// 	)
			// 	+ row,
			// 	1.f,
			// 	maxRow
			// );
		}

		// Get the value of the pattern select parameter.
		pattern = params[PATTERN_SELECT_PARAM].getValue();

		// Check if the pattern select CV input is in use.
		if (inputs[PATTERN_SELECT_CV_INPUT].isConnected()) {

			// Get the value of the pattern select CV input voltage.
			patternCV = inputs[PATTERN_SELECT_CV_INPUT].getVoltage();

			// Get the value of the pattern select CV input voltage attenuator.
			patternCVAttn = params[PATTERN_SELECT_CV_ATN_PARAM].getValue();

			// Set the pattern select value based on the combination of the pattern select parameter value, pattern select CV input voltage and pattern select CV input voltage attenuator. Phew!
			pattern = setSelection(pattern, patternCV, patternCVAttn, maxPatterns);

			// pattern = clampSafe(
			// 	rescale(
			// 		patternCV * patternCVAttn * numberOfPatterns,
			// 		0.f,
			// 		10.f * numberOfPatterns,
			// 		0.f,
			// 		numberOfPatterns
			// 	)
			// 	+ pattern,
			// 	1.f,
			// 	maxPatterns
			// );
		}

		// Set pattern indicator lights.
		for (int i = 0; i < numberOfRows; i++) {
			for (int j = 0; j < numberOfColumns; j++) {
				float brightness = 0.f;
				currentPatternLight = j + (i * numberOfColumns) + 8;
				if (patterns[(unsigned int)pattern - 1][i][j] == 1) {
					if ((int)row - 1 == i) {
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
			isCurrentColumnSelected = patterns[(unsigned int)pattern - 1][(unsigned int)row - 1][i];

			// Set the column light's brightness to indicate if it is selected. Yes means full brightness, no means hit the road, pal.
			lights[i].setBrightness((isCurrentColumnSelected) ? 1.f : 0.f);

			if (inputs[4 + i].isConnected() && triggers[i].process(inputs[4 + i].getVoltage() / 0.7) && isCurrentColumnSelected) {

				// Capture the current column's input voltage.
				inputVoltage[i] = inputs[i].getVoltage();

				// Pulse the output for this column.
				pulses[i].trigger(1e-3f);

				// Pulse light trigger.
				pulses[4 + i].trigger(1e-1f);

			} else if (!inputs[4 + i].isConnected() && inputs[TRIGGER_ALL_INPUT].isConnected() && triggerAll && isCurrentColumnSelected) {
				// Capture the current column's input voltage.
				inputVoltage[i] = inputs[i].getVoltage();

				// Pulse the output for this column.
				pulses[i].trigger(1e-3f);

				// Pulse light trigger.
				pulses[4 + i].trigger(1e-1f);
			}

			// Set the output voltage for the current column.
			outputs[i].setVoltage(inputVoltage[i]);

			// Set the polyphonic output voltages.
			outputs[POLYPHONIC_OUT_OUTPUT].setVoltage(outputs[i].getVoltage(), i);

			// Set the pulse output for the current column.
			outputs[4 + i].setVoltage((pulses[i].process(args.sampleTime)) ? 10.f : 0.f);

			// Set the pulse light for the current column.
			lights[4 + i].setBrightness((pulses[4 + i].process(args.sampleTime)) ? 1.f : 0.f);

		} // End of do stuff on the columns for loop.

		// Set polyphonic output channels..
		outputs[POLYPHONIC_OUT_OUTPUT].setChannels(numberOfColumns);


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
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 72.647)), module, Bitwise::IN_VOLTAGE + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 72.647)), module, Bitwise::IN_VOLTAGE + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 72.647)), module, Bitwise::IN_VOLTAGE + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 72.647)), module, Bitwise::IN_VOLTAGE + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 85.779)), module, Bitwise::IN_TRIGGER + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 85.779)), module, Bitwise::IN_TRIGGER + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(35.56, 85.779)), module, Bitwise::TRIGGER_ALL_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 85.779)), module, Bitwise::IN_TRIGGER + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 85.779)), module, Bitwise::IN_TRIGGER + 3));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 102.086)), module, Bitwise::OUT_VOLTAGE + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 102.086)), module, Bitwise::OUT_VOLTAGE + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.56, 102.086)), module, Bitwise::POLYPHONIC_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 102.086)), module, Bitwise::OUT_VOLTAGE + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 102.086)), module, Bitwise::OUT_VOLTAGE + 3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 115.218)), module, Bitwise::OUT_PULSE + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 115.218)), module, Bitwise::OUT_PULSE + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 115.218)), module, Bitwise::OUT_PULSE + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 115.218)), module, Bitwise::OUT_PULSE + 3));

		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 0));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 1));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 2));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 8.946)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 3));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 4));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 5));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 6));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 12.216)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 7));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 8));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 9));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 10));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 15.487)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 11));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 12));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 13));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 14));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 18.757)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 15));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 16));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 17));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 18));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 22.028)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 19));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 20));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 21));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 22));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 25.298)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 23));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 24));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 25));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 26));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 28.569)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 27));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 28));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 29));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 30));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 31.84)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 31));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 32));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 33));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 34));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 35.11)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 35));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 36));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 37));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 38));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 38.381)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 39));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 40));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 41));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 42));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 41.651)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 43));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 44));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 45));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 46));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 44.922)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 47));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 48));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 49));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 50));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 48.192)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 51));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 52));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 53));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 54));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 51.463)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 55));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 56));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 57));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 58));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 54.733)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 59));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(30.531, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 60));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(33.884, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 61));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(37.236, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 62));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(40.589, 58.004)), module, Bitwise::PATTERN_INDICATOR_LIGHT + 63));


		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(6.456, 60.253)), module, Bitwise::ACTIVE_LIGHT + 0));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(21.008, 60.253)), module, Bitwise::ACTIVE_LIGHT + 1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(50.112, 60.253)), module, Bitwise::ACTIVE_LIGHT + 2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(64.664, 60.253)), module, Bitwise::ACTIVE_LIGHT + 3));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(6.456, 123.232)), module, Bitwise::PULSE_LIGHT + 0));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(21.008, 123.232)), module, Bitwise::PULSE_LIGHT + 1));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(50.112, 123.232)), module, Bitwise::PULSE_LIGHT + 2));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(64.664, 123.232)), module, Bitwise::PULSE_LIGHT + 3));
	}
};


Model *modelBitwise = createModel<Bitwise, BitwiseWidget>("Bitwise");
