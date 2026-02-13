#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#define screenWidth   800
#define screenHeight  480

class LGFX : public lgfx::LGFX_Device
{
public:
    lgfx::Bus_RGB _bus;
    lgfx::Panel_RGB _panel;
    lgfx::Light_PWM _light;

    LGFX(void)
    {
        {
            auto cfg = _panel.config();
            cfg.memory_width  = screenWidth;
            cfg.memory_height = screenHeight;
            cfg.panel_width   = screenWidth;
            cfg.panel_height  = screenHeight;
            _panel.config(cfg);
        }

        {
            auto cfg = _bus.config();
            cfg.panel = &_panel;

            cfg.pin_d0  = GPIO_NUM_8;
            cfg.pin_d1  = GPIO_NUM_3;
            cfg.pin_d2  = GPIO_NUM_46;
            cfg.pin_d3  = GPIO_NUM_9;
            cfg.pin_d4  = GPIO_NUM_1;

            cfg.pin_d5  = GPIO_NUM_5;
            cfg.pin_d6  = GPIO_NUM_6;
            cfg.pin_d7  = GPIO_NUM_7;
            cfg.pin_d8  = GPIO_NUM_15;
            cfg.pin_d9  = GPIO_NUM_16;
            cfg.pin_d10 = GPIO_NUM_4;

            cfg.pin_d11 = GPIO_NUM_45;
            cfg.pin_d12 = GPIO_NUM_48;
            cfg.pin_d13 = GPIO_NUM_47;
            cfg.pin_d14 = GPIO_NUM_21;
            cfg.pin_d15 = GPIO_NUM_14;

            cfg.pin_henable = GPIO_NUM_40;
            cfg.pin_vsync   = GPIO_NUM_41;
            cfg.pin_hsync   = GPIO_NUM_39;
            cfg.pin_pclk    = GPIO_NUM_0;

            cfg.freq_write = 16000000;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 8;
            cfg.hsync_pulse_width = 4;
            cfg.hsync_back_porch  = 43;

            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 8;
            cfg.vsync_pulse_width = 4;
            cfg.vsync_back_porch  = 12;

            cfg.pclk_active_neg = 1;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _light.config();
            cfg.pin_bl = GPIO_NUM_2;
            _light.config(cfg);
            _panel.light(&_light);
        }

        setPanel(&_panel);
    }
};

LGFX tft;
