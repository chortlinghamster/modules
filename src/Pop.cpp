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

	dsp::SchmittTrigger st;

	Pop() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs &args) override {
		// Hold input voltage if triggered
		if (st.process(rescale(inputs[TRIGGER_INPUT].getVoltage(), 0.1f, 2.f, 0.f, 1.f)))
			sampledVoltage = inputs[VOLTAGE_INPUT].getVoltage();
		outputs[VOLTAGE_OUTPUT].setVoltage(sampledVoltage);
	}
};


struct PopWidget : ModuleWidget {
	PopWidget(Pop *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Pop.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 22.874)), module, Pop::VOLTAGE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 64.382)), module, Pop::TRIGGER_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.08, 108.931)), module, Pop::VOLTAGE_OUTPUT));
	}
};


Model *modelPop = createModel<Pop, PopWidget>("Pop");