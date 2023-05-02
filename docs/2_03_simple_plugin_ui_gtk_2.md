# 03 - A Simple Plugin UI Using GTK (Part 2)

See video: <https://youtu.be/bVJ3WBRHAEg>

> **Note**
> This video was made for teaching and demonstration of LV2UI.
> The use of GTK and Qt within plugins is not recommended.

Coding the [GTK3] UI, main steps:

* create a container (box) as a top level widget (in constructor)
* create a vertical scale (slider)
* add the slider to the box
* set the slider value by values received from the plugin DSP
  (in `portEvent`)
* define a callback function (`valueChangedCallback`)
  to handle changed slider values
* link the callback function (in constructor)
* send changed slider values to the plugin DSP
  (in `valueChangedCallback`)

Build:

* plugin DSP:

```sh
gcc myAmp.c -fvisibility=hidden -fPIC -DPIC -shared -pthread -lm `pkg-config --cflags --libs lv2` -o myAmp.so
```

* UI:

```sh
g++ myAmp_GTK3.c -fvisibility=hidden -fPIC -DPIC -shared -pthread -lm `pkg-config --cflags --libs lv2 gtk+-3.0` -o myAmp_GTK3.so
```

* Copy both .so files and the two .ttl files into a subfolder
  (e.g.: _myAmp_GTK3.lv2) of your lv2 directory.


[GTK3]: https://docs.gtk.org/gtk3/
