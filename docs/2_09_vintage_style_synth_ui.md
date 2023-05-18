# 09 - Make A Vintage Style Synth UI

See video: Friday May 19, 2023 14:00 CET

Create a vintage style graphical user interface based on image data for
myPolyBlepSynth by modifying mySPBSynth. Using
[B.Widgets](https://github.com/sjaehn/BWidgets). Also add a piano roll,
generate MIDI signals and send them to the plugin DSP. And make the user
interface resizable.

## Get started

### Duplicate mySPBSynth

* Start with mySPBSynth
* Adapt the names and the URIs, get the B.Widgets toolkit as described before

### Create a background image

* Take wood, steel, and SCREWS

### UI header file

* Use:
    * `BWidgets::ComboBox` for  waveform
    * `BWidgets::ImageHSlider` for attack, decay, sustain, and release
    * `BWidgets::ImageDial` for level
    * `BWidgets::Image` for display

### UI cpp file

* Initialize ImageHSliders and ImageDials with images taken from
BWidgets/examples/inc
    * Static: background
    * Active: displayed clipped depending on value
    * Dynamic: displayed moved depending on value
* Image anchor points taken from BWidgets/examples/widgetgallery.cpp
    * Static: gravity points for start and end
    * Active: gravity point for start
    * Dynamic: gravity point for center

### Compile, link, install, and run

* DSP

    ```sh
    gcc mySPBSynth_vintage.c -fvisibility=hidden -O3 -fPIC -DPIC -shared -pthread `pkg-config --cflags lv2` -Wl,-Bstatic `pkg-config --libs --static lv2` -Wl,-Bdynamic -lm -o mySPBSynth_vintage.so
    ```

* UI

    ```sh
    g++ -fPIC -DPIC -DPUGL_HAVE_CAIRO -fvisibility=hidden -std=c++17 -IBWidgets/include mySPBSynth_vintage_ui.cpp `pkg-config --cflags lv2 cairo x11` -c
    g++ -shared -pthread -LBWidgets/build mySPBSynth_vintage_ui.o -lbwidgetscore -lcairoplus -lpugl `pkg-config --libs lv2 cairo x11` -o mySPBSynth_vintage_ui.so
    ```

* install

    ```sh
    mkdir path/to/your/lv2folder/mySPBSynth_vintage.lv2
    cp *.so *.ttl *.png path/to/your/lv2folder/mySPBSynth_vintage.lv2/
    ```

* run

    ```sh
    jalv.gtk3 https://github.com/sjaehn/lv2tutorial/mySPBSynth
    ```

## Piano

### Add widgets

* Add a `BWidgets::HPianoRoll` for piano and `BWidgets::ImageVSlider` for velocity
* Link them to their respective valueChangedCallbacks

### Create and send LV2 MIDI atoms (pianoChangedCallback)

* Use LV2_Atom_Int
* Send it as an `ui_midiEvent` via `atom_EventTransfer`

### Define URI(D)s

* Export `struct Urids` from DSP
* Extend it by `ui_midiEvent` and `atom_EventTransfer`
* implement an `init()` method which converts URIs to URIDs using a host-provided map function
* Receive map from features

### Receive LV2 MIDI atoms

* In plugin DSP `run()` method
* Handle `ui_midiEvent` like `midi_midiEvent`

### Change velocity (velocityChangedCallback)

* `setVelocity()`

### Compile, link, install, and run

* As described above

## Resize

### onConfigureRequest

* Override the `BWidgets::Window::onConfigureRequest` method
* Get the dragged window area and calculate the zoom factor
* `setZoom()`

### Compile, link, install, and run

* As described above
