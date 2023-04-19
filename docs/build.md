# Build the projects

## XPutty

[XPutty][1] is a small UI library useful to provide an UI to plugins on Linux.

Currently we only provided a [source package in AUR][2], the Archlinux User Repository,
for convenience, so in order to build all plugins, included `myAmp_Xputty`,
it is required to build it using `make`.

The library can be built from a directory in this repository:

```sh
cd myAmp_Xputty/libxputty
make
cd ../..
```

Now it's possible to build all projects at once in the next step.

## CMake

All projects can be built separately or at once with [CMake][3].
A `CMakeLists.txt` build configuration file is provided in the root directory.

To build all projects with it:

```sh
# - Configure a `Release` version of the project in a `build` directory by skipping
#   developers' message warnings
# - Build the project by showing compiler message options

cmake -DCMAKE_BUILD_TYPE=Release -B build -Wno-dev
cmake --build build --verbose
```

Getting a `Package 'xputty', required by 'virtual:world', not found` is normal
if you don't have it installed in your system.


[1]: https://github.com/brummer10/libxputty/
[2]: https://aur.archlinux.org/pkgbase/xputty-git
[3]: https://cmake.org/cmake/help/latest/
