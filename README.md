# Programming LV2 Plugins From Scratch

Programming tutorial series for creating LV2 plugins using C/C++ and turtle.


## Content

- [Programming LV2 Plugins From Scratch I - Getting Started](#programming-lv2-plugins-from-scratch-i---getting-started)
  - [00 - Announce](#00---announce)
  - [01 - Introduction: What is LV2?](#01---introduction-what-is-lv2)
  - [02 - A Simple Amplifier (Part 1) - Do The Turtle](#02---a-simple-amplifier-part-1---do-the-turtle)
  - [03 - How LV2 Plugins Work](#03---how-lv2-plugins-work)
  - [04 - A Simple Amplifier Plugin (Part 2) - Code, Compile & Run](#04---a-simple-amplifier-plugin-part-2---code-compile--run)
  - [05 - A Sound Generator In C++](#05---a-sound-generator-in-c)
  - [06 - Atom, URIDs & URIs](#06---atom-urids--uris)
  - [07 - The Simplest MIDI Synth (Part 1)](#07---the-simplest-midi-synth-part-1)
  - [08 - The Simplest MIDI Synth (Part 2)](#08---the-simplest-midi-synth-part-2)
  - [09 - Waveform Synth In C++ 11](#09---waveform-synth-in-c-11)
  - [10 - Performance & Polyphony](#10---performance--polyphony)
  - [11 - Programming Realtime Audio](#11---programming-realtime-audio)

- [Programming LV2 Plugins From Scratch II - User Interfaces](#programming-lv2-plugins-from-scratch-ii---user-interfaces)
  - [00 - Announce II](#00---announce-ii)

- [Resources](#resources)
- [Further reading](#further-reading)

## Programming LV2 Plugins From Scratch I - Getting Started

### 00 - Announce

See video: https://www.youtube.com/watch?v=51eHCA4oCEI


### 01 - Introduction: What is LV2?

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


### 02 - A Simple Amplifier (Part 1) - Do The Turtle

See video: https://www.youtube.com/watch?v=aKN89LnmbvQ

Introduction to the basic principles of the Turtle description language. Subject,
predicate, and object. And URIs. Define the metadata for a simple amplifier plugin.

* [usefulinc.ttl](https://github.com/sjaehn/lv2tutorial/blob/master/usefulinc.ttl)
* [LV2](https://lv2plug.in/ns/)
* [RDF](https://www.w3.org/TR/turtle/)


### 03 - How LV2 Plugins Work

See video: https://www.youtube.com/watch?v=Zg-YUbfO6ww

This videos provides a look under the hood of an LV2 plugin and shows how it works
inside a DAW. The plugin has to provide:
* a manifest (and a linked plugin turtle file) for the plugin metadata, and
* a plugin binary file with
  * a C interface function
  * the seven core functions
    * instantiate
    * connect_port
    * activate
    * run
    * deactivate
    * cleanup
    * extension_data
  * a plugin class definition (at least) with pointers to the buffers provided by the DAW (ports).

The DAW has to provide:
* buffers for audio in, audio out, ...
* buffers for the controllers
* functions to call the plugin functions.


### 04 - A Simple Amplifier Plugin (Part 2) - Code, Compile & Run

See video: https://youtu.be/XSxZ4_SdqIQ

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
```
gcc -fvisibility=hidden -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread `pkg-config --cflags lv2` -lm `pkg-config --libs lv2` myAmp.c -o myAmp.so
```

Copy the resulting .so file and the two .ttl files into a newly created subfolder (I call this folder `myAmp.lv2/`)
in one of your systems LV2 folders. Like the hidden LV2 folder of your home directory: `~/.lv2/`.

Then you can test the plugin in an LV2 host, like the minimalistic [jalv](https://gitlab.com/drobilla/jalv):
```
jalv.gtk3 https://github.com/sjaehn/lv2tutorial/myAmp
```
(gtk3 may be replaced by one of the other jalv variants). Or feel free to directly test myAmp within a DAW.

Now we completed our first LV2 plugin!


### 05 - A Sound Generator In C++

See video: https://youtu.be/uvskHCJrJcY

Programming a test tone generator as a first step to make a software synthesizer. 

Topics:
* Introduction in classic C++.
* LV2 extensions: Units, Port Properties
* Writing a sine wave to the audio output


### 06 - Atom, URIDs & URIs

See video: https://youtu.be/EHpwO5yl66s

Introduction into the data structures used in LV2: Atoms, URIDs and URIs.

* **LV2_Atom**: Data containers with information about type and size. LV2_Atoms may have a body to store
data of different types (like int, float, double, string, ..., and even other atoms). 
  * **Vectors** are LV2_Atoms which contain an array of child data and information about the child type and the child size.
  * **Properties** are "named" LV2_Atoms which contain a key URID and data (e.g., as LV2_Atoms again).
  * **Objects** are LV2_Atoms which may take up multiple and/or different other property LV2_Atoms.
  * **Events** are prefixed with a time stamp.
  * And **Sequences** are LV2_atoms which can take up multiple events.
* **URI**: Unique identifier for each definition. 
* **URID**: Serial number for an URI.


### 07 - The Simplest MIDI Synth (Part 1)

See video: https://youtu.be/PuOeP-ln7UA

Programming a simple MIDI sine synth LV2 plugin based on our myTestTone.

Topics:
* Implementation of the LV2 extensions:
  * Atom
  * Urid
  * Midi
* Understanding LV2 MIDI atoms
* Programming a basic MIDI interpreter


### 08 - The Simplest MIDI Synth (Part 2)

See video: https://youtu.be/-mnleLiofdI

Programming a simple MIDI sine synth LV2 plugin based on our myTestTone.

Topics:
* MIDI data
* Convert MIDI notes to audio frequency
* ADSR envelope


### 09 - Waveform Synth In C++ 11

See video: https://youtu.be/5zbQLFNZRoI

Add a waveform selection feature (sine, triangle, square, saw, noise) to our 
synth. And this in modern C++. But first, we have to update our previously
written code to C++11 too.

Topics:
* Enumerations in turtle using 
  * lv2:enumeration: https://lv2plug.in/ns/lv2core#enumeration
  * lv2:integer: https://lv2plug.in/ns/lv2core#integer
  * lv2:scalePoint: https://lv2plug.in/ns/lv2core#scalePoint
* Exceptions
  * The try catch block: https://en.cppreference.com/w/cpp/language/try_catch
  * std::invalid_argument: https://en.cppreference.com/w/cpp/error/invalid_argument
  * std::bad_alloc: https://en.cppreference.com/w/cpp/memory/new/bad_alloc
* Streams
  * std::cerr: https://en.cppreference.com/w/cpp/io/cerr
* C++11 type casting
  * static_cast: https://en.cppreference.com/w/cpp/language/static_cast
  * reinterpret_cast: https://en.cppreference.com/w/cpp/language/reinterpret_cast
* Standard template library containers
  * std::array: https://en.cppreference.com/w/cpp/container/array
* constexpr: https://en.cppreference.com/w/cpp/keyword/constexpr
* Utilities
  * std::pair: https://en.cppreference.com/w/cpp/utility/pair
* Templates: https://en.cppreference.com/w/cpp/language/templates
* Linear fader
* Overloading
* Waveform mathematics
* Random generator: https://en.cppreference.com/w/cpp/numeric/random
* Value distributions


### 10 - Performance & Polyphony

See video: https://youtu.be/uelx7poEMNY

In the previous videos, we created a simple monosynth. This time, we will enable
polyphony by the use of key containers. This video also shows the features and
the limitations of the C++ standard template library (STL). And this video also
shows different ways of increasing the plugin performance.

Topics:
* Header files
* Polyphony using key as a std::array
* Alternative STL containers
  * std::vector
  * std::map
  * C++11 `for` loop
  * Using interators
  * Performance considerations
  * Realtime compatibility
* BUtility::BMap
* Compiler flags (`-O3`, `-ffast-math`)


### 11 - Programming Realtime Audio

See video: https://youtu.be/h_UvVOu12aQ

The basic concept of realtime audio we need for programming audio processing 
and sound generating plugins. Also showing the pitfalls for programming
realtime audio plugins. Last video of the first tutorial block. Will be
continued in 2022.

Topics:
* What is realtime?
* Principle of audio signal routing
* Realtime guarantee
* Violating realtime
* Not realtime-safe:
  * User interactions
  * Web access
  * Other I/O
  * Files, devices, pipes, sockets, system calls, ...
  * Heap memory allocation and deallocation
    * `malloc`, `calloc`, `free`, `new`, `delete`, ...
    * C++ strings, streams, exceptions, STL containers (except `std::array`), ...
  * Process / thread locks
* LV2 realtime thread (`connect_port`, `run`)
* Preview: LV2 worker extension


## Programming LV2 Plugins From Scratch II - User Interfaces

### 00 - Announce II

See video: https://youtu.be/7mCLDBBXajU


## Resources

* LV2 specifications: https://lv2plug.in/ns/
* LV2 book: https://lv2plug.in/book/


## Further reading
