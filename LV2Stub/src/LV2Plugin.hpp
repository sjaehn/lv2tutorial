#ifndef LV2PLUGIN_HPP_
#define LV2PLUGIN_HPP_


// LV2 core, always required
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>


// LV2 extensions, optional
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
//#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
//#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
//#include <lv2/lv2plug.in/ns/ext/state/state.h>
//#include <lv2/lv2plug.in/ns/ext/worker/worker.h>
//#include <lv2/lv2plug.in/ns/ext/time/time.h>
//#include <lv2/lv2plug.in/ns/ext/midi/midi.h>


// Plugin URIs
#define LV2PLUGIN_URI "https://www.your-url.com/LV2Stub"
//#define LV2PLUGIN_GUI_URI LV2PLUGIN_URI "#gui"


// Port definition
#include "Ports.hpp"


// Optional, map URI(D)s. Only needed if map / atom / urid is used.
#include "Urids.hpp"


// Plugin class declaration
class LV2Plugin
{
public:
	LV2Plugin (double samplerate, const char* bundlePath, const LV2_Feature* const* features);
	~LV2Plugin ();

	void connect_port (uint32_t port, void *data);
	void activate ();
	void run (uint32_t n_samples);
	void deactivate ();

	// Optional worker methods
	//LV2_Worker_Status work (LV2_Worker_Respond_Function respond, LV2_Worker_Respond_Handle handle, uint32_t size, const void* data);
	//LV2_Worker_Status work_response (uint32_t size, const void* data);
	//LV2_Worker_Status end_run ();

	// Optional state methods
	//LV2_State_Status state_save (LV2_State_Store_Function store, LV2_State_Handle handle, uint32_t flags, const LV2_Feature* const* features);
	//LV2_State_Status state_restore (LV2_State_Retrieve_Function retrieve, LV2_State_Handle handle, uint32_t flags, const LV2_Feature* const* features);

private:

	void process (uint32_t start, uint32_t end);

	double rate;

	// Ports
	LV2_Atom_Sequence* control;
	LV2_Atom_Sequence* notify;
	float* audio_in;
	float* audio_out;
	float* controllers[LV2PLUGIN_NR_CONTROLLERS];

	// Optional map feature
	//LV2_URID_Map* map;
	//LV2PluginURIDs urids;

	// Optional atom output
	//LV2_Atom_Forge forge;
	//LV2_Atom_Forge_Frame forge_frame;

};

#endif /* LV2PLUGIN_HPP_ */
