#include "ChortlingHamsterModules.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelPop);
	p->addModel(modelBitwise);
	p->addModel(modelTie);
}
