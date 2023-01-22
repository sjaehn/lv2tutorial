/* LV2 Plugin Template
 *
 * Copyright (C) 2018 - 2023 by Sven Jähnichen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "LV2Plugin.hpp"
//#include <lv2/lv2plug.in/ns/ext/time/time.h>
//#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <stdexcept>

LV2Plugin::LV2Plugin (double samplerate, const char* bundlePath, const LV2_Feature* const* features) :
	//map (nullptr),
	rate (samplerate),
	control (nullptr),
	notify (nullptr),
	audio_in (nullptr),
	audio_out (nullptr)
{
	// Init controllers
	for (int i = 0; i < LV2PLUGIN_NR_CONTROLLERS; ++i) controllers[i] = nullptr;

	// Optional scan host features for URID map
	/*for (int i = 0; features[i]; ++i)
	{
		if (strcmp(features[i]->URI, LV2_URID__map) == 0) map = static_cast<LV2_URID_Map*>(features[i]->data);
	}
	if (!map) throw std::invalid_argument ("Host does not support urid:map");
	urids.map (map);
	*/

	// Optional initialize forge
	//lv2_atom_forge_init (&forge, map);
}

LV2Plugin::~LV2Plugin () {}

void LV2Plugin::connect_port (uint32_t port, void *data)
{
	// Note: Everything within this method MUST be real-time safe!

	switch (port) 
	{
		case LV2PLUGIN_CONTROL:		control = static_cast<LV2_Atom_Sequence*>(data);
									break;

		case LV2PLUGIN_NOTIFY:		notify = static_cast<LV2_Atom_Sequence*>(data);
									break;

		case LV2PLUGIN_AUDIO_IN:	audio_in = static_cast<float*>(data);
									break;

		case LV2PLUGIN_AUDIO_OUT:	audio_out = static_cast<float*>(data);
									break;
		
		default:
			if ((port >= LV2PLUGIN_CONTROLLERS) && (port < LV2PLUGIN_CONTROLLERS + LV2PLUGIN_NR_CONTROLLERS))
			{
				controllers[port - LV2PLUGIN_CONTROLLERS] = static_cast<float*>(data);
			}
	}
}

void LV2Plugin::activate ()
{}

void LV2Plugin::deactivate ()
{}

void LV2Plugin::run (uint32_t n_samples)
{
	// Note: Everything within this method MUST be real-time safe!

	uint32_t last_t = 0;

	// Check ports
	if ((!control) || (!notify) || (!audio_in) || (!audio_out)) return;
	for (int i = 0; i < LV2PLUGIN_NR_CONTROLLERS; ++i)
	{
		if (!controllers[i]) return;
	}

	// Optional prepare forge buffer and initialize atom sequence
	//const uint32_t space = notify->atom.size;
	//lv2_atom_forge_set_buffer (&forge, (uint8_t*) notify, space);
	//lv2_atom_forge_sequence_head (&forge, &forge_frame, 0);

	// Optional process host / GUI atoms
	/*LV2_ATOM_SEQUENCE_FOREACH(controlPort, ev)
	{
		if ((ev->body.type == urids.atom_Object) || (ev->body.type == urids.atom_Blank))
		{
			const LV2_Atom_Object* obj = static_cast<const LV2_Atom_Object*>(&ev->body);

			if (obj->body.otype == ...)
			{
				LV2_Atom* oEx = nullptr;
				lv2_atom_object_get	(obj,
						     		 urids.lv2plugin_example, &oEx,
									 NULL);

				if (oEx && (oEx->type == urids.atom_Type_of_example))
				{

				}
		}

		uint32_t next_t = (ev->time.frames < n_samples ? ev->time.frames : n_samples);
		process (last_t, next_t);
		last_t = next_t;
	}*/

	// Process remaining samples
	if (last_t < n_samples) process (last_t, n_samples);


	// Optional close off sequence
	// lv2_atom_forge_pop (&forge, &forge_frame);
}

void LV2Plugin::process (uint32_t start, uint32_t end)
{
	for (uint32_t i = 0; i < end; ++i)
	{
		audio_out[i] = audio_in[i];
	}
}

/*LV2_State_Status LV2Plugin::state_save (LV2_State_Store_Function store, LV2_State_Handle handle, uint32_t flags, const LV2_Feature* const* features)
{
	store (handle, urids.lv2plugin_example, &example, sizeof(example), urids.atom_Type_of_example, LV2_STATE_IS_POD);

	return LV2_STATE_SUCCESS;
}*/

/*LV2_State_Status LV2Plugin::state_restore (LV2_State_Retrieve_Function retrieve, LV2_State_Handle handle, uint32_t flags, const LV2_Feature* const* features)
{
	size_t   size;
	uint32_t type;
	uint32_t valflags;

	// Restore sharedDataNr
	const void* exampleData = retrieve (handle, urids.lv2plugin_example, &size, &type, &valflags);
	if (exampleData && (type == urids.atom_Type_of_example))
	{
		
	}

	return LV2_STATE_SUCCESS;
}*/

/*LV2_Worker_Status LV2Plugin::work (LV2_Worker_Respond_Function respond, LV2_Worker_Respond_Handle handle, uint32_t size, const void* data)
{}*/

/*LV2_Worker_Status LV2Plugin::work_response (uint32_t size, const void* data)
{}*/

/*LV2_Worker_Status LV2Plugin::end_run ()
{}*/



static LV2_Handle instantiate (const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features)
{
	// New instance
	LV2Plugin* instance;
	try {instance = new LV2Plugin (samplerate, bundle_path, features);}
	catch (std::exception& exc)
	{
		fprintf (stderr, "Plugin instantiation failed. %s\n", exc.what ());
		return NULL;
	}

	return (LV2_Handle)instance;
}

static void connect_port (LV2_Handle instance, uint32_t port, void *data)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (inst) inst->connect_port (port, data);
}

static void activate (LV2_Handle instance)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (inst) inst->activate ();
}

static void run (LV2_Handle instance, uint32_t n_samples)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (inst) inst->run (n_samples);
}

static void deactivate (LV2_Handle instance)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (inst) inst->deactivate ();
}

/*static LV2_State_Status state_save (LV2_Handle instance, LV2_State_Store_Function store, LV2_State_Handle handle, uint32_t flags,
           const LV2_Feature* const* features)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (!inst) return LV2_STATE_SUCCESS;

	return inst->state_save (store, handle, flags, features);
}*/

/*static LV2_State_Status state_restore (LV2_Handle instance, LV2_State_Retrieve_Function retrieve, LV2_State_Handle handle, uint32_t flags,
           const LV2_Feature* const* features)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (!inst) return LV2_STATE_SUCCESS;

	return inst->state_restore (retrieve, handle, flags, features);
}*/

/*static LV2_Worker_Status work (LV2_Handle instance, LV2_Worker_Respond_Function respond, LV2_Worker_Respond_Handle handle,
	uint32_t size, const void* data)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (!inst) return LV2_WORKER_SUCCESS;

	return inst->work (respond, handle, size, data);
}*/

/*static LV2_Worker_Status work_response (LV2_Handle instance, uint32_t size,  const void* data)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (!inst) return LV2_WORKER_SUCCESS;

	return inst->work_response (size, data);
}*/

/*static LV2_Worker_Status end_run (LV2_Handle instance)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (!inst) return LV2_WORKER_SUCCESS;

	return inst->end_run ();
}*/

static void cleanup (LV2_Handle instance)
{
	LV2Plugin* inst = static_cast<LV2Plugin*>(instance);
	if (inst) delete inst;
}

static const void* extension_data (const char* uri)
{
	// State
	//static const LV2_State_Interface state  = {state_save, state_restore};
	//if (!strcmp(uri, LV2_STATE__interface)) return &state;

	// Worker
	//static const LV2_Worker_Interface worker = {work, work_response, end_run};
	//if (!strcmp(uri, LV2_WORKER__interface)) return &worker;

	return NULL;
}

static const LV2_Descriptor descriptor =
{
		LV2PLUGIN_URI,
		instantiate,
		connect_port,
		activate,
		run,
		deactivate,
		cleanup,
		extension_data
};

// LV2 Symbol Export
LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
	{
		case 0:		return &descriptor;
		default:	return NULL;
	}
}
