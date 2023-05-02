# 03 - How LV2 Plugins Work

See video: <https://www.youtube.com/watch?v=Zg-YUbfO6ww>

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
