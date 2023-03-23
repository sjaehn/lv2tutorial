#include <iostream>
#include "cairo/cairo.h"
#include "myAmp_Xputty.hpp"
#include <stdexcept>
#include "xadjustment.h"
#include "xputty.h"
#include "xwidget.h"
#include "xwidgets.h"

MyAmpUI::MyAmpUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, std::string bundlePath):
    write_function(write_function),
    controller(controller)
{
    main_init (&main);
    box = create_window (&main, reinterpret_cast<Window>(parentXWindow), 0, 0, 240, 200);
    box->parent_struct = this;
    box->label = "MyAmp_Xputty";
    std::string filename = bundlePath + "/Plugin.png";
    widget_get_png_from_file (box, filename.c_str());
    box->func.expose_callback = exposeCallback;

    dial = add_knob (box, "", 80, 50, 100, 100);
    dial->parent_struct = this;
    dial->data = 2;
    dial->func.value_changed_callback = valueChangedCallback;
    set_adjustment (dial->adj, 1.0, 1.0, 0.0, 2.0, 0.01, CL_CONTINUOS);
    widget_show_all (box);
}

MyAmpUI::~MyAmpUI()
{
    main_quit (&main);
}

LV2UI_Widget MyAmpUI::getTopLevelWidget()
{
    return reinterpret_cast<LV2UI_Widget>(box->widget);
}

Xputty* MyAmpUI::getXputty()
{
    return &main;
}

void MyAmpUI::portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    if (format == 0)
    {
        const float value = *static_cast<const float*>(buffer);
        switch (port_index)
        {
            case 2:     adj_set_value (dial->adj, value);
                        break;

            default:    break;
        }
    }
}

void MyAmpUI::valueChangedCallback (void* obj, void* data)
{
    Widget_t* widget = static_cast<Widget_t*>(obj);
    MyAmpUI* ui = static_cast<MyAmpUI*>(widget->parent_struct);
    if (ui)
    {
        float gain = adj_get_value (widget->adj);
        ui->write_function (ui->controller, 2, sizeof(gain), 0, &gain);
    }
}

void MyAmpUI::exposeCallback (void* obj, void* data)
{
    Widget_t* widget = static_cast<Widget_t*>(obj);
    cairo_set_source_surface (widget->crb, widget->image, 0, 0);
    cairo_paint (widget->crb);
}

static LV2UI_Handle instantiate(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features)
{
    if (strcmp (plugin_uri, "https://github.com/sjaehn/lv2tutorial/myAmp_Xputty") != 0) return nullptr;
    
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
    run_embedded (myUi->getXputty());
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
    "https://github.com/sjaehn/lv2tutorial/myAmp_Xputty#ui",
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

