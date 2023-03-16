#ifndef MYAMP_GTK_HPP
#define MYAMP_GTK_HPP

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <gtk/gtk.h>

class MyAmpUI
{
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    GtkWidget* slider;
    GtkWidget* box;

public:
    MyAmpUI (LV2UI_Write_Function write_function, LV2UI_Controller controller);
    LV2UI_Widget getMain();
    void portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer);
    static int valueChangedCallback (GtkWidget* widget, gpointer data);
};

#endif