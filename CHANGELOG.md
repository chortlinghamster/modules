# Changelog for Chortling Hamster Modules

## v1.1.2

* Updates Bitwise code to use a custom NanoVG widget for the pattern display LEDs instead of 64 light components. Slight performance improvement.

## v1.1.1

* Updates incorrect tags for Bitwise in JSON manifest. Oops!
* Adds this CHANGELOG document. Which is a bit meta?
* Adds "calculator style" segment displays to Bitwise front panel to show the values of the row and pattern parameters, and moves around the CV knobs.
* Removes the "0 1" global attenuator label from Bitwise front panel. It was annoying me.

## v1.1.0

* Includes new module Bitwise.
* Updates Pop front panel design to match Bitwise.

## v1.0.1

* Rewrote Pop based on the instructions from the [VCV Plugin Development Tutorial](https://vcvrack.com/manual/PluginDevelopmentTutorial.html) and [Panel Guide](https://vcvrack.com/manual/Panel.html). No functionality has changed but the code is cleaner and no longer based on the now deprecated VCV Rack starter template from pre-v1 days.
* Pop.svg file now includes Inkscape "components" layer for placement of inputs and outputs, and has the correct millimeter based dimensions.
* README updated to remove reference to VCV Rack starter template.

## v1.0.0

* Updated to work with VCV Rack v1.+.