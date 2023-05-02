# 02 - A Simple Plugin UI Using GTK (Part 1)

See video: <https://youtu.be/hSnMVSIBfww>

Note: This video was made for teaching and demonstration of LV2UI. The use of
GTK and Qt within plugins is not recommended.

Create our first own graphical LV2 plugin user interface. First, we define our
environment for programming LV2 UIs by:

* add the UI metadata to the manifest (and a linked plugin turtle file)
* create an UI binary file with
    * a public C interface function for the LV2UI (export function)
    * a static/private interface struct containing
        * the UI URI
        * function pointers to the four UI functions
    * the four static/private UI functions
        * instantiate
        * cleanup
        * port_event
        * extension_data
    * an UI class definition.

This "boilerplate" can be used for all [GTK3]-based LV2 plugins. And with some
minor adaptions for any LV2 plugin.


[GTK3]: https://docs.gtk.org/gtk3/
