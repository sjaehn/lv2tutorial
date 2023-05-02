# 06 - Make A Simple Amp Plugin UI With XPutty II

See video: <https://youtu.be/lXYyu7Ta7C4>

We compile and optimize the [Xputty]-based plugin we made before.

## Compile

### Plugin DSP

```sh
gcc myAmp.c -fvisibility=hidden -fPIC -DPIC -shared -pthread `pkg-config --cflags lv2` -Wl,-Bstatic -lm `pkg-config --libs --static lv2` -Wl,-Bdynamic -o myAmp.so
```

### User interface

```sh
g++ myAmp_Xputty.cpp -fvisibility=hidden -fPIC -DPIC -shared -pthread -Ilibxputty/libxputty/include `pkg-config --cflags lv2 cairo x11` -Llibxputty/libxputty -Wl,-Bstatic -lm `pkg-config --libs --static lv2` -Wl,-Bdynamic `pkg-config --libs cairo x11` -o myAmp_Xputty.so
```

### Optimization

* Load background image: `widget_get_png_from_file ()`
* Visualize it in an expose event callback:
    * `cairo_set_source_surface ()`
    * `cairo_paint ()`


[Xputty]: https://github.com/brummer10/libxputty
