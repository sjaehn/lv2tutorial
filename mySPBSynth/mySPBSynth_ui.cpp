#include <iostream>
#include "BWidgets/BEvents/Event.hpp"
#include "BWidgets/BStyles/Status.hpp"
#include "BWidgets/BStyles/Types/ColorMap.hpp"
#include "BWidgets/BStyles/Types/Fill.hpp"
#include "BWidgets/BUtilities/cairoplus.h"
#include "BWidgets/BWidgets/Supports/Clickable.hpp"
#include "BWidgets/BWidgets/Supports/ValueableTyped.hpp"
#include "BWidgets/BWidgets/ValueDial.hpp"
#include "BWidgets/BWidgets/Widget.hpp"
#include "ControlLimits.hpp"
#include "ControlPorts.hpp"
#include "PortGroups.hpp"
#include "Waveform.hpp"
#include "cairo/cairo.h"
#include "mySPBSynth_ui.hpp"
#include "pugl/pugl.h"
#include <stdexcept>
#include <random>
#include <ctime>

MySPBSynthUI::MySPBSynthUI (LV2UI_Write_Function write_function, LV2UI_Controller controller, void* parentXWindow, std::string bundlePath):
    Window (600, 200, reinterpret_cast<PuglNativeView>(parentXWindow), 0, "MySPBSynth", false, PUGL_MODULE, 0),
    write_function(write_function),
    controller(controller),
    waveform (220, 30, 160, 20, {"Sine", "Triangle", "Square", "Saw", "Noise"}, 1),
    attack (480, 20, 100, 30, 0.1, controlLimits[CONTROL_ATTACK].first, controlLimits[CONTROL_ATTACK].second),
    decay (480, 60, 100, 30, 0.1, controlLimits[CONTROL_DECAY].first, controlLimits[CONTROL_DECAY].second),
    sustain (480, 100, 100, 30, 0.1, controlLimits[CONTROL_SUSTAIN].first, controlLimits[CONTROL_SUSTAIN].second),
    release (480, 140, 100, 30, 0.1, controlLimits[CONTROL_RELEASE].first, controlLimits[CONTROL_RELEASE].second),
    level (50, 60, 100, 100, 0.1, controlLimits[CONTROL_LEVEL].first, controlLimits[CONTROL_LEVEL].second),
    display (220, 60, 160, 100)
{
    widgets[CONTROL_WAVEFORM] = &waveform;
    widgets[CONTROL_ATTACK] = &attack;
    widgets[CONTROL_DECAY] = &decay;
    widgets[CONTROL_SUSTAIN] = &sustain;
    widgets[CONTROL_RELEASE] = &release;
    widgets[CONTROL_LEVEL] = &level;

    for (BWidgets::Widget* w : widgets)
    {
        if (w != &waveform)
        {
            dynamic_cast<BWidgets::Clickable*>(w)->setClickable(false);
            w->setFgColors(BStyles::whites);
        }

        w->setCallbackFunction (BEvents::Event::EventType::valueChangedEvent, valueChangedCallback);
        add (w);
    }

    display.createImage(BStyles::Status::normal);
    drawWaveform();
    add(&display);

    setBackground(BStyles::Fill (bundlePath + "/Plugin.png"));
}

LV2UI_Widget MySPBSynthUI::getTopLevelWidget()
{
    return reinterpret_cast<LV2UI_Widget>(puglGetNativeView(getView()));
}

void MySPBSynthUI::portEvent(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    if (format == 0)
    {
        const float value = *static_cast<const float*>(buffer);
        const int controlNr = port_index - PORT_CONTROL;
        switch (controlNr)
        {
            case CONTROL_WAVEFORM:      waveform.setValue(value + 1);
                                        break;

            default:                    if ((controlNr >= CONTROL_ATTACK) && (controlNr < CONTROL_NR))
                                        {
                                            BWidgets::ValueableTyped<double>* widget = dynamic_cast<BWidgets::ValueableTyped<double>*>(widgets[controlNr]);
                                            widget->setValue(value);
                                        }
                                        break;
        }
    }
}

void MySPBSynthUI::valueChangedCallback (BEvents::Event* event)
{
    BWidgets::Widget* widget = event->getWidget();
    MySPBSynthUI* ui = dynamic_cast<MySPBSynthUI*>(widget->getMainWindow());

    if (ui)
    {
        float value = 0.0f;
        int controlNr = 0;

        for (controlNr = 0; controlNr < CONTROL_NR; ++controlNr)
        {
            BWidgets::Widget* w = ui->widgets[controlNr];
            if (w == widget)
            {
                if (w == &ui->waveform) value = ui->waveform.getValue() - 1;
                else value = dynamic_cast<BWidgets::ValueableTyped<double>*>(widget)->getValue();

                if ((w == &ui->waveform) || (w == &ui->level)) ui->drawWaveform();

                break;
            }
        }

        ui->write_function (ui->controller, PORT_CONTROL + controlNr, sizeof(value), 0, &value);
    }
}

void MySPBSynthUI::drawWaveform()
{
    cairo_surface_t* surface = display.getImageSurface(BStyles::Status::normal);
    cairoplus_surface_clear(surface);
    cairo_t* cr = cairo_create(surface);
    const double w = cairo_image_surface_get_width(surface);
    const double h = cairo_image_surface_get_height(surface);
    const double l = level.getValue();
    const int f = waveform.getValue() - 1;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_set_line_width(cr, 2.0);

    switch (f)
    {
        case WAVEFORM_SINE:     cairo_move_to (cr, 0.0, 0.5 * h);
                                for (double x = 0.0; x <= 1.0; x += 1.0 / 32.0)
                                {
                                    cairo_line_to (cr, x * w, 0.5 * h - 0.5 * h * sin (2.0 * M_PI * x) * l);
                                }
                                break;

        case WAVEFORM_TRIANGLE: cairo_move_to (cr, 0.0, 0.5 * h);
                                cairo_line_to (cr, 0.25 * w, 0.5 * h - 0.5 * h * l);
                                cairo_line_to (cr, 0.75 * w, 0.5 * h + 0.5 * h * l);
                                cairo_line_to (cr, w, 0.5 * h);
                                break;

        case WAVEFORM_SQUARE:   cairo_move_to (cr, 0.0, 0.5 * h);
                                cairo_line_to (cr, 0.0, 0.5 * h - 0.5 * h * l);
                                cairo_line_to (cr, 0.5 * w, 0.5 * h - 0.5 * h * l);
                                cairo_line_to (cr, 0.5 * w, 0.5 * h + 0.5 * h * l);
                                cairo_line_to (cr, w, 0.5 * h + 0.5 * h * l);
                                cairo_line_to (cr, w, 0.5 * h);
                                break;

        case WAVEFORM_SAW:      cairo_move_to (cr, 0.0, 0.5 * h);
                                cairo_line_to (cr, 0.0, 0.5 * h + 0.5 * h * l);
                                cairo_line_to (cr, w, 0.5 * h - 0.5 * h * l);
                                cairo_line_to (cr, w, 0.5 * h);
                                break;

        case WAVEFORM_NOISE:    {
                                    std::minstd_rand rnd;
                                    std::uniform_real_distribution<float> dist;
                                    cairo_move_to (cr, 0.0, 0.5 * h);
                                    for (double x = 0.0; x < 1.0; x += 1.0 / 160.0)
                                    {
                                        cairo_line_to (cr, x * w, 0.5 * h - 0.5 * h * l * dist (rnd));
                                    }
                                    cairo_line_to (cr, 1.0, 0.5 * h);
                                }
                                break;

        default:                break;
    }

    cairo_stroke (cr);
    cairo_destroy (cr);
    display.update();
}

static LV2UI_Handle instantiate(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features)
{
    if (strcmp (plugin_uri, "https://github.com/sjaehn/lv2tutorial/mySPBSynth") != 0) return nullptr;

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

    MySPBSynthUI* ui;
    try {ui = new MySPBSynthUI(write_function, controller, parentXWindow, std::string (bundle_path));}
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
    MySPBSynthUI* myUi = static_cast<MySPBSynthUI*>(ui);
    if (myUi) delete myUi;
}

static void port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer)
{
    MySPBSynthUI* myUi = static_cast<MySPBSynthUI*>(ui);
    if (myUi) myUi->portEvent(port_index, buffer_size, format, buffer);
}

static int ui_idle (LV2UI_Handle ui)
{
    MySPBSynthUI* myUi = static_cast<MySPBSynthUI*>(ui);
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
    "https://github.com/sjaehn/lv2tutorial/mySPBSynth#ui",
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

