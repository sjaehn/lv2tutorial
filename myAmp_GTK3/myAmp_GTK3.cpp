#include <iostream>
#include "myAmp_GTK3.hpp"
#include <stdexcept>

MyAmpUI::MyAmpUI (LV2UI_Write_Function write_function, LV2UI_Controller controller):
    write_function(write_function),
    controller(controller)
{
    box = gtk_vbox_new (false, 0);
    slider = gtk_vscale_new_with_range (0.0, 2.0, 0.01);
    gtk_range_set_value (reinterpret_cast<GtkRange*>(slider), 1.0);
    g_signal_connect_after (slider, "value-changed", G_CALLBACK(valueChangedCallback), this);
    gtk_container_add (reinterpret_cast<GtkContainer*>(box), slider);
    gtk_widget_set_size_request (box, 50, 200);
}

LV2UI_Widget MyAmpUI::getMain()
{
    return static_cast<LV2UI_Widget>(box);
}

void MyAmpUI::portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    if (format == 0)
    {
        const float value = *static_cast<const float*>(buffer);
        switch (port_index)
        {
            case 2:     gtk_range_set_value (reinterpret_cast<GtkRange*>(slider), value);
                        break;

            default:    break;
        }
    }
}

int MyAmpUI::valueChangedCallback (GtkWidget* widget, gpointer data)
{
    MyAmpUI* ui = static_cast<MyAmpUI*>(data);
    if (ui)
    {
        float gain = gtk_range_get_value (reinterpret_cast<GtkRange*>(widget));
        ui->write_function (ui->controller, 2, sizeof(gain), 0, &gain);
    }

    return 0;
}

static LV2UI_Handle instantiate(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features)
{
    if (strcmp (plugin_uri, "https://github.com/sjaehn/lv2tutorial/myAmp_GTK3") != 0) return nullptr;
    MyAmpUI* ui;
    try {ui = new MyAmpUI(write_function, controller);}
    catch (std::exception& exc)
    {
        std::cerr << "UI instantiation failed." << std::endl;
        return nullptr;
    }

    *widget = ui->getMain();
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

static const void * extension_data (const char *uri)
{
    return nullptr;
}

static const LV2UI_Descriptor ui_descriptor =
{
    "https://github.com/sjaehn/lv2tutorial/myAmp_GTK3#ui",
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

