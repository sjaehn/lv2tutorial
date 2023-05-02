# 05 - Make A Simple Amp Plugin UI With XPutty

See video: <https://youtu.be/no9pBAi-F1c>

We create an X11, [Cairo]-based graphical user interface with [Xputty].

## Topics

### New requirements for X11

* Declare as an X11 user interface: `ui:X11UI`
* Information about the host-provided parent window
* Interface for the to call the plugin UI event loops: idle interface
    * `lv2:requiredFeature ui:idleInterface`
    * `lv2:extensionData ui:idleInterface`
* Destructor to cleanup / deallocate plugin UI widgets

### Construct the UI

* Init Xputty:`main_init ()`
* Create a top level widget: `create_window ()`
* Create a dial widget inside the top level widget: `add_knob ()`
* Link the dial to valueChangedCallback
* Add values and a value range: `set_adjustment ()`
* Visualize: `widget_show_all ()`

### Set / Get dial

* Set dial value via portEvent: `adj_set_value ()`
* Get dial value in valueChangedCallback: `adj_get_value ()`

### Idle interface

* Provide a static idle interface function
* Call `run_embedded ()`
* Link idle interface function in extension data


[Xputty]: https://github.com/brummer10/libxputty
[Cairo]:  https://www.cairographics.org/manual/
