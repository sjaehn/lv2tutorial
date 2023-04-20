# 11 - Programming Realtime Audio

See video: <https://youtu.be/h_UvVOu12aQ>

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
