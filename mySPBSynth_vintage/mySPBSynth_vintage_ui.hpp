#ifndef MYAMP_XPUTTY_HPP
#define MYAMP_XPUTTY_HPP

#include <string>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/BEvents/Event.hpp"
#include "BWidgets/BWidgets/Window.hpp"
#include "BWidgets/BWidgets/ImageDial.hpp"
#include "BWidgets/BWidgets/ComboBox.hpp"
#include "BWidgets/BWidgets/ImageHSlider.hpp"
#include "ControlPorts.hpp"
#include "BWidgets/BWidgets/Image.hpp"
#include "BWidgets/BWidgets/HPianoRoll.hpp"
#include "BWidgets/BWidgets/ImageVSlider.hpp"
#include "Urids.hpp"

class MySPBSynthUI : public BWidgets::Window
{
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    LV2_URID_Map* map;
    Urids urids;
    BWidgets::ComboBox waveform;
    BWidgets::ImageHSlider attack;
    BWidgets::ImageHSlider decay;
    BWidgets::ImageHSlider sustain;
    BWidgets::ImageHSlider release;
    BWidgets::ImageDial level;
    std::array<BWidgets::Widget*, CONTROL_NR> widgets;
    BWidgets::Image display;
    BWidgets::HPianoRoll piano;
    BWidgets::ImageVSlider velocity;
    std::array<uint8_t, 128> keyVelocities;


public:
    MySPBSynthUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, LV2_URID_Map* map, std::string bundlePath);
    LV2UI_Widget getTopLevelWidget();
    void portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer);
    static void valueChangedCallback (BEvents::Event* event);
    static void pianoChangedCallback (BEvents::Event* event);
    static void velocityChangedCallback (BEvents::Event* event);
    virtual void onConfigureRequest (BEvents::Event* event) override;
    void drawWaveform();
};

#endif
