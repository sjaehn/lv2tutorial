#ifndef MYAMP_XPUTTY_HPP
#define MYAMP_XPUTTY_HPP

#include <string>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/BEvents/Event.hpp"
#include "BWidgets/BWidgets/Window.hpp"
#include "BWidgets/BWidgets/ValueDial.hpp"
#include "BWidgets/BWidgets/ComboBox.hpp"
#include "BWidgets/BWidgets/ValueHSlider.hpp"
#include "ControlPorts.hpp"
#include "BWidgets/BWidgets/Image.hpp"

class MySPBSynthUI : public BWidgets::Window
{
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    BWidgets::ComboBox waveform;
    BWidgets::ValueHSlider attack;
    BWidgets::ValueHSlider decay;
    BWidgets::ValueHSlider sustain;
    BWidgets::ValueHSlider release;
    BWidgets::ValueDial level;
    std::array<BWidgets::Widget*, CONTROL_NR> widgets;
    BWidgets::Image display;


public:
    MySPBSynthUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, std::string bundlePath);
    LV2UI_Widget getTopLevelWidget();
    void portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer);
    static void valueChangedCallback (BEvents::Event* event);
    void drawWaveform();
};

#endif
