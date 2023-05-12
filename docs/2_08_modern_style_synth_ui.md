# 08 - Make A Modern Style Synth UI

See video: Friday May 12, 2023 14:00 CET

Create a (just a bit more complex) graphical user interface for
myPolyBlepSynth with dials, sliders, a combobox and an image surface to draw
on it. Using [B.Widgets](https://github.com/sjaehn/BWidgets).

## Topics

### Get started

* Start with myPolyBlepSynth
* Adapt the names and the URIs
* Export `ControlPorts`, `controlLimits`, and `PortGroups` to seperate .hpp
files
* Get the UI files from myAmp_BWidgets
* Adapt the names and the URIs, too
* Get B.Widgets from <https://github.com/sjaehn/BWidgets> using git:

```sh
git submodule add https://github.com/sjaehn/BWidgets.git myAmp_BWidgets/BWidgets
git submodule update --init --recursive
```

* Build B.Widgets

```sh
cd BWidgets
make bwidgets
```

### UI header file

* Add:
    * `BWidgets::ComboBox` for  waveform
    * `BWidgets::ValueHSlider` for attack, decay, sustain, and release
    * `BWidgets::ValueDial` for level
    * `BWidgets::Image` for display
    * Store pointers to all controller widgets in a `std::array widgets`
    * Add method `drawWaveform()`

### UI cpp file

* Constructor
    * Initialize parent class `BWidgets::Window`
    * Initialize all widgets
    * Add all controller widgets to `widgets`
    * Iterate through `widgets` and
        * `setClickable(false)` (except for `waveform`)
        * `setFgColors()` (except for `waveform`)
        * `setCallbackfunction()`
        * `add()`
    * `createImage()`
    * `drawWaveform()`
    * `setBackground()`
* `portEvent()`
    * Get controller index from port index
    * Set the controller values using `setValue()`
* `valueChangedCallback()`
    * Find the event-emitting widget by iteration
        * Get the controller index
        * `getValue()`
    * Pass the controller value to the host using LV2 `write_function()` and
    the respective port index
* `drawWaveform()`
    * Get access to the image surface by `getImageSurface()`
    * Draw to this surface using Cairo
    * Update widget using `update()`

## Compile (and link)

### DSP

```sh
gcc mySPBSynth.c -fvisibility=hidden -O3 -fPIC -DPIC -shared -pthread `pkg-config --cflags lv2` -Wl,-Bstatic `pkg-config --libs --static lv2` -Wl,-Bdynamic -lm -o mySPBSynth.so
```

### UI

```sh
g++ -fPIC -DPIC -DPUGL_HAVE_CAIRO -fvisibility=hidden -std=c++17 -IBWidgets/include mySPBSynth_ui.cpp `pkg-config --cflags lv2 cairo x11` -c
g++ -shared -pthread -LBWidgets/build mySPBSynth_ui.o -lbwidgetscore -lcairoplus -lpugl `pkg-config --libs lv2 cairo x11` -o mySPBSynth_ui.so
```

## Test

* Create a new folder inside your LV2 directory
* Copy all .so, .ttl, and .png files
* And run

```sh
jalv.gtk3 https://github.com/sjaehn/lv2tutorial/mySPBSynth
```
