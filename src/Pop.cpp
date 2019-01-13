#include "ChortlingHamsterModules.hpp"

struct Pop : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		VOLTAGE_INPUT,
		TRIGGER_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		VOLTAGE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float sampledVoltage = 0.f;

	SchmittTrigger st;

	Pop() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};


void Pop::step() {
	// Hold input voltage if triggered
	if (st.process(rescale(inputs[TRIGGER_INPUT].value, 0.1f, 2.f, 0.f, 1.f)))
		sampledVoltage = inputs[VOLTAGE_INPUT].value;
	outputs[VOLTAGE_OUTPUT].value = sampledVoltage;
}


struct PopWidget : ModuleWidget {
	PopWidget(Pop *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Pop.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(Port::create<PJ301MPort>(Vec(2.5, 57), Port::INPUT, module, Pop::VOLTAGE_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(2.5, 180), Port::INPUT, module, Pop::TRIGGER_INPUT));

		addOutput(Port::create<PJ301MPort>(Vec(2.5, 310), Port::OUTPUT, module, Pop::VOLTAGE_OUTPUT));
	}
};

Model *modelPop = Model::create<Pop, PopWidget>("Chortling Hamster Modules", "Pop", "Pop", SAMPLE_AND_HOLD_TAG);
