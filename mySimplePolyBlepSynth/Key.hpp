#ifndef KEY_HPP_
#define KEY_HPP_

#include <cstdint>
#include <random>
#include <ctime>
#include "KeyStatus.hpp"
#include "Waveform.hpp"
#include "Envelope.hpp"
#include "LinearFader.hpp"

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
    Key ();
    Key (const double rt);
    void press (const Waveform wf, const uint8_t nt, const uint8_t vel, const Envelope env);
    void release ();
    void release (const uint8_t nt, const uint8_t vel);
    void off ();
    void mute ();
    float get ();
    void proceed ();
    bool isOn ();

private:
    float adsr ();
    float polyBlep (const double ptrans);
    double transitionSize ();
    float integrateSquareWave (const double p);
    float integrateF1 (const double p);
    float synth ();
};

inline Key::Key () :
    Key (48000)
{

}

inline Key::Key (const double rt) :
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

inline void Key::press (const Waveform wf, const uint8_t nt, const uint8_t vel, const Envelope env)
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

inline void Key::release ()
{
    release (note, velocity);
}

inline void Key::release (const uint8_t nt, const uint8_t vel)
{
    if ((status == KEY_PRESSED) && (note == nt))
    {
        start_level = adsr ();
        time = 0.0;
        status = KEY_RELEASED;
    }
}

inline void Key::off ()
{
    position = 0.0;
    status = KEY_OFF;
}

inline void Key::mute ()
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

inline float Key::polyBlep (const double ptrans)
{
    return
    (
        (ptrans <= -1.0) || (ptrans >= 1.0) ?
        0.0 :
        (
            ptrans <= 0.0 ?
            pow (ptrans + 1.0, 2.0) :
            -pow (ptrans - 1.0, 2.0)
        )
    );
}

inline double Key::transitionSize ()
{
    return 2.0 /* Number of samples for each side */ / (rate / freq);
}

inline float Key::integrateF1 (const double p)
{
    return -pow (p, 3.0) / 3.0 + pow (p, 2.0);
}

inline float Key::integrateSquareWave (const double p)
{
    float value = 0.0f;
    const double transition = transitionSize();
    double prest = p;

    if (p <= transition) value += integrateF1(p / transition) * transition;
    else
    {
        value += integrateF1(1.0) * transition;
        prest -= transition;

        if (p <= 0.5 - transition) value += prest;
        else
        {
            value += 0.5 - 2.0 * transition;
            prest -= 0.5 - 2.0 * transition;

            if (p <= 0.5) value += (integrateF1(1.0) - integrateF1(1.0 - prest /transition)) * transition;
            else
            {
                value += integrateF1(1.0) * transition;
                prest -= transition;
                value -= integrateSquareWave(prest);
            }
        }
    }

    return value;
}

inline float Key::synth()
{
    const float p = fmod (position, 1.0);

    switch (waveform) 
    {
        case WAVEFORM_SINE:     return sin (2.0 * M_PI * position);  
        case WAVEFORM_TRIANGLE: return 4.0 * integrateSquareWave(fmod (position + 0.25, 1.0)) - 1.0;
        case WAVEFORM_SQUARE:   return (p < 0.5f ? 1.0f : -1.0f) +
                                       polyBlep((fmod (position + 0.5, 1.0) - 0.5) / transitionSize()) -
                                       polyBlep((p - 0.5) / transitionSize());
        case WAVEFORM_SAW:      return 2.0f * p - 1.0f -
                                       polyBlep((fmod (position + 0.5, 1.0) - 0.5) / transitionSize());
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

inline bool Key::isOn ()
{
    return (status != KEY_OFF);
}

#endif /* KEY_HPP_ */