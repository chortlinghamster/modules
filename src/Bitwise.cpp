#include "ChortlingHamsterModules.hpp"

// Helper function to set the values of the row and pattern selections. Very helpful.
static int setSelection(float val, float cv, float attn, float max)
{
	return (int)clampSafe(
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

// Button up your overcoat, because here we go!
struct Bitwise : Module {
	enum ParamIds {
		ROW_SELECT_PARAM,
		ROW_SELECT_CV_ATN_PARAM,
		PATTERN_SELECT_PARAM,
		PATTERN_SELECT_CV_ATN_PARAM,
		GLOBAL_VOLTAGE_ATTENUATOR_PARAM,
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
		NUM_LIGHTS
	};

	// Setting these just in case. For laterz, y'know? I think it's fairly clear what they represent.
	static const int numberOfPatterns = 8;
	static const int numberOfRows = 16;
	static const int numberOfColumns = 4;
	static const int numberOfPatternSlots = numberOfRows * numberOfColumns;

	// Preset patterns. Patterns, patterns and more patterns. Well. Eight of them. I mean, you've got to stop somewhere, right?
	int patterns[numberOfPatterns][numberOfPatternSlots] = {
		// Pattern 1. Hexed. 0 to 15 in binary. Two's complement. Three's an error.
		{
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 1, 0,
			0, 0, 1, 1,
			0, 1, 0, 0,
			0, 1, 0, 1,
			0, 1, 1, 0,
			0, 1, 1, 1,
			1, 0, 0, 0,
			1, 0, 0, 1,
			1, 0, 1, 0,
			1, 0, 1, 1,
			1, 1, 0, 0,
			1, 1, 0, 1,
			1, 1, 1, 0,
			1, 1, 1, 1
		},
		// Pattern 2. Stairs. Or chevrons. Whatevs.
		{
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 1, 1,
			0, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, 1, 0,
			1, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0,
			1, 1, 1, 0,
			1, 1, 1, 1,
			0, 1, 1, 1,
			0, 0, 1, 1,
			0, 0, 0, 1
		},
		// Pattern 3. Snow. Either winter is coming or I forgot to turn the heating on.
		{
			0, 1, 0, 0,
			1, 0, 0, 1,
			1, 0, 1, 0,
			1, 0, 0, 1,
			0, 1, 0, 0,
			1, 0, 0, 1,
			0, 0, 1, 0,
			1, 1, 1, 1,
			0, 1, 0, 1,
			1, 0, 0, 1,
			0, 0, 1, 0,
			1, 0, 0, 1,
			1, 1, 0, 0,
			1, 0, 0, 1,
			0, 0, 1, 1,
			1, 0, 0, 1
		},
		// Pattern 4. Wriggle. Is it a sine wave? Is it a snake? Is it a one-dimensional C++ array?
		{
			0, 0, 0, 1,
			0, 0, 0, 1,
			0, 0, 1, 1,
			0, 0, 1, 0,
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 1, 1,
			0, 0, 0, 1,
			0, 0, 0, 1
		},
		// Pattern 5. Apartment. If you squint a bit.
		{
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			1, 0, 1, 0,
			1, 0, 1, 0,
			1, 0, 1, 0,
			1, 0, 1, 0,
			0, 0, 0, 0,
			1, 0, 1, 0,
			1, 0, 1, 0,
			1, 0, 1, 0,
			1, 0, 1, 0
		},
		// Pattern 6. Papneo. Rot4.
		{
			1, 0, 0, 0,
			1, 0, 1, 1,
			1, 0, 0, 1,
			1, 1, 0, 1,
			0, 0, 0, 1,
			1, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			1, 1, 0, 1,
			1, 1, 0, 1,
			0, 0, 0, 1,
			1, 1, 0, 1,
			0, 1, 0, 0,
			0, 1, 0, 1,
			0, 1, 0, 1,
			0, 0, 0, 0
		},
		// Pattern 7. Shimmer. Where's the lighthouse keeper when you need him?
		{
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		},
		// Patern 8. CHM.
		{
			0, 0, 0, 0,
			1, 0, 0, 1,
			0, 1, 1, 0,
			1, 0, 0, 1,
			1, 0, 0, 1,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 0, 0, 1,
			0, 1, 1, 0,
			1, 0, 0, 1,
			0, 1, 1, 0,
			1, 0, 0, 1,
			0, 1, 1, 0,
			1, 0, 0, 1,
			0, 1, 1, 0
		}
	};

	// This will hold the value of the selected row.
	int row = 0;
	int oldRow = 0;

	// This will hold the value of the selected pattern.
	int pattern = 0;
	int oldPattern = 0;

	// This will hold the value of the global voltage out attenuator.
	float globalAttenuatorVoltage = 0.f;

	// This will hold the values of the main input voltages.
	float inputVoltage[numberOfColumns] = {0.f};

	// As it says on the tin.
	int isTheCurrentColumnSelected = 0;

	// Are there any descriptive variable names in the house?
	bool hasTriggerAllBeenTriggered = false;
	bool isPolyphonicOutCableConnected = false;

	// Here are some Schmitt Triggers which we will use to check when trigger inputs are triggered.
	// 0 to 3: trig inputs.
	// 4: trigger all input.
	dsp::SchmittTrigger triggers[5];

	// And here, blinking in the light of a strange new dawn, some tiny little pulse generators. Aren't they just so gosh darn cute?
	// 0 to 3: pulse output voltages.
	// 4 to 7: pulse output lights.
	dsp::PulseGenerator pulses[8];

	// Dirty drawing flags. Harumph!
	bool dirtyPatternGrid = false;
	bool dirtySegmentDisplay = false;

	Bitwise() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		// Configure the row select parameter knobs.
		configParam(ROW_SELECT_PARAM, 1.f, (float)numberOfRows, 1.f, "Row select");
		configParam(ROW_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Row select CV attenuator");

		// Configure the pattern select parameter knobs.
		configParam(PATTERN_SELECT_PARAM, 1.f, (float)numberOfPatterns, 1.f, "Pattern select");
		configParam(PATTERN_SELECT_CV_ATN_PARAM, 0.f, 1.f, 0.f, "Pattern select CV attenuator");

		// Configure the global out attenuator parameter knob.
		configParam(GLOBAL_VOLTAGE_ATTENUATOR_PARAM, 0.f, 1.f, 1.f, "Global voltage output attenuator");
	}


	void process(const ProcessArgs &args) override {

		// Remember, row and pattern are integers, so the floats returned from setSelection() have their fractional parts removed. Ouch!

		// Set the current row, using an arcane spell involving the row select parameter value, row select CV input voltage, row select CV input voltage attenuator, fresh garlic and a thermal blanket. Crikey!
		row = (inputs[ROW_SELECT_CV_INPUT].isConnected())
			? setSelection(
				// The row select parameter knob's value.
				params[ROW_SELECT_PARAM].getValue(),

				// The row select CV input value.
				inputs[ROW_SELECT_CV_INPUT].getVoltage(),

				// The row select CV attenuator knob's value.
				params[ROW_SELECT_CV_ATN_PARAM].getValue(),

				// The number of rows in the pattern. Cast to a float for reasons.
				(float)numberOfRows
			)
			// The row select CV input doesn't have a cable attached, so just use the value of the row select knob.
			: params[ROW_SELECT_PARAM].getValue();

		// Flag that the front panel needs repainting.
		if (row != oldRow) {
			dirtyPatternGrid = true;
			dirtySegmentDisplay = true;
		}
		oldRow = row;


		// Set the pattern select value, just like we did with the row selection. Only different!
		pattern = (inputs[PATTERN_SELECT_CV_INPUT].isConnected())
			? setSelection(
				// The pattern select parameter knob's value.
				params[PATTERN_SELECT_PARAM].getValue(),

				// The pattern select CV input value.
				inputs[PATTERN_SELECT_CV_INPUT].getVoltage(),

				// The pattern select CV attenuator knob's value.
				params[PATTERN_SELECT_CV_ATN_PARAM].getValue(),

				// The number of patterns. Cast to a float for reasons.
				(float)numberOfPatterns
			)
			// The pattern select CV input doesn't have a cable attached, so just use the value of the pattern select knob.
			: params[PATTERN_SELECT_PARAM].getValue();

		// Flag that the front panel needs repainting.
		if (pattern != oldPattern) {
			dirtyPatternGrid = true;
			dirtySegmentDisplay = true;
		}
		oldPattern = pattern;

		// Check the trigger all input to see if it has triggered.
		// Note to self. We do this OUTSIDE the loop below, otherwise the Schmitt Trigger gets reset after the first loop interation and only the first sample and hold circuit works when trigger all is fired. You doofus!
		hasTriggerAllBeenTriggered = triggers[4].process(inputs[TRIGGER_ALL_INPUT].getVoltage() / 0.7);

		// Check if a cable is attached to the polyphonic voltage output port.
		isPolyphonicOutCableConnected = outputs[POLYPHONIC_OUT_OUTPUT].isConnected();

		// Get the value of global voltage out attenuator.
		globalAttenuatorVoltage = params[GLOBAL_VOLTAGE_ATTENUATOR_PARAM].getValue();

		// Loop through the four sample and hold columns (circuits, things, whatevs) and do stuff to them. Nasty, evil stuff. Muahahahaaa! The knuckles! The horrible knuckles!
		for (int i = 0; i < numberOfColumns; i++) {

			// Is the current column selected in the pattern? Questions, questions.
			isTheCurrentColumnSelected = patterns[pattern - 1][((row - 1) * numberOfColumns) + i];

			// Set the current column's active light brightness to full brightness if, er, active. Darkness if not. Such terrible, terrible darkness…
			lights[i].setBrightness((isTheCurrentColumnSelected) ? 1.f : 0.f);

			// Here's the crazy logic which decides if a sample and hold column is triggered. If you can make it better I will buy you a bag of Jelly Tots. Note that individual trigger inputs take priority over the trigger all input.
			if (isTheCurrentColumnSelected) {
				// The current column is indeed selected.
				if (
					// Do the sample and hold if the current column's trigger in port is connected, and it's been triggered.
					(
						inputs[IN_TRIGGER + i].isConnected()
						&& triggers[i].process(inputs[IN_TRIGGER + i].getVoltage() / 0.7)
					)

					// OR!
					||

					// Do the sample and hold if the current column's trigger in port isn't connected, but the trigger all in port is, and the trigger all in port has been, er, triggered.
					(
						!inputs[IN_TRIGGER + i].isConnected()
						&& inputs[TRIGGER_ALL_INPUT].isConnected()
						&& hasTriggerAllBeenTriggered
					)
				) {
					// Capture the current column's input voltage.
					inputVoltage[i] = inputs[i].getVoltage();

					// Pulse the output voltage for this column.
					pulses[i].trigger(1e-3f);

					// Pulse the output light for this column.
					pulses[4 + i].trigger(1e-1f);
				} // End of if ... sample and hold check.
			} // End of if ... current column selected check.

			// Set the output voltage, if a cable is connected.
			if (outputs[OUT_VOLTAGE + i].isConnected())
				outputs[OUT_VOLTAGE + i].setVoltage(inputVoltage[i] * globalAttenuatorVoltage);

			// Set the polyphonic output voltage, if a cable is connected.
			if (isPolyphonicOutCableConnected)
				outputs[POLYPHONIC_OUT_OUTPUT].setVoltage(inputVoltage[i] * globalAttenuatorVoltage, i);

			// Set the pulse output for the current column, if a cable is connected.
			if (outputs[OUT_PULSE + i].isConnected())
				outputs[OUT_PULSE + i].setVoltage((pulses[i].process(args.sampleTime)) ? 10.f : 0.f);

			// Set the pulse light for the current column.
			lights[PULSE_LIGHT + i].setBrightness((pulses[4 + i].process(args.sampleTime)) ? 1.f : 0.f);

		} // End of do stuff on the columns for loop.

		// Set polyphonic output channels, if a cable is connected.
		if (isPolyphonicOutCableConnected)
			outputs[POLYPHONIC_OUT_OUTPUT].setChannels(numberOfColumns);

	} // End of process() function.

};
// Well, that was fun. What now?

// Oh, yeah. The widget. Here it comes! Look busy!
struct BitwiseWidget : ModuleWidget {

	// Contains the pattern LEDs and should only repaint itself when it's dirty. Ewww!
	struct CHMPatternGridFramebufferWidget : FramebufferWidget{
		Bitwise *module;

		CHMPatternGridFramebufferWidget(Bitwise *m){
			module = m;
		}

		void step() override{
			// Only draw if dirty.
			if(module->dirtyPatternGrid) {
				FramebufferWidget::dirty = true;
				module->dirtyPatternGrid = false;
			}
			FramebufferWidget::step();
		}
	};

	// Pattern LEDs.
	struct CHMPatternGridTransparentWidget : TransparentWidget{
		Bitwise *module;

		CHMPatternGridTransparentWidget(Bitwise *m){
			module = m;
			box.pos = mm2px(Vec(28.935, 7.77));
			box.size = mm2px(Vec(13.25, 53.45));
		}

		void draw(const DrawArgs &args) override {
			float ledRadius = mm2px(1.1);
			float ledSpacing = mm2px(3.35);
			float ledOffset = mm2px(0.5);

			// Draw LEDs not in current pattern. (Off.)
			nvgBeginPath(args.vg);
			nvgFillColor(args.vg, nvgRGBA(80,80,80,255));
			for (int y = 0; y < module->numberOfRows; y++) {
				for (int x = 0; x < module->numberOfColumns; x++) {
					if (module->patterns[module->pattern-1][(y * module->numberOfColumns) + x] == 0)
						nvgCircle(args.vg, ledOffset + ledRadius + (ledSpacing * x), ledOffset + ledRadius + (ledSpacing * y), ledRadius);
				}
			}
			nvgFill(args.vg);

			// Draw LEDs in current pattern. (Half on.)
			nvgBeginPath(args.vg);
			nvgFillColor(args.vg, nvgRGBA(160,160,0,255));
			for (int y = 0; y < module->numberOfRows; y++) {
				for (int x = 0; x < module->numberOfColumns; x++) {
					if (module->patterns[module->pattern-1][(y * module->numberOfColumns) + x] == 1 && y != module->row-1)
						nvgCircle(args.vg, ledOffset + ledRadius + (ledSpacing * x), ledOffset + ledRadius + (ledSpacing * y), ledRadius);
				}
			}
			nvgFill(args.vg);

			// Draw LEDs in current pattern and row. (Full on.)
			nvgBeginPath(args.vg);
			nvgFillColor(args.vg, nvgRGBA(255,255,215,255));
			for (int y = 0; y < module->numberOfRows; y++) {
				for (int x = 0; x < module->numberOfColumns; x++) {
					if (module->patterns[module->pattern-1][(y * module->numberOfColumns) + x] == 1 && y == module->row-1)
						nvgCircle(args.vg, ledOffset + ledRadius + (ledSpacing * x), ledOffset + ledRadius + (ledSpacing * y), ledRadius);
				}
			}
			nvgFill(args.vg);

			// Draw LEDs in current pattern and row, glow. (Full on.)
			nvgBeginPath(args.vg);
			nvgFillColor(args.vg, nvgRGBA(255,255,255,60));
			for (int y = 0; y < module->numberOfRows; y++) {
				for (int x = 0; x < module->numberOfColumns; x++) {
					if (module->patterns[module->pattern-1][(y * module->numberOfColumns) + x] == 1 && y == module->row-1)
						nvgCircle(args.vg, ledOffset + ledRadius + (ledSpacing * x), ledOffset + ledRadius + (ledSpacing * y), ledRadius + ledOffset);
				}
			}
			nvgFill(args.vg);

		}
	};

	// Contains the segment displays and should only repaint itself when it's dirty. Ewww!
	// Tip o' th' hat to https://community.vcvrack.com/t/framebufferwidget-question/3041.
	struct CHMSegmentDisplayFramebufferWidget : FramebufferWidget{
		Bitwise *module;
		CHMSegmentDisplayFramebufferWidget(Bitwise *m){
			module = m;
		}
		void step() override{
			// Only draw if dirty.
			if(module->dirtySegmentDisplay) {
				FramebufferWidget::dirty = true;
				module->dirtySegmentDisplay = false;
			}
			FramebufferWidget::step();
		}
	};

	// Generic segment display.
	// Thanks to Impromptu Modular as I reviewed the Clocked code to help me to understand how to create the segment display widget.
	struct CHMSegmentDisplayWidget : TransparentWidget{
		Bitwise *module;
		std::shared_ptr<Font> font;
		int *valueToDisplay;
		Vec textOffset;
		bool isHex;

		CHMSegmentDisplayWidget(Bitwise *m, Vec pos, Vec size, Vec to, int *v, bool ih){
			module = m;
			textOffset = to;
			valueToDisplay = v;
			isHex = ih;

			box.pos = mm2px(Vec(pos.x, pos.y));
			box.size = mm2px(Vec(size.x, size.y));
			font = APP->window->loadFont(asset::plugin(pluginInstance, "res/fonts/dseg7-modern/DSEG7Modern-BoldItalic.ttf"));
		}

		void draw(const DrawArgs &args) override {
			// Draw the "inactive" display segments. Ahem!
			nvgBeginPath(args.vg);
			nvgFontFaceId(args.vg, font->handle);
			nvgFontSize(args.vg, 16);
			nvgTextAlign(args.vg, NVG_ALIGN_TOP | NVG_ALIGN_RIGHT);
			nvgFillColor(args.vg, nvgRGB(70, 70, 70));
			// A little jig here to convert the int value to a string. See also the use of c_str() in the next line.
			std::string valueAsString = std::to_string((isHex ? 88 : 8));
			nvgText(args.vg, mm2px(textOffset.x), mm2px(textOffset.y), valueAsString.c_str(), NULL);
			nvgFill(args.vg);

			// Draw the actual value.
			nvgBeginPath(args.vg);
			nvgFontFaceId(args.vg, font->handle);
			nvgFontSize(args.vg, 16);
			nvgTextAlign(args.vg, NVG_ALIGN_TOP | NVG_ALIGN_RIGHT);
			nvgFillColor(args.vg, nvgRGB(200, 200, 10));
			// A little jig here to convert the int value to a string. See also the use of c_str() in the next line.
			valueAsString = std::to_string(*valueToDisplay);
			nvgText(args.vg, mm2px(textOffset.x), mm2px(textOffset.y), valueAsString.c_str(), NULL);
			nvgFill(args.vg);
		}
	};

	BitwiseWidget(Bitwise *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bitwise.svg")));

		// Using if (module) stops Rack crashing when it tries to render a preview of the front panel in the module browser, as the segment displays don't have any values to render in the module browser.
		if (module) {
			// Pattern LEDs.
			// Create the framebuffer widget which will contain the pattern LEDs widget.
			CHMPatternGridFramebufferWidget *chmPatternGridDisplay = new CHMPatternGridFramebufferWidget(module);
			// Create the pattern LEDs widget.
			CHMPatternGridTransparentWidget *chmPatternGrid = new CHMPatternGridTransparentWidget(module);
			// Add the pattern LEDs widget to the framebuffer widget.
			chmPatternGridDisplay->addChild(chmPatternGrid);
			// Add the framebuffer widget to the main module widget.
			addChild(chmPatternGridDisplay);

			// Segment displays.
			// Create the framebuffer widget which will contain the segment display widgets for the row and pattern selection values.
			CHMSegmentDisplayFramebufferWidget *chmSegmentDisplays = new CHMSegmentDisplayFramebufferWidget(module);
			// Create the segment display widget for the row value.
			CHMSegmentDisplayWidget *chmRowSegmentDisplay = new CHMSegmentDisplayWidget(module, Vec(7.674, 10.659), Vec(12.7, 7.114), Vec(10.8, 0.85), &module->row, true);
			// Create the segement display widget for the pattern value.
			CHMSegmentDisplayWidget *chmPatternSegmentDisplay = new CHMSegmentDisplayWidget(module, Vec(51.038, 10.659), Vec(12.7, 7.114), Vec(8.75, 0.85), &module->pattern, false);
			// Add the segment display widgets to the framebuffer widget.
			chmSegmentDisplays->addChild(chmRowSegmentDisplay);
			chmSegmentDisplays->addChild(chmPatternSegmentDisplay);
			// Add the framebuffer widget to the main module widget.
			addChild(chmSegmentDisplays);
		}

		// Screwy screws!
		addChild(createWidget<ScrewBlack>(Vec(0, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// Knobbly knobs!
		addParam(createParamCentered<CHMRoundLargeSnapKnob>(mm2px(Vec(14.024, 25.951)), module, Bitwise::ROW_SELECT_PARAM));
		addParam(createParamCentered<CHMRoundLargeSnapKnob>(mm2px(Vec(57.388, 25.951)), module, Bitwise::PATTERN_SELECT_PARAM));
		addParam(createParamCentered<CHMRoundSmallKnob>(mm2px(Vec(6.456, 38.307)), module, Bitwise::ROW_SELECT_CV_ATN_PARAM));
		addParam(createParamCentered<CHMRoundSmallKnob>(mm2px(Vec(64.664, 38.307)), module, Bitwise::PATTERN_SELECT_CV_ATN_PARAM));
		addParam(createParamCentered<CHMRoundLargeKnob>(mm2px(Vec(35.56, 117.335)), module, Bitwise::GLOBAL_VOLTAGE_ATTENUATOR_PARAM));

		// Inputs!
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.024, 38.307)), module, Bitwise::ROW_SELECT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(57.388, 38.307)), module, Bitwise::PATTERN_SELECT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 70.53)), module, Bitwise::IN_VOLTAGE + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 70.53)), module, Bitwise::IN_VOLTAGE + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 70.53)), module, Bitwise::IN_VOLTAGE + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 70.53)), module, Bitwise::IN_VOLTAGE + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.456, 84.192)), module, Bitwise::IN_TRIGGER + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.008, 84.192)), module, Bitwise::IN_TRIGGER + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(35.56, 84.192)), module, Bitwise::TRIGGER_ALL_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.112, 84.192)), module, Bitwise::IN_TRIGGER + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(64.664, 84.192)), module, Bitwise::IN_TRIGGER + 3));

		// Outputs!
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 100.499)), module, Bitwise::OUT_VOLTAGE + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 100.499)), module, Bitwise::OUT_VOLTAGE + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.56, 100.499)), module, Bitwise::POLYPHONIC_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 100.499)), module, Bitwise::OUT_VOLTAGE + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 100.499)), module, Bitwise::OUT_VOLTAGE + 3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 117.335)), module, Bitwise::OUT_PULSE + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.008, 117.335)), module, Bitwise::OUT_PULSE + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.112, 117.335)), module, Bitwise::OUT_PULSE + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(64.664, 117.335)), module, Bitwise::OUT_PULSE + 3));

		// (Shake it all about puts!)

		// Blinkenlights!
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(6.456, 58.665)), module, Bitwise::ACTIVE_LIGHT + 0));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(21.008, 58.665)), module, Bitwise::ACTIVE_LIGHT + 1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(50.112, 58.665)), module, Bitwise::ACTIVE_LIGHT + 2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(64.664, 58.665)), module, Bitwise::ACTIVE_LIGHT + 3));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(6.456, 123.761)), module, Bitwise::PULSE_LIGHT + 0));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(21.008, 123.761)), module, Bitwise::PULSE_LIGHT + 1));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(50.112, 123.761)), module, Bitwise::PULSE_LIGHT + 2));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(64.664, 123.761)), module, Bitwise::PULSE_LIGHT + 3));
	}
};


Model *modelBitwise = createModel<Bitwise, BitwiseWidget>("Bitwise");

// And that, as the onion said, is shallot. I hope you enjoyed this as much as I did. Time for a cuppa.
