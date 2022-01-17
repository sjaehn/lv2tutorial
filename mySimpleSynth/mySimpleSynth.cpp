/* include libs */
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <new>
#include <array>
#include <random>
#include <ctime>
#include <utility>
#include "lv2.h"
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>
#include <lv2/midi/midi.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/util.h>

template <class T>
class LinearFader
{
private:
    T destination_;
    uint32_t distance_;
    T value_;

public:
    LinearFader (const T destination) :
        destination_ (destination),
        distance_ (0),
        value_ (destination)
    {

    }

    void set (const T destination, const uint32_t distance)
    {
        destination_ = destination;
        distance_ = distance;
        if (distance == 0) value_ = destination;
    }

    T get () const
    {
        return value_;
    }

    void proceed ()
    {
        if (distance_ == 0) value_ = destination_;
        else
        {
            value_ += (destination_ - value_) * (1.0 / static_cast<double> (distance_));
            distance_ -= 1;
        }
    }
};

template <class T>
T limit (const T x, const T min, const T max)
{
    return (x < min ? min : (x > max ? max : x));
}

enum ControlPorts
{
    CONTROL_WAVEFORM= 0,
    CONTROL_ATTACK  = 1,
    CONTROL_DECAY   = 2,
    CONTROL_SUSTAIN = 3,
    CONTROL_RELEASE = 4,
    CONTROL_LEVEL   = 5,
    CONTROL_NR      = 6
};

constexpr std::array<std::pair<float, float>, CONTROL_NR> controlLimits =
{{
    {0.0f, 4.0f},
    {0.001f, 4.0f},
    {0.001f, 4.0f},
    {0.0f, 1.0f},
    {0.001f, 4.0f},
    {0.0f, 1.0f}
}};

enum PortGroups
{
    PORT_MIDI_IN    = 0,
    PORT_AUDIO_OUT  = 1,
    PORT_CONTROL    = 2,
    PORT_NR         = 3
};

enum Waveform
{
    WAVEFORM_SINE       = 0,
    WAVEFORM_TRIANGLE   = 1,
    WAVEFORM_SQUARE     = 2,
    WAVEFORM_SAW        = 3,
    WAVEFORM_NOISE      = 4,
    WAVEFORM_NR         = 5
};

struct Urids
{
    LV2_URID midi_MidiEvent;
};

enum KeyStatus
{
    KEY_OFF,
    KEY_PRESSED,
    KEY_RELEASED
};

struct Envelope
{
    double attack;
    double decay;
    float sustain;
    double release;
};

class Key
{
private:
    KeyStatus status;
    Waveform waveform;
    uint8_t note;
    uint8_t velocity;
    Envelope envelope;
    double rate;
    double position;
    float start_level;
    double freq;
    double time;
    LinearFader<float> fader;
    std::minstd_rand rnd;
    std::uniform_real_distribution<float> dist;

public:
    Key (const double rt);
    void press (const Waveform wf, const uint8_t nt, const uint8_t vel, const Envelope env);
    void release ();
    void release (const uint8_t nt, const uint8_t vel);
    void off ();
    void mute ();
    float get ();
    void proceed ();

private:
    float adsr ();
    float synth ();
};

Key::Key (const double rt) :
    status (KEY_OFF),
    waveform (WAVEFORM_SINE),
    note (0),
    velocity (0),
    envelope {0.0, 0.0, 0.0f, 0.0},
    rate (rt),
    position (0.0),
    start_level (0.0f),
    freq (pow (2.0, (static_cast<double> (note) - 69.0) / 12.0) * 440.0),
    time (0.0),
    fader (1.0f),
    rnd (std::time (0)),
    dist (-1.0f, 1.0f)
{

}

void Key::press (const Waveform wf, const uint8_t nt, const uint8_t vel, const Envelope env)
{
    start_level = adsr();
    note = nt;
    velocity = vel;
    envelope = env;
    freq = pow (2.0, (static_cast<double> (note) - 69.0) / 12.0) * 440.0;
    time = 0.0;
    fader.set (1.0f, 0.0);
    waveform = wf;
    status = KEY_PRESSED;
}

void Key::release ()
{
    release (note, velocity);
}

void Key::release (const uint8_t nt, const uint8_t vel)
{
    if ((status == KEY_PRESSED) && (note == nt))
    {
        start_level = adsr ();
        time = 0.0;
        status = KEY_RELEASED;
    }
}

void Key::off ()
{
    position = 0.0;
    status = KEY_OFF;
}

void Key::mute ()
{
    fader.set (0.0f, 0.01 * rate);
}

inline float Key::adsr ()
{
    switch (status)
    {
    case KEY_PRESSED:
        if (time < envelope.attack)
        {
            return start_level + (1.0f - start_level) * time /envelope.attack;
        }

        if (time < envelope.attack + envelope.decay)
        {
            return 1.0f + (envelope.sustain - 1.0f) * (time - envelope.attack) / envelope.decay;
        }

        return envelope.sustain;

    case KEY_RELEASED:
        return start_level - start_level * time /envelope.release;
    
    default:
        return 0.0f;
    }
}

inline float Key::synth()
{
    const float p = fmod (position, 1.0);

    switch (waveform) 
    {
        case WAVEFORM_SINE:     return sin (2.0 * M_PI * position);  
        case WAVEFORM_TRIANGLE: return (p < 0.25f ? 4.0f * p : (p < 0.75f ? 1.0f - 4.0 * (p - 0.25f) : -1.0f + 4.0f * (p - 0.75f)));
        case WAVEFORM_SQUARE:   return (p < 0.5f ? 1.0f : -1.0f);
        case WAVEFORM_SAW:      return 2.0f * p - 1.0f;
        case WAVEFORM_NOISE:    return dist (rnd);
        default:                return 0.0f;
    }
}

inline float Key::get ()
{
    return  adsr() *
            synth () *
            (static_cast<float> (velocity) / 127.0f) *
            fader.get();
}

inline void Key::proceed ()
{
    time += 1.0 / rate;
    position += freq / rate;
    fader.proceed();
    if ((status == KEY_RELEASED) && (time >= envelope.release)) off();
}

/* class definition */
class MySimpleSynth 
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
    Key key;

public:
    MySimpleSynth (const double sample_rate, const LV2_Feature *const *features);
    void connectPort (const uint32_t port, void* data_location);
    void run (const uint32_t sample_count);

private:
    void play (const uint32_t start, const uint32_t end);
};

MySimpleSynth::MySimpleSynth (const double sample_rate, const LV2_Feature *const *features) :
    midi_in_ptr (nullptr),
    audio_out_ptr (nullptr),
    controlLevel (0.0f),
    map (nullptr),
    rate (sample_rate),
    position (0.0),
    key (rate)
{
    control_ptr.fill (nullptr);
    control.fill (0.0f);

    const char* missing = lv2_features_query
    (
        features,
        LV2_URID__map, &map, true,
        NULL
    );

    if (missing) throw std::invalid_argument ("Feature map not provided by the host. Can't instantiate mySimpleSynth");

    urids.midi_MidiEvent = map->map (map->handle, LV2_MIDI__MidiEvent);
}

void MySimpleSynth::connectPort (const uint32_t port, void* data_location)
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

void MySimpleSynth::play (const uint32_t start, const uint32_t end)
{
    for (uint32_t i = start; i < end; ++i)
    {
        audio_out_ptr[i] = key.get() * controlLevel.get();
        key.proceed();
        controlLevel.proceed();
    }
}

void MySimpleSynth::run (const uint32_t sample_count)
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

        if (ev->body.type == urids.midi_MidiEvent)
        {
            const uint8_t* const msg = reinterpret_cast<const uint8_t*> (ev + 1);
            const uint8_t typ = lv2_midi_message_type (msg);

            switch (typ)
            {
            case LV2_MIDI_MSG_NOTE_ON:
                key.press
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
                key.release (msg[1], msg[2]);
                break;

            case LV2_MIDI_MSG_CONTROLLER:
                if (msg[1] == LV2_MIDI_CTL_ALL_NOTES_OFF) key.release();
                else if (msg[1] == LV2_MIDI_CTL_ALL_SOUNDS_OFF) key.mute();
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
    MySimpleSynth* m = nullptr;
    try 
    {
        m = new MySimpleSynth (sample_rate, features);
    } 
    catch (const std::invalid_argument& ia) 
    {
        std::cerr << ia.what() << std::endl;
        return nullptr;
    }
    catch (const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate memory. Can't instantiate mySimpleSynth" << std::endl;
        return nullptr;
    }
    return m;
}

static void connect_port (LV2_Handle instance, uint32_t port, void *data_location)
{
    MySimpleSynth* m = static_cast <MySimpleSynth*> (instance);
    if (m) m->connectPort (port, data_location);
}

static void activate (LV2_Handle instance)
{
    /* not needed here */
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    MySimpleSynth* m = static_cast <MySimpleSynth*> (instance);
    if (m) m->run (sample_count);
}

static void deactivate (LV2_Handle instance)
{
    /* not needed here */
}

static void cleanup (LV2_Handle instance)
{
    MySimpleSynth* m = static_cast <MySimpleSynth*> (instance);
    if (m) delete m;
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/sjaehn/lv2tutorial/mySimpleSynth",
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