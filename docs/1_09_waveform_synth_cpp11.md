# 09 - Waveform Synth In C++ 11

See video: <https://youtu.be/5zbQLFNZRoI>

Add a waveform selection feature (sine, triangle, square, saw, noise) to our
synth. And this in modern C++. But first, we have to update our previously
written code to C++11 too.

## Topics

### Enumerations in turtle

* [lv2:enumeration]
* [lv2:integer]
* [lv2:scalePoint]

### Exceptions

* The [try catch] block
* [std::invalid_argument]
* [std::bad_alloc]

### Streams

* [std::cerr]

### C++11 type casting

* [static_cast]
* [reinterpret_cast]

### Standard template library containers

* [std::array]

### Utilities

* [std::pair]

### Other

* [constexpr]
* [Templates]
* Linear fader
* Overloading
* Waveform mathematics
* [Random generator]
* Value distributions


[lv2:enumeration]:       https://lv2plug.in/ns/lv2core#enumeration
[lv2:integer]:           https://lv2plug.in/ns/lv2core#integer
[lv2:scalePoint]:        https://lv2plug.in/ns/lv2core#scalePoint
[try catch]:             https://en.cppreference.com/w/cpp/language/try_catch
[std::invalid_argument]: https://en.cppreference.com/w/cpp/error/invalid_argument
[std::bad_alloc]:        https://en.cppreference.com/w/cpp/memory/new/bad_alloc
[std::cerr]:             https://en.cppreference.com/w/cpp/io/cerr
[static_cast]:           https://en.cppreference.com/w/cpp/language/static_cast
[reinterpret_cast]:      https://en.cppreference.com/w/cpp/language/reinterpret_cast
[std::array]:            https://en.cppreference.com/w/cpp/container/array
[constexpr]:             https://en.cppreference.com/w/cpp/keyword/constexpr
[std::pair]:             https://en.cppreference.com/w/cpp/utility/pair
[Templates]:             https://en.cppreference.com/w/cpp/language/templates
[Random generator]:      https://en.cppreference.com/w/cpp/numeric/random
