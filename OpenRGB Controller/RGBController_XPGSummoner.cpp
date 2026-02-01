/*---------------------------------------------------------*\
| RGBController_XPGSummoner.cpp                             |
|                                                           |
|   RGBController for XPG Summoner keyboard                 |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_XPGSummoner.h"

#define NA 0xFFFFFFFF
#define LED_COUNT 105

/*---------------------------------------------------------*\
| matrix_map_iso: Logical LED mapping for ISO layout       |
| Based on SignalRGB vKeys mapping                          |
\*---------------------------------------------------------*/
static unsigned int matrix_map_iso[6][21] =
{
    {11, NA, 22, 30, 25, 27, 7,  51, 57, 62, 86, 87, 83, 85, 79,  72,  0,   NA, NA,  NA,  NA},
    {14, 15, 23, 31, 39, 38, 46, 47, 55, 63, 71, 70, 54, 81, 102, 118, 110, 92, 100, 108, 109},
    {9,  8,  16, 24, 32, 33, 41, 40, 48, 56, 64, 65, 49, NA, 94,  119, 111, 88, 96,  104, 112},
    {17, 10, 18, 26, 34, 35, 43, 42, 50, 58, 66, 67, 68,NA, NA,  NA,  84,  89, 97,  105, NA},
    {121,19,12, 20, 28, 36, 37, 45, 44, 52, 60, 69, 122,NA, 115, NA,  90,  NA, 98,  106, 114},
    {6,  124,75, NA, NA, 91, NA, NA, 77, 125,61, 4,  117,93, 101, NA,  99,  NA, NA,  107, NA}
};

/*---------------------------------------------------------*\
| ordered_matrix_iso: Physical LED layout for display       |
\*---------------------------------------------------------*/
static unsigned int ordered_matrix_iso[6][21] =
{
    {0,  NA, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,  15,  NA,  NA,  NA, NA},
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  32,  33,  34,  35, 36},
    {37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, NA, 50, 51,  52,  53,  54,  55, 56},
    {57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, NA, NA, NA,  70,  71,  72,  73, NA},
    {74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, NA, 86, NA, 87,  NA,  88,  89,  90, 91},
    {92, 93, 94, NA, NA, 95, NA, NA, 96, 97, 98, 99, 100,101,102,NA,  103, NA,  NA,  104,NA}
};

/*---------------------------------------------------------*\
| zone_names: Zone names                                    |
\*---------------------------------------------------------*/
const char *zone_names[] =
{
    ZONE_EN_KEYBOARD
};

zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

const unsigned int zone_sizes[] =
{
    LED_COUNT
};

/*---------------------------------------------------------*\
| led_names_iso: LED names for ISO-105 layout              |
\*---------------------------------------------------------*/
static const char *led_names_iso[] =
{
    KEY_EN_ESCAPE,          // 0
    KEY_EN_F1,              // 1
    KEY_EN_F2,              // 2
    KEY_EN_F3,              // 3
    KEY_EN_F4,              // 4
    KEY_EN_F5,              // 5
    KEY_EN_F6,              // 6
    KEY_EN_F7,              // 7
    KEY_EN_F8,              // 8
    KEY_EN_F9,              // 9
    KEY_EN_F10,             // 10
    KEY_EN_F11,             // 11
    KEY_EN_F12,             // 12
    KEY_EN_PRINT_SCREEN,    // 13
    KEY_EN_SCROLL_LOCK,     // 14
    KEY_EN_PAUSE_BREAK,     // 15
    KEY_EN_BACK_TICK,       // 16
    KEY_EN_1,               // 17
    KEY_EN_2,               // 18
    KEY_EN_3,               // 19
    KEY_EN_4,               // 20
    KEY_EN_5,               // 21
    KEY_EN_6,               // 22
    KEY_EN_7,               // 23
    KEY_EN_8,               // 24
    KEY_EN_9,               // 25
    KEY_EN_0,               // 26
    KEY_EN_MINUS,           // 27
    KEY_EN_EQUALS,          // 28
    KEY_EN_BACKSPACE,       // 29
    KEY_EN_INSERT,          // 30
    KEY_EN_HOME,            // 31
    KEY_EN_PAGE_UP,         // 32
    KEY_EN_NUMPAD_LOCK,     // 33
    KEY_EN_NUMPAD_DIVIDE,   // 34
    KEY_EN_NUMPAD_TIMES,    // 35
    KEY_EN_NUMPAD_MINUS,    // 36
    KEY_EN_TAB,             // 37
    KEY_EN_Q,               // 38
    KEY_EN_W,               // 39
    KEY_EN_E,               // 40
    KEY_EN_R,               // 41
    KEY_EN_T,               // 42
    KEY_EN_Y,               // 43
    KEY_EN_U,               // 44
    KEY_EN_I,               // 45
    KEY_EN_O,               // 46
    KEY_EN_P,               // 47
    KEY_EN_LEFT_BRACKET,    // 48
    KEY_EN_RIGHT_BRACKET,   // 49
    KEY_EN_DELETE,          // 50
    KEY_EN_END,             // 51
    KEY_EN_PAGE_DOWN,       // 52
    KEY_EN_NUMPAD_7,        // 53
    KEY_EN_NUMPAD_8,        // 54
    KEY_EN_NUMPAD_9,        // 55
    KEY_EN_NUMPAD_PLUS,     // 56
    KEY_EN_CAPS_LOCK,       // 57
    KEY_EN_A,               // 58
    KEY_EN_S,               // 59
    KEY_EN_D,               // 60
    KEY_EN_F,               // 61
    KEY_EN_G,               // 62
    KEY_EN_H,               // 63
    KEY_EN_J,               // 64
    KEY_EN_K,               // 65
    KEY_EN_L,               // 66
    KEY_EN_SEMICOLON,       // 67
    KEY_EN_QUOTE,           // 68
    KEY_EN_POUND,           // 69 - ISO Hash/Tilde key
    KEY_EN_ISO_ENTER,       // 70
    KEY_EN_NUMPAD_4,        // 71
    KEY_EN_NUMPAD_5,        // 72
    KEY_EN_NUMPAD_6,        // 73
    KEY_EN_LEFT_SHIFT,      // 74
    KEY_EN_ISO_BACK_SLASH,  // 75 - ISO key next to left shift
    KEY_EN_Z,               // 76
    KEY_EN_X,               // 77
    KEY_EN_C,               // 78
    KEY_EN_V,               // 79
    KEY_EN_B,               // 80
    KEY_EN_N,               // 81
    KEY_EN_M,               // 82
    KEY_EN_COMMA,           // 83
    KEY_EN_PERIOD,          // 84
    KEY_EN_FORWARD_SLASH,   // 85
    KEY_EN_RIGHT_SHIFT,     // 86
    KEY_EN_UP_ARROW,        // 87
    KEY_EN_NUMPAD_1,        // 88
    KEY_EN_NUMPAD_2,        // 89
    KEY_EN_NUMPAD_3,        // 90
    KEY_EN_NUMPAD_ENTER,    // 91
    KEY_EN_LEFT_CONTROL,    // 92
    KEY_EN_LEFT_WINDOWS,    // 93
    KEY_EN_LEFT_ALT,        // 94
    KEY_EN_SPACE,           // 95
    KEY_EN_RIGHT_ALT,       // 96
    KEY_EN_RIGHT_FUNCTION,  // 97
    KEY_EN_MENU,            // 98
    KEY_EN_RIGHT_CONTROL,   // 99
    KEY_EN_LEFT_ARROW,      // 100
    KEY_EN_DOWN_ARROW,      // 101
    KEY_EN_RIGHT_ARROW,     // 102
    KEY_EN_NUMPAD_0,        // 103
    KEY_EN_NUMPAD_PERIOD    // 104
};

/**------------------------------------------------------------------*\
    @name XPG Summoner Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectXPGSummonerControllers
    @comment
\*-------------------------------------------------------------------*/

/*---------------------------------------------------------*\
| RGBController_XPGSummoner constructor                     |
\*---------------------------------------------------------*/
RGBController_XPGSummoner::RGBController_XPGSummoner(XPGSummonerController *controller_ptr)
{
    controller   = controller_ptr;
    name         = controller->GetNameString();
    vendor       = "XPG";
    description  = "XPG Summoner Keyboard Device";
    location     = controller->GetLocationString();
    serial       = controller->GetSerialString();
    type         = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name        = "Direct";
    Direct.value       = XPG_SUMMONER_MODE_DIRECT;
    Direct.flags       = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode  = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name           = "Static";
    Static.value          = XPG_SUMMONER_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.brightness_min = 5;
    Static.brightness_max = 100;
    Static.brightness     = 100;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Stars;
    Stars.name         = "Stars";
    Stars.value        = XPG_SUMMONER_MODE_STARS;
    Stars.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Stars.colors_min   = 1;
    Stars.colors_max   = 1;
    Stars.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Stars.colors.resize(1);
    modes.push_back(Stars);

    mode Off;
    Off.name           = "Off";
    Off.value          = XPG_SUMMONER_MODE_OFF;
    Off.flags          = 0;
    Off.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

/*---------------------------------------------------------*\
| Destructor                                                |
\*---------------------------------------------------------*/
RGBController_XPGSummoner::~RGBController_XPGSummoner()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
    delete controller;
}

/*---------------------------------------------------------*\
| SetupZones: Initializes zones and LEDs                    |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::SetupZones()
{
    leds.clear();
    colors.clear();
    zones.clear();
    leds.reserve(LED_COUNT);
    colors.reserve(LED_COUNT);
    zones.reserve(1);

    zone new_zone;
    new_zone.name       = zone_names[0];
    new_zone.type       = zone_types[0];
    new_zone.leds_min   = zone_sizes[0];
    new_zone.leds_max   = zone_sizes[0];
    new_zone.leds_count = zone_sizes[0];

    if(new_zone.type == ZONE_TYPE_MATRIX)
    {
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 21;
        new_zone.matrix_map->map    = (unsigned int *)&ordered_matrix_iso;
    }
    else
    {
        new_zone.matrix_map = NULL;
    }
    zones.push_back(new_zone);

    size_t linear_idx = 0;
    for(int row = 0; row < 6; ++row)
    {
        for(int col = 0; col < 21; ++col)
        {
            unsigned int led_id = matrix_map_iso[row][col];
            if(led_id == NA)
                continue;
            led new_led;
            new_led.name  = led_names_iso[linear_idx];
            new_led.value = led_id;
            leds.push_back(new_led);
            ++linear_idx;
        }
    }
    colors.assign(LED_COUNT, 0x000000);

    SetupColors();
}

/*---------------------------------------------------------*\
| ResizeZone: Not supported for this device                 |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // This device does not support resizing zones
}

/*---------------------------------------------------------*\
| DeviceUpdateLEDs: Updates LED colors                      |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::DeviceUpdateLEDs()
{
    const unsigned int frame_buf_length       = 126 * 4;
    unsigned char frame_buf[frame_buf_length] = {0};

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(leds[led_idx].value == NA)
            continue;

        std::size_t real_idx = leds[led_idx].value;
        
        if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
        {
            frame_buf[(real_idx * 4) + 0] = 0x64;  // Brightness (100)
            frame_buf[(real_idx * 4) + 1] = RGBGetRValue(colors[led_idx]);
            frame_buf[(real_idx * 4) + 2] = RGBGetGValue(colors[led_idx]);
            frame_buf[(real_idx * 4) + 3] = RGBGetBValue(colors[led_idx]);
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && modes[active_mode].value == XPG_SUMMONER_MODE_STATIC)
        {
            frame_buf[(real_idx * 4) + 0] = modes[active_mode].brightness;
            frame_buf[(real_idx * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
            frame_buf[(real_idx * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
            frame_buf[(real_idx * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && modes[active_mode].value == XPG_SUMMONER_MODE_STARS)
        {
            // Clear all LEDs first
            for(std::size_t i = 0; i < leds.size(); ++i)
            {
                std::size_t idx = leds[i].value;
                frame_buf[(idx * 4) + 0] = 0;
                frame_buf[(idx * 4) + 1] = 0;
                frame_buf[(idx * 4) + 2] = 0;
                frame_buf[(idx * 4) + 3] = 0;
            }
            controller->SendColors(frame_buf, sizeof(frame_buf));

            // Select random center LED
            int rows = 6;
            int cols = 21;
            int center_row = rand() % rows;
            int center_col = rand() % cols;
            unsigned int center_led = matrix_map_iso[center_row][center_col];

            if(center_led != NA)
            {
                frame_buf[(center_led * 4) + 0] = 0x64;
                frame_buf[(center_led * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
                frame_buf[(center_led * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
                frame_buf[(center_led * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
            }

            // Add fade to neighbors
            unsigned char fade_brightness = 0x32;
            int neighbor_offsets[4][2] = { {0, -1}, {0, +1}, {-1, 0}, {+1, 0} };

            for(int k = 0; k < 4; ++k)
            {
                int n_row = center_row + neighbor_offsets[k][0];
                int n_col = center_col + neighbor_offsets[k][1];
                if(n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols)
                {
                    unsigned int neighbor_led = matrix_map_iso[n_row][n_col];
                    if(neighbor_led != NA)
                    {
                        frame_buf[(neighbor_led * 4) + 0] = fade_brightness;
                        frame_buf[(neighbor_led * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
                        frame_buf[(neighbor_led * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
                        frame_buf[(neighbor_led * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
                    }
                }
            }
            controller->SendColors(frame_buf, sizeof(frame_buf));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
        {
            frame_buf[(real_idx * 4) + 0] = 0;
            frame_buf[(real_idx * 4) + 1] = 0;
            frame_buf[(real_idx * 4) + 2] = 0;
            frame_buf[(real_idx * 4) + 3] = 0;
        }
    }
    controller->SendColors(frame_buf, sizeof(frame_buf));
}

/*---------------------------------------------------------*\
| UpdateZoneLEDs: Updates all LEDs in a zone                |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

/*---------------------------------------------------------*\
| UpdateSingleLED: Updates a single LED                     |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

/*---------------------------------------------------------*\
| DeviceUpdateMode: Updates device mode                     |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}