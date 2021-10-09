# Programming LV2 Plugins From Scratch

Programming tutorial series for creating LV2 plugins using C/C++ and turtle.


## Content

* [00 - Announce](#00---announce)
* [01 - Introduction: What is LV2? - And what do I need for plugin programming?](#01---introduction-what-is-lv2)
* [02 - A simple amplifier (part 1) - Do the turtle](#02---a-simple-amplifier-part-1---do-the-turtle)


## 00 - Announce

See video: https://www.youtube.com/watch?v=51eHCA4oCEI


## 01 - Introduction: What is LV2?

See video: https://www.youtube.com/watch?v=AdB2pLCFQY4

LV2 is a free and open source audio plugin standard. LV2 is available on Linux, Mac, Windows, and on other systems.
There are more than 1000 LV2 plugins available by now. And the number is growing.

LV2 is exensible and stores its meta-data (like plugin name, plugin type, and ports used) in separate files. This
allows to get plugin informations without loading and executing the plugin.

To create LV2 plugins, you need:

* a C/C++ compiler (gcc, Clang or MSVC),
* an editor (Atom, Eclipse, VS Code, KDevelop, or ...), 
* and some developer libraries:
  * C/C++ libraries
  * LV2
  * sndfile
  * X11 (or ...)
  * Cairo
  * pkg-config

On Debian linux-based systems you can get them by calling from the terminal:
```
sudo apt-get install build-essential
sudo apt-get install pkg-config lv2-dev libsndfile1-dev libx11-dev libcairo2-dev
```

And on Arch linux-based systems you can get them by calling from the terminal:
```
sudo pacman -S base-devel
sudo pacman -S pkg-config lv2 libsndfile libX11-dev cairo
```

Recommended optional tools:
* Jalv (minimal LV2 host)
* [lv2lint](https://github.com/OpenMusicKontrollers/lv2lint) (validation)
* gdb (debugger)
* valgrind (memory debugging / leak detection)


## 02 - A Simple Amplifier (Part 1) - Do The Turtle

See video: once established

Introduction into the basic principles of the turtle description language. Subject,
predicate, and object. And URIs. Define the metadata for a simple amplifier plugin.

* [usefulinc.ttl](https://github.com/sjaehn/lv2tutorial/blob/master/usefulinc.ttl)
* [LV2](https://lv2plug.in/ns/)
* [RDF](https://www.w3.org/TR/turtle/)


## Resources

* LV2 specifications: https://lv2plug.in/ns/
* LV2 book: https://lv2plug.in/book/


## Further reading
