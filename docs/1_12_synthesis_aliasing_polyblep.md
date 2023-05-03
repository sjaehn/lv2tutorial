# 12 - Synthesis, Aliasing & PolyBLEP

See video: scheduled for May 05, 2023, 14:00 CET

Get rid of nasty aliasing artifacts from sound synthesis. Especially for saw
and square waves. This video describes the cause for aliasing in audio and
how to fix it.

Warning: This video may contain traces of mathematics.

## Topics:
### The problem
* Distorted sound for saw and square (and triangle) wave in
  mySimplePolySynth
* Artifacts in the spectrogram

### The cause
* Aliasing
* Nyquist frequency

### The fix
* Fouries series synthesis
* Oversampling
* BLIT, BLEP, PolyBLEP
    * PolyBLEP (Polynomial BandLimited stEP)
        * Replace steps by a simple quadratic function
        * Replace kinks by integration of PolyBLEP

### The implementation
* Start with mySimplePolySynth
* Adapt the filenames and the URIs
* Add a `Key::polyBlep()` method
* Square wave:
    * Add polyBLEP at phase position 0
    * Subtract polyBLEP at phase position 0.5
* Saw wave:
    * Subtract polyBLEP at phase position 0
* Triangle wave:
    * Add a `Key::integrateSquareWave()` method
        * Divide the square wave into six steps
        * Step 1: Integrate polyBLEP using `Key::IntegrateF1()`
        * Step 2: Integration is a simple rectangle
        * Step 3: Reversed step 1
        * Steps 4-6: Minus steps 1-3
    * Replace triangle formula by `Key::integrateSquareWave()`
    * Scale and shift the triangle wave

## Compile:
```sh
g++ mySimplePolyBlepSynth.cpp -fvisibility=hidden -fPIC -DPIC -O3 -shared -pthread `pkg-config --cflags lv2` -Wl,-Bstatic `pkg-config --libs --static lv2` -Wl,-Bdynamic -lm -o mySimplePolyBlepSynth.so
```

## Test:
* Create a new folder inside your LV2 directory
* Copy all .so and .ttl files
* And run:

```sh
jalv.gtk3 https://github.com/sjaehn/lv2tutorial/mySimplePolyBlepSynth
```

