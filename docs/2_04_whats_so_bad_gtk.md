# 04 - What's Sooo Bad About GTK?

See video: <https://www.youtube.com/watch?v=Mp58M9ZGeSs>

No, GTK is not bad! But the use within plugins is not recommended. And this
is also true for other big GUI toolkits. Like Qt.

Our GTK3-based Plugin UI doesn't work in Ardour. Why?

On the other hand, if we make a GTK(2)-based version using `ui:GtkUI`, and compile it by:

```sh
g++ myAmp_GTK.c -fvisibility=hidden -fPIC -DPIC -shared -pthread -lm `pkg-config --cflags --libs lv2 gtk+-2.0` -o myAmp_GTK.so
```

Now it runs in Ardour. But it fails in jalv.gtk3. Why?
Although LV2 contains specifications for GTK(2) and GTK3:

* GTK3 plugin UIs don't work in GTK2 LV2 hosts (javl.gtk, Ardour)
* GTK2 plugin UIs may fail in GTK3 LV2 hosts (jalv.gkt3, Zrythm)
* any GTK-based plugins don't work in host which don't support GTK(2)/3
  (Reaper).

The reason: linking. There are two types of linking:

* static linking (code included, at compile time)
* dynamic linking (shared objects, at runtime)

You can't dynamicly link to two different versions of the same library. And
you don't have any control about the library loaded before.

The solution: static linking (as much as possible). (Or provide UIs for all
toolkits. But I wouldn't do all the work ;-)). But GTK, Qt and other big UI
toolkits are made for dynamic linking. And they are full of compatibility
breaks. Use UI toolkits which can be staticly linked, instead.

> **Note:**
> The linking process is shown in a much simplified way.
> There are more steps than shown in this video.
> Including relocation, symbol table, symbol resolution, ... .
> But this would be far beyond this tutorial series.

And there are also different ways of loading plugins by the host.
