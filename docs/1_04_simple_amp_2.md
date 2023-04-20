# 04 - A Simple Amplifier Plugin (Part 2) - Code, Compile & Run

See video: <https://youtu.be/XSxZ4_SdqIQ>

Coding in C. Basic introduction into C programming and what we need to make LV2 plugins:

* a class definition using `typedef struct`,
* the seven core functions called:
    * instantiate,
    * connect_port,
    * activate,
    * run,
    * deactivate,
    * cleanup, and
    * extension_data
* a descriptor struct which contains:
    * the plugin URI as a C string and
    * the pointers to the seven core functions (or `NULL` if not needed), and
* a C interface function which returns a pointer to the descriptor struct.

Compile the code from the code directory with:

```sh
gcc -fvisibility=hidden -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread `pkg-config --cflags lv2` -lm `pkg-config --libs lv2` myAmp.c -o myAmp.so
```

Copy the resulting .so file and the two .ttl files into a newly created subfolder (I call this folder `myAmp.lv2/`)
in one of your systems LV2 folders. Like the hidden LV2 folder of your home directory: `~/.lv2/`.

Then you can test the plugin in an LV2 host, like the minimalistic [jalv](https://gitlab.com/drobilla/jalv):

```sh
jalv.gtk3 https://github.com/sjaehn/lv2tutorial/myAmp
```

(gtk3 may be replaced by one of the other jalv variants). Or feel free to directly test myAmp within a DAW.

Now we completed our first LV2 plugin!
