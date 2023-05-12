# 07 - Make An Amp Plugin UI With B.Widgets

See video: <https://youtu.be/1MP2Y1g6KCY>

Introduction into a new widget tookit designed for graphical user interfaces
for music production: [B.Widgets](https://github.com/sjaehn/BWidgets). And
create an UI for myAmp with this toolkit.

## Topics

### Get started

* Start with myAmp_Xputty
* Adapt the names and the URIs
* Get B.Widgets from <https://github.com/sjaehn/BWidgets> using git:

```sh
git submodule add https://github.com/sjaehn/BWidgets.git myAmp_BWidgets/BWidgets
git submodule update --init --recursive
```

### Build BWidgets

```sh
cd BWidgets
make bwidgets
```

### UI header file

* Inherit the MyAmpUI class from `BWidgets::Window`
* Declare:
    * `BWidgets::ValueDial dial`
    * Constructor
    * (Destructor)
    * `getTopLevelWidget()`
    * `portEvent()`
    * `valueChangedCallback()`

### UI cpp file

* Constructor
    * Initialize parent class `BWidgets::Window`
    * Initialize `dial`
    * `setBackground()`
    * dial
        * `add()`
        * `setClickable()`
        * `setCallbackfunction()`
    * `getTopLevelWidget()`
        * Get widget from the pugl backend of B.Widgets using
          `puglGetNativeView(getView())`
* `portEvent()`
    * Set the dial value using `setValue()`
* `valueChangedCallback()`
    * Get the event-emitting widget from the passed `BWidgets::Event* event`
      by `event->getWidget()`
    * Get the main window from this widget using `getMainWindow()`
    * Get the dial value using `getValue()`
    * Pass the dial value to the host using LV2 `write_function()`
* Idle interface (`ui_idle()`)
    * Call the `BWidgets::Window` event handler using `handleEvents()`

## Compile (and link)

### DSP

```sh
gcc myAmp.c -fvisibility=hidden -fPIC -DPIC -shared -pthread `pkg-config --cflags lv2` -Wl,-Bstatic `pkg-config --libs --static lv2` -Wl,-Bdynamic -lm -o myAmp.so
```

### UI

```sh
g++ -fPIC -DPIC -DPUGL_HAVE_CAIRO -fvisibility=hidden -std=c++17 -IBWidgets/include myAmp_BWidgets.cpp `pkg-config --cflags lv2 cairo x11` -c
g++ -shared -pthread -LBWidgets/build myAmp_BWidgets.o -lbwidgetscore -lcairoplus -lpugl `pkg-config --libs lv2 cairo x11` -o myAmp_BWidgets.so
```

## Test

* Create a new folder inside your LV2 directory
* Copy all .so, .ttl, and .png files
* And run

```sh
jalv.gtk3 https://github.com/sjaehn/lv2tutorial/myAmp_BWidgets
```
