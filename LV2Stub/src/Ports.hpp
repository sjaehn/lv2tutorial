#ifndef PORTS_HPP_
#define PORTS_HPP_

// Plugin ports as declared in the .ttl file
enum LV2PluginPorts
{
	LV2PLUGIN_CONTROL			= 0,
	LV2PLUGIN_NOTIFY			= 1,
	LV2PLUGIN_AUDIO_IN			= 2,
	LV2PLUGIN_AUDIO_OUT			= 3,
	LV2PLUGIN_CONTROLLERS		= 4
	
};

// Plugin control ports as declared in the .ttl file, offset to
// LV2PluginPorts::LV2PLUGIN_CONTROLLERS
enum LV2PluginControllers
{
	LV2PLUGIN_VALUE_CTRL		= 0,
	LV2PLUGIN_TOGGLE_CTRL		= 1,
	LV2PLUGIN_ENUM_CTRL			= 2,
	LV2PLUGIN_NR_CONTROLLERS	= 3
};

#endif /* PORTS_HPP_ */