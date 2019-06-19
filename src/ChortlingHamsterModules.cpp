#include "ChortlingHamsterModules.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
	pluginInstance = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);
	p->addModel(modelPop);
}
