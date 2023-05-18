/* include libs */
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <new>
#include <array>
#include <utility>
#include "BMap.hpp"
#include "lv2.h"
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>
#include <lv2/midi/midi.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/util.h>
#include "Key.hpp"
#include "Limit.hpp"
#include "ControlPorts.hpp"
#include "ControlLimits.hpp"
#include "PortGroups.hpp"
#include "Urids.hpp"

/* class definition */
class MySimplePolySynth
{
private:
    const LV2_Atom_Sequence* midi_in_ptr ;
    float* audio_out_ptr;
    std::array<const float*, CONTROL_NR> control_ptr;
    std::array<float, CONTROL_NR> control;
    LinearFader<float> controlLevel;
    LV2_URID_Map* map ;
    Urids urids;
    double rate;
    double position;
    BUtilities::BMap<uint8_t, Key, 128> key;

public:
    MySimplePolySynth (const double sample_rate, const LV2_Feature *const *features);
    void connectPort (const uint32_t port, void* data_location);
    void run (const uint32_t sample_count);

private:
    void play (const uint32_t start, const uint32_t end);
};

MySimplePolySynth::MySimplePolySynth (const double sample_rate, const LV2_Feature *const *features) :
    midi_in_ptr (nullptr),
    audio_out_ptr (nullptr),
    controlLevel (0.0f),
    map (nullptr),
    rate (sample_rate),
    position (0.0),
    key ()
{
    control_ptr.fill (nullptr);
    control.fill (0.0f);

    const char* missing = lv2_features_query
    (
        features,
        LV2_URID__map, &map, true,
        NULL
    );

    if (missing) throw std::invalid_argument ("Feature map not provided by the host. Can't instantiate mySPBSynth_vintage");

    urids.init (map);
}

void MySimplePolySynth::connectPort (const uint32_t port, void* data_location)
{
    switch (port)
    {
    case PORT_MIDI_IN:
        midi_in_ptr = static_cast<const LV2_Atom_Sequence*> (data_location);
        break;

    case PORT_AUDIO_OUT:
        audio_out_ptr = static_cast<float*> (data_location);
        break;

    default:
        if (port < PORT_CONTROL + CONTROL_NR)
        {
            control_ptr[port - PORT_CONTROL] = static_cast<const float*> (data_location);
        }
        break;
    }
}

void MySimplePolySynth::play (const uint32_t start, const uint32_t end)
{
    for (uint32_t i = start; i < end; ++i)
    {
        float out = 0.0f;
        for (BUtilities::BMap<uint8_t, Key, 128>::iterator it = key.begin(); it < key.end (); /* empty */)
        {
            BUtilities::BMap<uint8_t, Key, 128>::reference k = *it;
            if (k.second.isOn())
            {
                out += k.second.get() * controlLevel.get();
                k.second.proceed();
                ++it;
            }

            else it = key.erase(it);
        }
        audio_out_ptr[i] = out;
        controlLevel.proceed();
    }
}

void MySimplePolySynth::run (const uint32_t sample_count)
{
    /* check if all ports connected */
    if ((!audio_out_ptr) || (!midi_in_ptr)) return;

    for (int i = 0; i < CONTROL_NR; ++i)
    {
        if (!control_ptr[i]) return;
    }

    /* copy and validate control port values */
    for (int i = 0; i < CONTROL_NR; ++i)
    {
        if (*control_ptr[i] != control[i])
        {
            control[i] = limit<float> (*control_ptr[i], controlLimits[i].first, controlLimits[i].second);
            if (i == CONTROL_LEVEL) controlLevel.set (control[i], 0.01 * rate);
        }
    }

    /* analyze incomming MIDI data */
    uint32_t last_frame = 0;
    LV2_ATOM_SEQUENCE_FOREACH (midi_in_ptr, ev)
    {
        /* play frames until event */
        const uint32_t frame = ev->time.frames;
        play (last_frame, frame);
        last_frame = frame;

        if ((ev->body.type == urids.midi_MidiEvent) || (ev->body.type == urids.ui_MidiEvent))
        {
            const uint8_t* const msg = reinterpret_cast<const uint8_t*> (ev + 1);
            const uint8_t typ = lv2_midi_message_type (msg);

            switch (typ)
            {
            case LV2_MIDI_MSG_NOTE_ON:
                key[msg[1] & 0x7f].press
                (
                    static_cast<Waveform> (control[CONTROL_WAVEFORM]),
                    msg[1] /* note */,
                    msg[2] /* velocity */,
                    {
                        control[CONTROL_ATTACK],
                        control[CONTROL_DECAY],
                        control[CONTROL_SUSTAIN],
                        control[CONTROL_RELEASE]
                    }
                );
                break;

            case LV2_MIDI_MSG_NOTE_OFF:
                key[msg[1] & 0x7f].release (msg[1], msg[2]);
                break;

            case LV2_MIDI_MSG_CONTROLLER:
                if (msg[1] == LV2_MIDI_CTL_ALL_NOTES_OFF)
                {
                    for (BUtilities::BMap<uint8_t, Key, 128>::reference k : key) k.second.release();
                }
                else if (msg[1] == LV2_MIDI_CTL_ALL_SOUNDS_OFF)
                {
                    for (BUtilities::BMap<uint8_t, Key, 128>::reference k : key) k.second.mute();
                }
                break;

            default:
                break;
            }
        }
    }

    /* play remaining frames */
    play (last_frame, sample_count);
}

/* internal core methods */
static LV2_Handle instantiate (const struct LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    MySimplePolySynth* m = nullptr;
    try
    {
        m = new MySimplePolySynth (sample_rate, features);
    }
    catch (const std::invalid_argument& ia)
    {
        std::cerr << ia.what() << std::endl;
        return nullptr;
    }
    catch (const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate memory. Can't instantiate mySimplePolySynth" << std::endl;
        return nullptr;
    }
    return m;
}

static void connect_port (LV2_Handle instance, uint32_t port, void *data_location)
{
    MySimplePolySynth* m = static_cast <MySimplePolySynth*> (instance);
    if (m) m->connectPort (port, data_location);
}

static void activate (LV2_Handle instance)
{
    /* not needed here */
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    MySimplePolySynth* m = static_cast <MySimplePolySynth*> (instance);
    if (m) m->run (sample_count);
}

static void deactivate (LV2_Handle instance)
{
    /* not needed here */
}

static void cleanup (LV2_Handle instance)
{
    MySimplePolySynth* m = static_cast <MySimplePolySynth*> (instance);
    if (m) delete m;
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/sjaehn/lv2tutorial/mySPBSynth_vintage",
    instantiate,
    connect_port,
    activate,
    run,
    deactivate /* or NULL */,
    cleanup,
    extension_data /* or NULL */
};

/* interface */
LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    if (index == 0) return &descriptor;
    else return NULL;
}
