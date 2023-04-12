#ifndef URIDS_HPP_
#define URIDS_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

// URIDs used for this plugin
struct LV2PluginURIDs
{
	LV2_URID lv2plugin_example;

	void map (LV2_URID_Map* m);
};

inline void LV2PluginURIDs::map (LV2_URID_Map* m)
{
	lv2plugin_example = m->map(m->handle, "https://www.your-url.com/LV2Stub#lv2plugin_example");
}

#endif /* URIDS_HPP_ */
