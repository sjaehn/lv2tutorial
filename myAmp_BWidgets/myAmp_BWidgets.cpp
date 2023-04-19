#include <iostream>
#include "BWidgets/BEvents/Event.hpp"
#include "BWidgets/BStyles/Types/Fill.hpp"
#include "BWidgets/BWidgets/ValueDial.hpp"
#include "cairo/cairo.h"
#include "myAmp_BWidgets.hpp"
#include "pugl/pugl.h"
#include <stdexcept>

MyAmpUI::MyAmpUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, std::string bundlePath):
    Window (240, 200, reinterpret_cast<PuglNativeView>(parentXWindow), 0, "MyAmp - B.Widgets", false, PUGL_MODULE, 0),
    write_function(write_function),
    controller(controller),
    dial (78, 55, 100, 100, 1.0, 0.0, 2.0)
{
    setBackground(BStyles::Fill (bundlePath + "/Plugin.png"));
    add (&dial);
    dial.setClickable(false);
    dial.setCallbackFunction (BEvents::Event::EventType::valueChangedEvent, valueChangedCallback);
}

MyAmpUI::~MyAmpUI()
{
    
}

LV2UI_Widget MyAmpUI::getTopLevelWidget()
{
    return reinterpret_cast<LV2UI_Widget>(puglGetNativeView(getView()));
}

void MyAmpUI::portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    if (format == 0)
    {
        const float value = *static_cast<const float*>(buffer);
        switch (port_index)
        {
            case 2:     dial.setValue(value);
                        break;

            default:    break;
        }
    }
}

void MyAmpUI::valueChangedCallback (BEvents::Event* event)
{
    BWidgets::ValueDial* d = dynamic_cast<BWidgets::ValueDial*>(event->getWidget());
    MyAmpUI* ui = dynamic_cast<MyAmpUI*>(d->getMainWindow());
    if (ui)
    {
        float gain = d->getValue();
        ui->write_function (ui->controller, 2, sizeof(gain), 0, &gain);
    }
}

static LV2UI_Handle instantiate(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features)
{
    if (strcmp (plugin_uri, "https://github.com/sjaehn/lv2tutorial/myAmp_BWidgets") != 0) return nullptr;
    
    void* parentXWindow = nullptr;
    for (int i = 0; features[i]; ++i)
    {
        if (strcmp (features[i]->URI, LV2_UI__parent) == 0) parentXWindow = features[i]->data;
    }

    if (!parentXWindow)
    {
        std::cerr << "Required feature LV2_UI__parent not provided" << std::endl;
        return nullptr;
    }

    MyAmpUI* ui;
    try {ui = new MyAmpUI(write_function, controller, parentXWindow, std::string (bundle_path));}
    catch (std::exception& exc)
    {
        std::cerr << "UI instantiation failed." << std::endl;
        return nullptr;
    }

    *widget = ui->getTopLevelWidget();
    return (LV2UI_Handle) ui;
}

static void cleanup (LV2UI_Handle ui)
{
    MyAmpUI* myUi = static_cast<MyAmpUI*>(ui);
    if (myUi) delete myUi;
}

static void port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    MyAmpUI* myUi = static_cast<MyAmpUI*>(ui);
    if (myUi) myUi->portEvent(port_index, buffer_size, format, buffer);
}

static int ui_idle (LV2UI_Handle ui)
{
    MyAmpUI* myUi = static_cast<MyAmpUI*>(ui);
    myUi->handleEvents();
    return 0;
}

static const void * extension_data (const char *uri)
{
    static const LV2UI_Idle_Interface idle = { ui_idle };
    if (strcmp (uri, LV2_UI__idleInterface) == 0) return &idle;
    return nullptr;
}

static const LV2UI_Descriptor ui_descriptor =
{
    "https://github.com/sjaehn/lv2tutorial/myAmp_BWidgets#ui",
    instantiate,
    cleanup,
    port_event,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor * 	lv2ui_descriptor (uint32_t index)
{
    switch (index)
    {
        case 0:     return &ui_descriptor;
        default:    return 0;
    }
}

