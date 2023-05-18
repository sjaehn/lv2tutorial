#ifndef URIDS_HPP_
#define URIDS_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>

struct Urids
{
    LV2_URID midi_MidiEvent;
    LV2_URID ui_MidiEvent;
    LV2_URID atom_eventTransfer;

    void init (LV2_URID_Map* m);
};

inline void Urids::init (LV2_URID_Map* m)
{
    midi_MidiEvent = m->map(m->handle, LV2_MIDI__MidiEvent);
    ui_MidiEvent = m->map(m->handle, "https://github.com/sjaehn/lv2tutorial/mySPBSynth_vintage#ui_MidiEvennt");
    atom_eventTransfer = m->map(m->handle, LV2_ATOM__eventTransfer);
}

#endif
