#ifndef MYAMP_XPUTTY_HPP
#define MYAMP_XPUTTY_HPP

#include <string>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/BEvents/Event.hpp"
#include "BWidgets/BWidgets/Window.hpp"
#include "BWidgets/BWidgets/ValueDial.hpp"

class MyAmpUI : public BWidgets::Window
{
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    BWidgets::ValueDial dial;
    

public:
    MyAmpUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, std::string bundlePath);
    ~MyAmpUI();
    LV2UI_Widget getTopLevelWidget();
    void portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer);
    static void valueChangedCallback (BEvents::Event* event);
};

#endif